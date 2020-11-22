#ifndef GAMMA_OPENGL_BUFFER_H
#define GAMMA_OPENGL_BUFFER_H

#include "../../Gamma.h"
#include "Handle.h"

namespace Gamma {

	namespace OpenGL {

		class GAMMA_API Buffer : protected Handle {
		public:

		private:
			uint32_t Target;
		};

	}

}

#endif // !GAMMA_OPENGL_BUFFER_H
