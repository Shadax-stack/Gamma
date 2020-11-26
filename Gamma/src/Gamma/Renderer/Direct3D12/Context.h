#ifndef GAMMA_DIRECT3D12_CONTEXT_H
#define GAMMA_DIRECT3D12_CONTEXT_H

#include "../../Gamma.h"
#include "../../Window/Window.h"
#include <d3d12.h>
#include <dxgi.h>
#include <dxgi1_4.h>
#include <wrl.h>

namespace Gamma { 
	namespace Direct3D12 {

#define GAMMA_D3D12_SWAP_CHAIN_BUFFER_COUNT 2

		class GAMMA_API Context {
		public:
			Context(void);
			Context(Window* window);
			~Context(void);
			void Initialize(Window* window, uint32_t msaa_samples = 1);
			void EndFrame(void);
			void NewFrame(void);
		private:
			void CreateContext(Window* window, UINT msaa_samples);
			D3D12_CPU_DESCRIPTOR_HANDLE CurrentFrontBufferRTV(void);
			D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferRTV(void);
			D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilDSV(void);
			D3D12_RESOURCE_BARRIER CreateResourceBarrierTransition(ID3D12Resource* resource, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after, UINT subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES, D3D12_RESOURCE_BARRIER_FLAGS flags = D3D12_RESOURCE_BARRIER_FLAG_NONE);
			void FlushCommandQueue(void);
			Microsoft::WRL::ComPtr<ID3D12Device> Device;
			Microsoft::WRL::ComPtr<ID3D12CommandQueue> CommandQueue;
			Microsoft::WRL::ComPtr<ID3D12CommandAllocator> CommandAllocator;
			Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> CommandList;
			Microsoft::WRL::ComPtr<ID3D12Fence> Fence; 
			UINT64 CurrentFencePoint;
			Microsoft::WRL::ComPtr<IDXGIAdapter> Adapter;
			Microsoft::WRL::ComPtr<IDXGIOutput> Output;
			UINT RTVDescriptorSize;
			UINT DSVDescriptorSize;
			UINT CBV_SRV_UAVDescriptorSize;
			Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> RTVDescriptorHeap;
			Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> DSVDescriptorHeap;
			Microsoft::WRL::ComPtr<IDXGISwapChain> SwapChain; 
			UINT CurrentBackBufferIndex;
			Microsoft::WRL::ComPtr<ID3D12Resource> SwapChainBuffers[GAMMA_D3D12_SWAP_CHAIN_BUFFER_COUNT];
			Microsoft::WRL::ComPtr<ID3D12Resource> DepthStencilBuffer;
			D3D12_VIEWPORT Viewport;
			Window* SwapChainWindow;
		};

	} 
}

#endif // !GAMMA_DIRECT3D12_CONTEXT_H
