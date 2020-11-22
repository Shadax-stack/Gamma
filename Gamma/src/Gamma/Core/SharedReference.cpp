#include "SharedReference.h"

namespace Gamma {

	GammaUnknown16::GammaUnknown16(void) : ReferenceCount(1) {}

	void GammaUnknown16::AddRef(void) {
		ReferenceCount++;
	}

	void GammaUnknown16::Release(void) {
		if (--ReferenceCount == 0) {
			delete this;
		}
	}

	uint16_t GammaUnknown16::GetRefCount(void) {
		return ReferenceCount;
	}

	GammaUnknown16MT::GammaUnknown16MT(void) : ReferenceCount(1) {}

	void GammaUnknown16MT::AddRef(void) {
		ReferenceCount++;
	}

	void GammaUnknown16MT::Release(void) {
		if (--ReferenceCount == 0) {
			delete this;
		}
	}

	uint16_t GammaUnknown16MT::GetRefCount(void) {
		return ReferenceCount;
	}

	GammaUnknown32::GammaUnknown32(void) : ReferenceCount(1) {}

	void GammaUnknown32::AddRef(void) {
		ReferenceCount++;
	}

	void GammaUnknown32::Release(void) {
		if (--ReferenceCount == 0) {
			delete this;
		}
	}

	uint32_t GammaUnknown32::GetRefCount(void) {
		return ReferenceCount;
	}

	GammaUnknown32MT::GammaUnknown32MT(void) : ReferenceCount(1) {}

	void GammaUnknown32MT::AddRef(void) {
		ReferenceCount++;
	}

	void GammaUnknown32MT::Release(void) {
		if (--ReferenceCount == 0) {
			delete this;
		}
	}

	uint32_t GammaUnknown32MT::GetRefCount(void) {
		return ReferenceCount;
	}

}