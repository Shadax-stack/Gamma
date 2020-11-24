#ifndef GAMMA_OPENGL_VERTEX_BUFFER_H
#define GAMMA_OPENGL_VERTEX_BUFFER_H

#include "../../Gamma.h"
#include "../Vertex.h"
#include "Handle.h"

namespace Gamma {

	namespace OpenGL {

		class GAMMA_API VertexBuffer : public Handle {
		public:
			virtual ~VertexBuffer(void) override;
		private:
			friend class Renderer;
			friend class Factory;
			int32_t VertexCount;
		};

	}

}

#endif // !GAMMA_OPENGL_VERTEX_BUFFER_H
