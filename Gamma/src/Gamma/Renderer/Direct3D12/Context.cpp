#include "Context.h"
#include "../../Core/Log.h"
#include "../../Win32/Direct3DUtils.h"
#include <iostream>

#ifdef GAMMA_MSVC
#pragma warning(disable : 26495)
#pragma warning(disable : 26812)
#endif

namespace Gamma {
	namespace Direct3D12 {

		Context::Context(void) : CurrentFencePoint(0), CurrentBackBufferIndex(0), RTVDescriptorSize(0), DSVDescriptorSize(0), CBV_SRV_UAVDescriptorSize(0) {}

		Context::Context(Window* window) : Context() {
			Initialize(window);
		}

		Context::~Context(void) {
			FlushCommandQueue();
		}

		void Context::Initialize(Window* window, uint32_t msaa_samples) {
			CreateContext(window, msaa_samples);
		}

		void Context::CreateContext(Window* window, UINT msaa_samples) {
			HRESULT Result;
			#ifdef GAMMA_DEBUG
			Microsoft::WRL::ComPtr<ID3D12Debug> DebugController;
			Result = D3D12GetDebugInterface(__uuidof(ID3D12Debug), (void**)DebugController.GetAddressOf());
			GAMMA_ASSERT_ERROR(Result == S_OK, "D3D12", "Unable to create a ID3D12Debug, D3D12GetDebugInterface returned %#010x", Result);
			GAMMA_INFO("D3D12", "Created a ID3D12Debug");
			DebugController->EnableDebugLayer();
			#endif
			Microsoft::WRL::ComPtr<IDXGIFactory4> DXGIFactory;
			Result = CreateDXGIFactory1(__uuidof(IDXGIFactory4), (void**)DXGIFactory.GetAddressOf());
			GAMMA_ASSERT_CRITICAL(Result == S_OK, "D3D12", "Unable to create a IDXGIFactory4, CreateDXGIFactory1 returned %#010x", Result);
			GAMMA_INFO("D3D12", "Created a IDXGIFactory4");
			// Create the ID3D12Device using the first adapter in the system
			Result = D3D12CreateDevice(NULL, D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), (void**)Device.GetAddressOf());
			GAMMA_ASSERT_CRITICAL(Result == S_OK, "D3D12", "Unable to create a ID3D12Device, D3D12CreateDevice returned %#010x", Result);
			GAMMA_INFO("D3D12", "Created a ID3D12Device");
			D3D12_COMMAND_QUEUE_DESC CommandQueueDesc = {};
			CommandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
			Result = Device->CreateCommandQueue(&CommandQueueDesc, __uuidof(ID3D12CommandQueue), (void**)CommandQueue.GetAddressOf());
			GAMMA_ASSERT_CRITICAL(Result == S_OK, "D3D12", "Unable to create a ID3D12CommandQueue, ID3D12Device::CreateCommandQueue returned %#010x", Result);
			GAMMA_INFO("D3D12", "Created a ID3D12CommandQueue");
			Result = Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, __uuidof(ID3D12CommandAllocator), (void**)CommandAllocator.GetAddressOf());
			GAMMA_ASSERT_CRITICAL(Result == S_OK, "D3D12", "Unable to create a ID3D12CommandAllocator, ID3D12Device::CreateCommandAllocator returned %#010x", Result);
			GAMMA_INFO("D3D12", "Created a ID3D12CommandAllocator");
			Result = Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, CommandAllocator.Get(), NULL, __uuidof(ID3D12GraphicsCommandList), (void**)CommandList.GetAddressOf());
			GAMMA_ASSERT_CRITICAL(Result == S_OK, "D3D12", "Unable to create a ID3D12GraphicsCommandList, ID3D12Device::CreateCommandList returned %#010x", Result);
			GAMMA_INFO("D3D12", "Created a ID3D12GraphicsCommandList");
			Result = Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, __uuidof(ID3D12Fence), (void**)Fence.GetAddressOf());
			GAMMA_ASSERT_CRITICAL(Result == S_OK, "D3D12", "Unable to create a ID3D12Fence, ID3D12Device::CreateFence returned %#010x", Result);
			GAMMA_INFO("D3D12", "Created a ID3D12Fence");
			RTVDescriptorSize = Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
			DSVDescriptorSize = Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
			CBV_SRV_UAVDescriptorSize = Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
			D3D12_DESCRIPTOR_HEAP_DESC RTVDescriptorHeapDesc = {};
			RTVDescriptorHeapDesc.NumDescriptors = GAMMA_D3D12_SWAP_CHAIN_BUFFER_COUNT;
			RTVDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
			RTVDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			RTVDescriptorHeapDesc.NodeMask = 0;
			Device->CreateDescriptorHeap(&RTVDescriptorHeapDesc, __uuidof(ID3D12DescriptorHeap), (void**)RTVDescriptorHeap.GetAddressOf());
			GAMMA_ASSERT_CRITICAL(Result == S_OK, "D3D12", "Unable to create a RTV ID3D12DescriptorHeap, ID3D12Device::CreateDescriptorHeap returned %#010x", Result);
			GAMMA_INFO("D3D12", "Created a RTV ID3D12DescriptorHeap");
			D3D12_DESCRIPTOR_HEAP_DESC DSVDescriptorHeapDesc = {};
			DSVDescriptorHeapDesc.NumDescriptors = 1;
			DSVDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
			DSVDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			DSVDescriptorHeapDesc.NodeMask = 0;
			Result = Device->CreateDescriptorHeap(&DSVDescriptorHeapDesc, __uuidof(ID3D12DescriptorHeap), (void**)DSVDescriptorHeap.GetAddressOf());
			GAMMA_ASSERT_CRITICAL(Result == S_OK, "D3D12", "Unable to create a DSV ID3D12DescriptorHeap, ID3D12Device::CreateDescriptorHeap returned %#010x", Result);
			GAMMA_INFO("D3D12", "Created a DSV ID3D12DescriptorHeap");
			D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS MultisampleQualityLevels;
			MultisampleQualityLevels.SampleCount = msaa_samples;
			MultisampleQualityLevels.NumQualityLevels = 0;
			MultisampleQualityLevels.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			MultisampleQualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
			Result = Device->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, (void*)&MultisampleQualityLevels, sizeof(D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS));
			GAMMA_ASSERT_ERROR(Result == S_OK, "D3D12", "MSSA %ux is not supported, ID3D12Device::CheckFeatureSupport returned %#010x", msaa_samples, Result);
			GAMMA_INFO("D3D12", "MSSA %ux is supported", msaa_samples);
			DXGI_SWAP_CHAIN_DESC SwapChainDesc = {}; 
			SwapChainDesc.BufferDesc.Width = window->GetDimensions().Width;
			SwapChainDesc.BufferDesc.Height = window->GetDimensions().Height;
			SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			SwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
			SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
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
			Result = DXGIFactory->CreateSwapChain(CommandQueue.Get(), &SwapChainDesc, SwapChain.GetAddressOf());
			GAMMA_ASSERT_CRITICAL(Result == S_OK, "D3D12", "Unable to create a IDXGISwapChain, IDXGIFactory::CreateSwapChain returned %#010x", Result);
			GAMMA_INFO("D3D12", "Created a IDXGISwapChain");
			// Create color buffers and RTVs
			D3D12_CPU_DESCRIPTOR_HANDLE RTVHeapBase = RTVDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
			for (UINT index = 0; index < GAMMA_D3D12_SWAP_CHAIN_BUFFER_COUNT; index++) {
				Result = SwapChain->GetBuffer(index, __uuidof(ID3D12Resource), (void**)SwapChainBuffers[index].GetAddressOf());
				GAMMA_ASSERT_CRITICAL(Result == S_OK, "D3D12", "Unable to create ID3D12Resource[%u], IDXGISwapChain::GetBuffer returned %#010x", index, Result);
				GAMMA_INFO("D3D12", "Created ID3D12Resource[%u]", index);
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
			Result = Device->CreateCommittedResource(&DepthStencilHeapProperties, D3D12_HEAP_FLAG_NONE, &DepthStencilDesc, D3D12_RESOURCE_STATE_COMMON, &DepthStencilClearValue, __uuidof(ID3D12Resource), (void**)DepthStencilBuffer.GetAddressOf());
			GAMMA_ASSERT_CRITICAL(Result == S_OK, "D3D12", "Unable to create a ID3D12Resource, ID3D12Device::CreateCommittedResource returned %#010x",  Result);
			GAMMA_INFO("D3D12", "Created a ID3D12Resource");
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
			GAMMA_INFO("D3D12", "Done initializing D3D12");
		}

		// Mainly based off Frank Luna's DX12 book
		void Context::FlushCommandQueue(void) {
			HRESULT Result;
			CurrentFencePoint++;
			Result = CommandQueue->Signal(Fence.Get(), CurrentFencePoint);
			// Thankfully we can use events so we don't have to put a while loop here
			if (Fence->GetCompletedValue() < CurrentFencePoint) {
				HANDLE FenceEvent = CreateEventEx(NULL, NULL, 0, EVENT_ALL_ACCESS);
				// Then we may fall back to brute force methods if we were not able to create a handle
				GAMMA_ASSERT_CRITICAL(FenceEvent != NULL, "D3D12", "CreateEventEx returned NULL. The program will not be able to wait for the fence event. Last error on this thread: %#010x", GetLastError());
				Fence->SetEventOnCompletion(CurrentFencePoint, FenceEvent);
				DWORD FenceResult = WaitForSingleObject(FenceEvent, 10000); // Wait ten seconds
				if (FenceResult != WAIT_OBJECT_0) {
					// Something is very wrong if the GPU has not flushed the queue within 10 seconds
					GAMMA_CRITICAL("D3D12", "The fence event did not become signaled after 10 seconds, WaitForSingleObject returned %#010x", FenceResult);
					psnip_trap();
				}
				CloseHandle(FenceEvent);
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

		void Context::EndFrame(void) {
			CommandList->ResourceBarrier(1, &CreateResourceBarrierTransition(SwapChainBuffers[CurrentBackBufferIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));
			CommandList->Close();
			ID3D12CommandList* CommandLists[] = { CommandList.Get() };
			CommandQueue->ExecuteCommandLists(sizeof(CommandLists) / sizeof(ID3D12CommandList*), CommandLists);
			FlushCommandQueue();
			CommandAllocator->Reset();
			SwapChain->Present(0, 0);
			CurrentBackBufferIndex = (CurrentBackBufferIndex+1) % GAMMA_D3D12_SWAP_CHAIN_BUFFER_COUNT;
		}

		void Context::NewFrame(void) {
			const static float Black[4] = { 0.0f };
			CommandList->Reset(CommandAllocator.Get(), NULL);
			CommandList->ResourceBarrier(1, &CreateResourceBarrierTransition(SwapChainBuffers[CurrentBackBufferIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));
			CommandList->RSSetViewports(1, &Viewport);
			CommandList->ClearRenderTargetView(CurrentBackBufferRTV(), Black, 0, NULL);
			CommandList->ClearDepthStencilView(DepthStencilDSV(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, NULL);
			CommandList->OMSetRenderTargets(1, &CurrentBackBufferRTV(), TRUE, &DepthStencilDSV());
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