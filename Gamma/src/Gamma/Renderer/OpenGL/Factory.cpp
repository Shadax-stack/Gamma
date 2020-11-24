#include "Factory.h"

namespace Gamma {

	namespace OpenGL {

		// Objects are not unbound after being processed to improve speed

		Buffer* Factory::CreateBuffer(Buffer::TargetType target, size_t bytes, void* data) {
			Buffer* buffer = new Buffer;
			buffer->Target = target;
			glGenBuffers(1, &buffer->Identifier);
			glBindBuffer((GLenum)target, buffer->Identifier);
			glBufferData((GLenum)target, bytes, data, GL_STATIC_DRAW);
			glBindBuffer((GLenum)target, 0);
			return buffer;
		}

		VertexBuffer* Factory::CreateVertexBuffer(int32_t numVerticies, ScalarVertex* verticies) {
			// Create the buffer
			VertexBuffer* buffer = new VertexBuffer;
			// Reference for the Factory::CreateVertexBuffer function
			buffer->AddRef(); 
			// Set buffer size
			buffer->VertexCount = numVerticies;
			// Create the buffer's identifier
			glGenBuffers(1, &buffer->Identifier);
			// Bind it
			glBindBuffer(GL_ARRAY_BUFFER, buffer->Identifier);
			// Upload the data
			glBufferData(GL_ARRAY_BUFFER, numVerticies * sizeof(ScalarVertex), verticies, GL_STATIC_DRAW);
			// Done creating the buffer, release it
			buffer->Release(); 
			// Return the buffer
			return buffer;
		}

		Mesh* Factory::CreateMesh(int32_t numVerticies, ScalarVertex* verticies) {
			// Allocate the mesh
			Mesh* mesh = new Mesh;
			// Increment the refrence counter for this current scope
			mesh->AddRef();
			mesh->VertexBuffer = CreateVertexBuffer(numVerticies, verticies);
			// Create the mesh VAO
			glGenVertexArrays(1, &mesh->Identifier);
			// Bind the VAO
			glBindVertexArray(mesh->Identifier);
			// VBO is still bound from CreateVertexBuffer function call, no need to bind it again
			// Set the vertex attributes
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ScalarVertex), (const void*)offsetof(ScalarVertex, Position));
			glEnableVertexAttribArray(0);
			// Decrement the refrence counter while leaving this scope
			mesh->Release();
			// Return the mesh
			return mesh;
		}

	}

}