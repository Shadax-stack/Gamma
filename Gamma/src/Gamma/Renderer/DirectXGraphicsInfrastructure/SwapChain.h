#ifndef GAMMA_DIRECTXGRAPHICSINFRASTRUCTURE_H
#define GAMMA_DIRECTXGRAPHICSINFRASTRUCTURE_H

#include "../../Gamma.h"

#include <dxgi.h>
#ifdef GAMMA_GRAPHICS_API_DIRECT3D12
#include <dxgi1_4.h>
#endif
#include <wrl.h>

namespace Gamma {

	namespace DirectXGraphicsInfrastructure {

		class GAMMA_API SwapChain {
		public:
		private:

		};

	}

	namespace DXGI = DirectXGraphicsInfrastructure;

}

#endif
