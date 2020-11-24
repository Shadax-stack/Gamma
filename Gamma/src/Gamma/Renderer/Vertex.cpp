#include "Vertex.h"

namespace Gamma {

	void ScalarVec2f::operator=(Krypton::Vector2f vec) {
		this->x = vec.x;
		this->y = vec.y;
	}

	ScalarVec2f::ScalarVec2f(void) : x(0.0f), y(0.0f) {}
	ScalarVec2f::ScalarVec2f(float x, float y) : x(x), y(y) {}

	void ScalarVec3f::operator=(Krypton::Vector3f vec) {
		this->x = vec.x;
		this->y = vec.y;
		this->z = vec.z;
	}

	ScalarVec3f::ScalarVec3f(void) : x(0.0f), y(0.0f), z(0.0f) {}
	ScalarVec3f::ScalarVec3f(float x, float y, float z) : x(x), y(y), z(z) {}

	ScalarVertex Vertex::CreateScalarVertex(void) {
		ScalarVertex vertex;
		// MSVC might create a C6001 warning here, do not worry about it
		vertex.Position = this->Position;
		return vertex;
	}

}