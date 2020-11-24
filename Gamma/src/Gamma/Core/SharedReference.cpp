#include "SharedReference.h"

namespace Gamma {

	Reference16::Reference16(void) : ReferenceCount(1) {}

	void Reference16::AddRef(void) {
		ReferenceCount++;
	}

	void Reference16::Release(void) {
		if (--ReferenceCount == 0) {
			delete this;
		}
	}

	uint16_t Reference16::GetRefCount(void) {
		return ReferenceCount;
	}

	Reference16MT::Reference16MT(void) : ReferenceCount(1) {}

	void Reference16MT::AddRef(void) {
		ReferenceCount++;
	}

	void Reference16MT::Release(void) {
		if (--ReferenceCount == 0) {
			delete this;
		}
	}

	uint16_t Reference16MT::GetRefCount(void) {
		return ReferenceCount;
	}

	Reference32::Reference32(void) : ReferenceCount(1) {}

	void Reference32::AddRef(void) {
		ReferenceCount++;
	}

	void Reference32::Release(void) {
		if (--ReferenceCount == 0) {
			delete this;
		}
	}

	uint32_t Reference32::GetRefCount(void) {
		return ReferenceCount;
	}

	Reference32MT::Reference32MT(void) : ReferenceCount(1) {}

	void Reference32MT::AddRef(void) {
		ReferenceCount++;
	}

	void Reference32MT::Release(void) {
		if (--ReferenceCount == 0) {
			delete this;
		}
	}

	uint32_t Reference32MT::GetRefCount(void) {
		return ReferenceCount;
	}

}