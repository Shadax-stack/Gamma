#include "Renderer.h"
#include "../../Core/Log.h"
#include <GL/glew.h>

namespace Gamma {

	namespace OpenGL {

		void DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
			GAMMA_CRITICAL("OpenGL error %u [ID %u] of severity %u at %u: %s", type, id, severity, source, message);
			psnip_trap();
		}

		void Renderer::CreateContext(Gamma::Window* window) {
			Context = SDL_GL_CreateContext(window->GetInternalWindow());
			GAMMA_ASSERT_CRITICAL(Context, "Error while creating the context: ", SDL_GetError());
			GAMMA_INFO("Created OpenGL context");
			SDL_GL_MakeCurrent(window->GetInternalWindow(), Context);
			Window = window;
			SDL_GL_SetSwapInterval(1);
			uint32_t glewInitResult = glewInit();
			GAMMA_ASSERT_CRITICAL(glewInitResult == GLEW_OK, "Unable to initailize GLEW");
			GAMMA_INFO("Initailized GLEW");
			Factory.Context = this;
			GAMMA_INFO("Initailized factory");
			// Set debug callback only on debug builds
			#ifdef GAMMA_DEBUG
			// Based on https://blog.nobel-joergensen.com/2013/02/17/debugging-opengl-part-2-using-gldebugmessagecallback/
			// Check whether debug callback is available 
			if (glDebugMessageCallback) {
				glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
				glDebugMessageCallback(DebugCallback, nullptr);
				// I do not know what UnusedIds does, but it was in the original implementation
				GLuint UnusedIds = 0;
				glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, &UnusedIds, true);
				GAMMA_INFO("Registered OpenGL debug callback");
			} else {
				GAMMA_WARN("It appears that the OpenGL context does not support the OpenGL 4.3 feature glDebugMessageCallback, please update your drivers or get a newer GPU. OpenGL debugging will not be available");
			}
			#endif
		}

		void Renderer::FreeContext(void) {
			SDL_GL_DeleteContext(Context);
		}

		void Renderer::SwapBuffers(void) {
			SDL_GL_SwapWindow(Window->GetInternalWindow());
		}

		void Renderer::Clear(const Krypton::Vector4f color, float depth, uint8_t stencil) {
			glClearBufferfv(GL_COLOR, 0, (const float*)&color);
			//glClearBufferfv(GL_DEPTH, 0, (const float*)&depth);
			//glClearBufferiv(GL_STENCIL, 0, (const int*)&stencil);
		}

		void Renderer::NewFrame(void) {
			Clear(Krypton::Vector4f(0.0f, 0.0f, 0.0f, 0.0f), 1.0f, 255);
		}

		void Renderer::EndFrame(void) {
			SwapBuffers();
		}

		Factory* Renderer::GetFactory(void) {
			return &Factory;
		}

		void Renderer::Draw(Mesh* mesh) {
			mesh->AddRef();
			glBindVertexArray(mesh->Identifier);
			glDrawArrays(GL_TRIANGLES, 0, mesh->VertexBuffer->VertexCount);
			mesh->Release();
		}

		void Gamma_Graphics_API_Init(void) {
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
			SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
			SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
			glewExperimental = GL_TRUE;
			GAMMA_INFO("OpenGL ready for initialization");
		}

		void Gamma_Graphics_API_Quit(void) {
			GAMMA_INFO("Terminated OpenGL");
		}


	}

}