#ifndef GAMMA_OPENGL_BUFFER_H
#define GAMMA_OPENGL_BUFFER_H

#include "../../Gamma.h"
#include "Handle.h"
#include <GL/glew.h>

namespace Gamma {

	namespace OpenGL {

		class GAMMA_API Buffer : protected Handle {
		public:

			enum class TargetType {
				ARRAY = GL_ARRAY_BUFFER, VERTEX = ARRAY,
				ELEMENT = GL_ELEMENT_ARRAY_BUFFER, INDEX = ELEMENT,
			};
		private:
			TargetType Target;
			friend class Factory;
		};

	}

}

#endif // !GAMMA_OPENGL_BUFFER_H
