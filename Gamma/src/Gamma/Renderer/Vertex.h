#ifndef GAMMA_VERTEX_H
#define GAMMA_VERTEX_H

#include "../Gamma.h"
#include <Krypton.h>

namespace Gamma {

	// TODO: move scalar vectors to krypton
	// Scalar vectors are non-SIMD vectors designed for memory efficiency
	struct ScalarVec2f {
		union {
			struct {
				float x, y;
			};
			struct {
				float r, g;
			};
		};
		void operator=(Krypton::Vector2f vec);
		ScalarVec2f(void);
		ScalarVec2f(float x, float y);
	};

	struct ScalarVec3f {
		union {
			struct {
				float x, y, z;
			};
			struct {
				float r, g, b;
			};
		};
		void operator=(Krypton::Vector3f vec);
		ScalarVec3f(void);
		ScalarVec3f(float x, float y, float z);
	};

	struct GAMMA_API ScalarVertex {
		ScalarVec3f Position;
	};

	struct GAMMA_API Vertex {
	public:
		Krypton::Vector3f Position;
		ScalarVertex CreateScalarVertex(void);
	};

}

#endif // !GAMMA_VERTEX_H

