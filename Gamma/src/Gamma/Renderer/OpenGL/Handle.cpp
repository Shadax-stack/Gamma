#include "Handle.h"

namespace Gamma {

	namespace OpenGL {

		Handle::Handle(void) : Identifier(0) {}

		uint32_t Handle::GetIdentifier(void) {
			return Identifier;
		}

		void Handle::SetIdentifier(uint32_t newID) {
			Identifier = newID;
		}

	}

}