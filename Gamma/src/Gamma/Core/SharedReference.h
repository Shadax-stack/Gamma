#ifndef GAMMA_SHARED_REFERENCE_H
#define GAMMA_SHARED_REFERENCE_H

#include "../Gamma.h"
#include <stdint.h>
#include <atomic>

namespace Gamma {

	// Base on IUnknown
	class GAMMA_API GammaUnknown16 {
	public:
		GammaUnknown16(void);
		virtual ~GammaUnknown16(void) = default;
		void AddRef(void);
		void Release(void);
		uint16_t GetRefCount(void);
	protected:
		uint16_t ReferenceCount;
	};

	class GAMMA_API GammaUnknown16MT { 
	public:
		GammaUnknown16MT(void);
		virtual ~GammaUnknown16MT(void) = default;
		void AddRef(void);
		void Release(void);
		uint16_t GetRefCount(void);
	protected:
		std::atomic<uint16_t> ReferenceCount;
	};

	class GAMMA_API GammaUnknown32 {
	public:
		GammaUnknown32(void);
		virtual ~GammaUnknown32(void) = default;
		void AddRef(void);
		void Release(void);
		uint32_t GetRefCount(void);
	protected:
		uint32_t ReferenceCount;
	};

	class GAMMA_API GammaUnknown32MT {
	public:
		GammaUnknown32MT(void);
		virtual ~GammaUnknown32MT(void) = default;
		void AddRef(void);
		void Release(void);
		uint32_t GetRefCount(void);
	protected:
		std::atomic<uint32_t> ReferenceCount;
	};

	typedef GammaUnknown16 GammaUnknown;

}

#endif
