//---------------------------------------------------------------------------

#include <opengl_window/context.h>

//---------------------------------------------------------------------------

namespace asd
{
	static void set_pixel_format(HDC dc)
	{
		if(dc == nullptr) {
			throw context_creation_exception("Can't set pixel format, device context is null!");
		}

		PIXELFORMATDESCRIPTOR pfd;

		const int attribs[] = {
			WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
			WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
			WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
			WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
			WGL_COLOR_BITS_ARB, 32,
			WGL_DEPTH_BITS_ARB, 24,
			WGL_STENCIL_BITS_ARB, 8,
			0
		};

		int pixelFormat, numFormats;
		wglChoosePixelFormatARB(dc, attribs, NULL, 1, &pixelFormat, (UINT*)&numFormats);

		if(SetPixelFormat(dc, pixelFormat, &pfd) == FALSE) {
			throw context_creation_exception("Can't set pixel format! Call to SetPixelFormat failed");
		}
	}

	window_context<opengl::driver>::~window_context() {
		wglMakeCurrent(nullptr, nullptr);

		if(_context != nullptr) {
			wglDeleteContext(_context);
		}
	}

	void window_context<opengl::driver>::flush() {
		SwapBuffers(_window.device());
	}

	void window_context<opengl::driver>::prepare() {
		set_pixel_format(_window.device());

		int flags = _driver.config.flags;

#ifdef GL_DEBUG

		flags |= WGL_CONTEXT_DEBUG_BIT_ARB;

#endif

		int attribs[] = {
			WGL_CONTEXT_MAJOR_VERSION_ARB, _driver.config.major,
			WGL_CONTEXT_MINOR_VERSION_ARB, _driver.config.minor,
			WGL_CONTEXT_FLAGS_ARB,         flags,
			WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
			0
		};

		_context = wglCreateContextAttribsARB(_window.device(), nullptr, attribs);

		if(!_context) {
			throw context_creation_exception("Can't create OpenGL render context!");
		}

		if(wglMakeCurrent(_window.device(), _context) == GL_FALSE) {
			throw context_creation_exception("Can't bind OpenGL render context!");
		}

		check_for_errors();

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_CULL_FACE);
		glEnable(GL_SCISSOR_TEST);

		glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
		//glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

#ifdef GL_DEBUG

		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

		if(glDebugMessageCallback != nullptr) {
			glDebugMessageCallback(opengl::glDebugCallbackFunc, nullptr);
		} else {
			std::cout << "glDebugMessageCallback is not supported" << std::endl;
		}

#endif

		check_for_errors();

		// _driver.set_modifiers(*this);
	}

	void window_context<opengl::driver>::init_device() {
		auto _wnd = window::create_empty_handle();
		auto _device = ::GetDC(_wnd);

		PIXELFORMATDESCRIPTOR pfd;
		memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));

		pfd.nSize      = sizeof(PIXELFORMATDESCRIPTOR);
		pfd.nVersion   = 1;
		pfd.dwFlags    = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 32;
		pfd.cDepthBits = 32;
		pfd.iLayerType = PFD_MAIN_PLANE;

		int pixelFormat = ChoosePixelFormat(_device, &pfd);

		if(pixelFormat == 0) {
			throw context_creation_exception("Can't choose pixel format!");
		}

		if(SetPixelFormat(_device, pixelFormat, &pfd) == FALSE) {
			throw context_creation_exception("Can't set pixel format! Call to SetPixelFormat failed");
		}

		auto _tmpContext = wglCreateContext(_device);

		if(_tmpContext == nullptr || wglMakeCurrent(_device, _tmpContext) == GL_FALSE) {
			throw context_creation_exception("Can't create initial OpenGL render context!");
		}

		glewExperimental = GL_TRUE;
		GLenum error = glewInit();

		if(error > 0) {
			throw context_creation_exception("Can't initialize GLEW! ", (const char *)glewGetErrorString(error));
		}

		wglMakeCurrent(nullptr, nullptr);
		wglDeleteContext(_tmpContext);
		DestroyWindow(_wnd);
	}

	window_context<opengl::driver>::window_context(opengl::driver & driver, window & w) : base(driver), basic_window_context(w) {
		init_device();

		auto update_size = [](const math::uint_size & size) {
			glViewport(0, 0, size.x, size.y);
			glScissor(0, 0, size.x, size.y);
		};

		w.inputs.size.subscribe(update_size);
		update_size(w.size());
	}
}

//---------------------------------------------------------------------------
