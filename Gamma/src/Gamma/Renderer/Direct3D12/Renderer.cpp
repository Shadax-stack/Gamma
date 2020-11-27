#include "Renderer.h"

namespace Gamma {

	namespace Direct3D12 {

		Renderer::Renderer(Context* context) : CommandExecutor(context) {
			CommandExecutor->Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, __uuidof(ID3D12CommandAllocator), (void**)CommandAllocator.GetAddressOf());
			CommandExecutor->Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, CommandAllocator.Get(), NULL, __uuidof(ID3D12GraphicsCommandList), (void**)CommandList.GetAddressOf());
			CommandList->Close();
		}

		void Renderer::EndFrame(void) {
			CommandList->ResourceBarrier(1, &CommandExecutor->CreateResourceBarrierTransition(CommandExecutor->SwapChainBuffers[CommandExecutor->CurrentBackBufferIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));
			CommandList->Close();
			ID3D12CommandList* CommandLists[] = { CommandList.Get() };
			CommandExecutor->CommandQueue->ExecuteCommandLists(sizeof(CommandLists) / sizeof(ID3D12CommandList*), CommandLists);
			CommandExecutor->FlushCommandQueue();
			CommandAllocator->Reset();
			CommandExecutor->SwapChain->Present(0, 0);
			CommandExecutor->CurrentBackBufferIndex = (CommandExecutor->CurrentBackBufferIndex + 1) % GAMMA_D3D12_SWAP_CHAIN_BUFFER_COUNT;
		}

		void Renderer::NewFrame(void) {
			const static float Color[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
			CommandList->Reset(CommandAllocator.Get(), NULL);
			CommandList->RSSetViewports(1, &CommandExecutor->Viewport);
			CommandList->ResourceBarrier(1, &CommandExecutor->CreateResourceBarrierTransition(CommandExecutor->SwapChainBuffers[CommandExecutor->CurrentBackBufferIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));
			CommandList->ClearRenderTargetView(CommandExecutor->CurrentBackBufferRTV(), Color, 0, NULL);
			CommandList->ClearDepthStencilView(CommandExecutor->DepthStencilDSV(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, NULL);
			CommandList->OMSetRenderTargets(1, &CommandExecutor->CurrentBackBufferRTV(), TRUE, &CommandExecutor->DepthStencilDSV());
		}

	}

}