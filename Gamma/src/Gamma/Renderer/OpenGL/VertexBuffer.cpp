#include "VertexBuffer.h"
#include <GL/glew.h>

namespace Gamma {

	namespace OpenGL {

		VertexBuffer::~VertexBuffer(void) {
			glDeleteBuffers(1, &Identifier);
		}

	}

}