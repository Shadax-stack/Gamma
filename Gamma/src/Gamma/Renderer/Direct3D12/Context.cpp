#include "Context.h"
#include "../../Core/Log.h"
#include "../../Win32/Direct3DUtils.h"
#include <iostream>

#ifdef GAMMA_MSVC
#pragma warning(disable : 26495)
#pragma warning(disable : 26812)
#pragma warning(disable : 6387)
#endif

namespace Gamma {
	namespace Direct3D12 {

		Context::Context(void) : CurrentFencePoint(0), CurrentBackBufferIndex(0), RTVDescriptorSize(0), DSVDescriptorSize(0), CBV_SRV_UAVDescriptorSize(0), SwapChainWindow(NULL) {}

		Context::Context(Window* window) : Context() {
			Initialize(window);
		}

		Context::~Context(void) {
			FlushCommandQueue();
			SwapChain->SetFullscreenState(FALSE, NULL);
		}

		void Context::Initialize(Window* window, uint32_t msaa_samples) {
			SwapChainWindow = window;
			CreateContext(window, msaa_samples);
		}

		void Context::CreateContext(Window* window, UINT msaa_samples) {
			#ifdef GAMMA_DEBUG
			Microsoft::WRL::ComPtr<ID3D12Debug> DebugController;
			D3D12GetDebugInterface(__uuidof(ID3D12Debug), (void**)DebugController.GetAddressOf());
			DebugController->EnableDebugLayer();
			#endif
			DXGI_RATIONAL RefreshRate;
			RefreshRate.Numerator = 60;
			RefreshRate.Denominator = 1;
			Microsoft::WRL::ComPtr<IDXGIFactory4> Factory;
			CreateDXGIFactory1(__uuidof(IDXGIFactory4), (void**)Factory.GetAddressOf());
			Factory->EnumAdapters(0, Adapter.GetAddressOf());
			Adapter->EnumOutputs(0, Output.GetAddressOf());
			DXGI_OUTPUT_DESC MonitorDesc = {};
			Output->GetDesc(&MonitorDesc);
			MONITORINFOEXW MonitorInfo;
			MonitorInfo.cbSize = sizeof(MonitorInfo);
			GetMonitorInfoW(MonitorDesc.Monitor, &MonitorInfo);
			UINT32 RequiredPaths, RequiredModes;
			GetDisplayConfigBufferSizes(QDC_ONLY_ACTIVE_PATHS, &RequiredPaths, &RequiredModes);
			std::vector<DISPLAYCONFIG_PATH_INFO> PathsList(RequiredPaths);
			std::vector<DISPLAYCONFIG_MODE_INFO> ModesList(RequiredModes);
			QueryDisplayConfig(QDC_ONLY_ACTIVE_PATHS, &RequiredPaths, PathsList.data(), &RequiredModes, ModesList.data(), nullptr);
			int32_t MonitorIndex = 0;
			for (DISPLAYCONFIG_PATH_INFO PathInfo : PathsList) {
				DISPLAYCONFIG_SOURCE_DEVICE_NAME SourceName;
				SourceName.header.type = DISPLAYCONFIG_DEVICE_INFO_GET_SOURCE_NAME;
				SourceName.header.size = sizeof(SourceName);
				SourceName.header.adapterId = PathInfo.sourceInfo.adapterId;
				SourceName.header.id = PathInfo.sourceInfo.id;
				DisplayConfigGetDeviceInfo(&SourceName.header);
				if (wcscmp(MonitorInfo.szDevice, SourceName.viewGdiDeviceName) == 0) {
					RefreshRate.Numerator = PathInfo.targetInfo.refreshRate.Numerator;
					RefreshRate.Denominator = PathInfo.targetInfo.refreshRate.Denominator;
					break;
				}
				MonitorIndex++;
			}
			// Create the ID3D12Device using the first adapter in the system
			D3D12CreateDevice(Adapter.Get(), D3D_FEATURE_LEVEL_12_0, __uuidof(ID3D12Device), (void**)Device.GetAddressOf());
			D3D12_COMMAND_QUEUE_DESC CommandQueueDesc = {};
			CommandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
			Device->CreateCommandQueue(&CommandQueueDesc, __uuidof(ID3D12CommandQueue), (void**)CommandQueue.GetAddressOf());
			Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, __uuidof(ID3D12CommandAllocator), (void**)CommandAllocator.GetAddressOf());
			Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, CommandAllocator.Get(), NULL, __uuidof(ID3D12GraphicsCommandList), (void**)CommandList.GetAddressOf());
			Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, __uuidof(ID3D12Fence), (void**)Fence.GetAddressOf());
			RTVDescriptorSize = Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
			DSVDescriptorSize = Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
			CBV_SRV_UAVDescriptorSize = Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
			D3D12_DESCRIPTOR_HEAP_DESC RTVDescriptorHeapDesc = {};
			RTVDescriptorHeapDesc.NumDescriptors = GAMMA_D3D12_SWAP_CHAIN_BUFFER_COUNT;
			RTVDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
			RTVDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			RTVDescriptorHeapDesc.NodeMask = 0;
			Device->CreateDescriptorHeap(&RTVDescriptorHeapDesc, __uuidof(ID3D12DescriptorHeap), (void**)RTVDescriptorHeap.GetAddressOf());
			D3D12_DESCRIPTOR_HEAP_DESC DSVDescriptorHeapDesc = {};
			DSVDescriptorHeapDesc.NumDescriptors = 1;
			DSVDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
			DSVDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			DSVDescriptorHeapDesc.NodeMask = 0;
			Device->CreateDescriptorHeap(&DSVDescriptorHeapDesc, __uuidof(ID3D12DescriptorHeap), (void**)DSVDescriptorHeap.GetAddressOf());
			D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS MultisampleQualityLevels;
			MultisampleQualityLevels.SampleCount = msaa_samples;
			MultisampleQualityLevels.NumQualityLevels = 0;
			MultisampleQualityLevels.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			MultisampleQualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
			Device->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, (void*)&MultisampleQualityLevels, sizeof(D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS));
			DXGI_SWAP_CHAIN_DESC SwapChainDesc = {}; 
			SwapChainDesc.BufferDesc.Width = window->GetDimensions().Width;
			SwapChainDesc.BufferDesc.Height = window->GetDimensions().Height;
			SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			SwapChainDesc.BufferDesc.RefreshRate = RefreshRate;
			SwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
			SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			SwapChainDesc.SampleDesc.Count = MultisampleQualityLevels.SampleCount;
			SwapChainDesc.SampleDesc.Quality = MultisampleQualityLevels.NumQualityLevels - 1;
			SwapChainDesc.OutputWindow = Win32::GetWindowHandle(window);
			SwapChainDesc.Windowed = !window->GetFullscreenState();
			SwapChainDesc.BufferCount = GAMMA_D3D12_SWAP_CHAIN_BUFFER_COUNT;
			SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
			SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
			Factory->CreateSwapChain(CommandQueue.Get(), &SwapChainDesc, SwapChain.GetAddressOf());
			// Create color buffers and RTVs
			D3D12_CPU_DESCRIPTOR_HANDLE RTVHeapBase = RTVDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
			for (UINT index = 0; index < GAMMA_D3D12_SWAP_CHAIN_BUFFER_COUNT; index++) {
				SwapChain->GetBuffer(index, __uuidof(ID3D12Resource), (void**)SwapChainBuffers[index].GetAddressOf());
				Device->CreateRenderTargetView(SwapChainBuffers[index].Get(), NULL, { RTVHeapBase.ptr + (UINT64)index * RTVDescriptorSize });
			}
			// Create depth-stencil buffer and DSV
			D3D12_RESOURCE_DESC DepthStencilDesc = {};
			DepthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
			DepthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			DepthStencilDesc.Width = SwapChainDesc.BufferDesc.Width;
			DepthStencilDesc.Height = SwapChainDesc.BufferDesc.Height;
			DepthStencilDesc.DepthOrArraySize = 1;
			DepthStencilDesc.MipLevels = 1;
			DepthStencilDesc.SampleDesc = SwapChainDesc.SampleDesc;
			DepthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
			DepthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
			D3D12_HEAP_PROPERTIES DepthStencilHeapProperties = {};
			DepthStencilHeapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;
			DepthStencilHeapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
			DepthStencilHeapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
			DepthStencilHeapProperties.CreationNodeMask = 0;
			DepthStencilHeapProperties.VisibleNodeMask = 0;
			D3D12_CLEAR_VALUE DepthStencilClearValue = {};
			DepthStencilClearValue.Format = DepthStencilDesc.Format;
			DepthStencilClearValue.DepthStencil.Depth = 1.0;
			DepthStencilClearValue.DepthStencil.Stencil = 0;
			Device->CreateCommittedResource(&DepthStencilHeapProperties, D3D12_HEAP_FLAG_NONE, &DepthStencilDesc, D3D12_RESOURCE_STATE_COMMON, &DepthStencilClearValue, __uuidof(ID3D12Resource), (void**)DepthStencilBuffer.GetAddressOf());
			Device->CreateDepthStencilView(DepthStencilBuffer.Get(), NULL, DepthStencilDSV());
			CommandList->ResourceBarrier(1, &CreateResourceBarrierTransition(DepthStencilBuffer.Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE));
			Viewport.TopLeftX = 0.0f;
			Viewport.TopLeftY = 0.0f;
			Viewport.Width = (FLOAT)SwapChainDesc.BufferDesc.Width;
			Viewport.Height = (FLOAT)SwapChainDesc.BufferDesc.Height;
			Viewport.MinDepth = 0.0f;
			Viewport.MaxDepth = 1.0f;
			CommandList->RSSetViewports(1, &Viewport);
			CommandList->Close();
			ID3D12CommandList* CommandLists[] = { CommandList.Get() };
			CommandQueue->ExecuteCommandLists(sizeof(CommandLists) / sizeof(ID3D12CommandList*), CommandLists);
			FlushCommandQueue();
			GAMMA_INFO("D3D12", "Done initializing D3D12");
		}

		// Mainly based off Frank Luna's DX12 book
		void Context::FlushCommandQueue(void) {
			CurrentFencePoint++;
			CommandQueue->Signal(Fence.Get(), CurrentFencePoint);
			// Thankfully we can use events so we don't have to put a while loop here
			if (Fence->GetCompletedValue() < CurrentFencePoint) {
				HANDLE FenceEvent = CreateEventEx(NULL, NULL, 0, EVENT_ALL_ACCESS);
				Fence->SetEventOnCompletion(CurrentFencePoint, FenceEvent);
				WaitForSingleObject(FenceEvent, INFINITE);
				//CloseHandle(FenceEvent);
			}
		}

		D3D12_CPU_DESCRIPTOR_HANDLE Context::CurrentBackBufferRTV(void) {
			D3D12_CPU_DESCRIPTOR_HANDLE BackBufferRTV;
			BackBufferRTV.ptr = RTVDescriptorHeap->GetCPUDescriptorHandleForHeapStart().ptr + (UINT64)CurrentBackBufferIndex * RTVDescriptorSize;
			return BackBufferRTV;
		}

		D3D12_CPU_DESCRIPTOR_HANDLE Context::CurrentFrontBufferRTV(void) {
			D3D12_CPU_DESCRIPTOR_HANDLE FrontBufferRTV;
			FrontBufferRTV.ptr = RTVDescriptorHeap->GetCPUDescriptorHandleForHeapStart().ptr + (UINT64)!CurrentBackBufferIndex * RTVDescriptorSize;
			return FrontBufferRTV;
		}

		D3D12_CPU_DESCRIPTOR_HANDLE Context::DepthStencilDSV(void) {
			return DSVDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
		}

		D3D12_RESOURCE_BARRIER Context::CreateResourceBarrierTransition(ID3D12Resource* resource, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after, UINT subresource, D3D12_RESOURCE_BARRIER_FLAGS flags) {
			D3D12_RESOURCE_BARRIER ResourceBarrier;
			ResourceBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			ResourceBarrier.Flags = flags;
			ResourceBarrier.Transition.pResource = resource;
			ResourceBarrier.Transition.StateBefore = before;
			ResourceBarrier.Transition.StateAfter = after;
			ResourceBarrier.Transition.Subresource = subresource;
			return ResourceBarrier;
		}

		void Gamma_Graphics_API_Init(void) {

		}

		void Gamma_Graphics_API_Quit(void) {

		}

	}
}