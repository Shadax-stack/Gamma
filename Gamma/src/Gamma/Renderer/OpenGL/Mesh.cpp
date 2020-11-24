#include "Mesh.h"
#include <GL/glew.h>

namespace Gamma {

	namespace OpenGL {

		Mesh::~Mesh(void) {
			glDeleteVertexArrays(1, &Identifier);
		}

	}

}