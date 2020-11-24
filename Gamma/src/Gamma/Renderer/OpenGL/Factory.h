#ifndef GAMMA_OPENGL_FACTORY_H
#define GAMMA_OPENGL_FACTORY_H

#include "../../Gamma.h"
#include "Buffer.h"
#include "VertexBuffer.h"
#include "Mesh.h"

namespace Gamma {

	namespace OpenGL {

		class GAMMA_API Factory {
		public:
			Buffer* CreateBuffer(Buffer::TargetType target, size_t bytes, void* data);
			VertexBuffer* CreateVertexBuffer(int32_t numVerticies, ScalarVertex* verticies);
			Mesh* CreateMesh(int32_t numVerticies, ScalarVertex* verticies);
		private:
			friend class Renderer;
			Renderer* Context;
		};

	}

}

#endif // !GAMMA_OPENGL_FACTORY_H
