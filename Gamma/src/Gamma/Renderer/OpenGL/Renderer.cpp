#include "Renderer.h"
#include "../../Core/Log.h"
#include <GL/glew.h>

namespace Gamma {

	namespace OpenGL {

		void Gamma_OpenGL_Init(void) {
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
			SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
			SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
			glewExperimental = GL_TRUE;
		}

		void Renderer::CreateContext(Gamma::Window* window) {
			Context = SDL_GL_CreateContext(window->pWindow);
			GAMMA_ASSERT_CRITICAL(Context, "Error while creating the context: ", SDL_GetError());
			SDL_GL_MakeCurrent(window->pWindow, Context);
			Window = window;
			SDL_GL_SetSwapInterval(1);
			uint32_t glewInitResult = glewInit();
			GAMMA_ASSERT_CRITICAL(glewInitResult == GLEW_OK, "Unable to initailize GLEW");
		}

		void Renderer::FreeContext(void) {
			SDL_GL_DeleteContext(Context);
		}

		void Renderer::SwapBuffers(void) {
			SDL_GL_SwapWindow(Window->pWindow);
		}

		void Renderer::ClearColor(const Krypton::Vector4f color) {
			//glClearBufferfv(GL_COLOR, 0, (const float*)&color);
		}

		void Renderer::NewFrame(void) {
			Krypton::Vector4f defaultColor;
			defaultColor.r = 1.0f;
			defaultColor.g = 1.0f;
			defaultColor.b = 1.0f;
			defaultColor.a = 1.0f;
			ClearColor(defaultColor);
		}

		void Renderer::EndFrame(void) {
			SwapBuffers();
		}

	}

}