#ifndef GAMMA_OPENGL_MESH_H
#define GAMMA_OPENGL_MESH_H

#include "../../Gamma.h"
#include "Handle.h"
#include "VertexBuffer.h"

namespace Gamma {

	namespace OpenGL {

		class Mesh : public Handle {
		public:
			virtual ~Mesh(void) override;
		private:
			VertexBuffer* VertexBuffer;
			friend class Factory;
			friend class Renderer;
		};

	}

}

#endif // !GAMMA_OPENGL_MESH_H

