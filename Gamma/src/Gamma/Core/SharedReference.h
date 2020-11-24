#ifndef GAMMA_SHARED_REFERENCE_H
#define GAMMA_SHARED_REFERENCE_H

#include "../Gamma.h"
#include <stdint.h>
#include <atomic>

namespace Gamma {

	// Base on IUnknown
	class GAMMA_API Reference16 {
	public:
		Reference16(void);
		virtual ~Reference16(void) = default;
		void AddRef(void);
		void Release(void);
		uint16_t GetRefCount(void);
	protected:
		uint16_t ReferenceCount;
	};

	class GAMMA_API Reference16MT {
	public:
		Reference16MT(void);
		virtual ~Reference16MT(void) = default;
		void AddRef(void);
		void Release(void);
		uint16_t GetRefCount(void);
	protected:
		std::atomic<uint16_t> ReferenceCount;
	};

	class GAMMA_API Reference32 {
	public:
		Reference32(void);
		virtual ~Reference32(void) = default;
		void AddRef(void);
		void Release(void);
		uint32_t GetRefCount(void);
	protected:
		uint32_t ReferenceCount;
	};

	class GAMMA_API Reference32MT {
	public:
		Reference32MT(void);
		virtual ~Reference32MT(void) = default;
		void AddRef(void);
		void Release(void);
		uint32_t GetRefCount(void);
	protected:
		std::atomic<uint32_t> ReferenceCount;
	};

	typedef Reference16 Reference;

}

#endif
