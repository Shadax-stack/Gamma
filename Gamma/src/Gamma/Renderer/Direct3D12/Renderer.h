#ifndef GAMMA_DIRECT3D12_RENDERER_H
#define GAMMA_DIRECT3D12_RENDERER_H

#include "Context.h"
#include <wrl.h>

namespace Gamma {

	namespace Direct3D12 {

		class GAMMA_API Renderer {
		public:
			Renderer(Context* context);
			void EndFrame(void);
			void NewFrame(void);
		private:
			Context* CommandExecutor;
			// The renderer will have it's own command allocator 
			Microsoft::WRL::ComPtr<ID3D12CommandAllocator> CommandAllocator;
			Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> CommandList;
		};

	}

}

#endif
