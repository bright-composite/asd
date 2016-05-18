//---------------------------------------------------------------------------

#include <opengl/OpenGL3_3.h>

#include <ui/UISpace.h>

//---------------------------------------------------------------------------

namespace Rapture
{
	namespace OpenGL3_3
	{
		Graphics3D::Graphics3D()
		{
			initDevice();
			initFacilities();

			_wnd = createEmptyWindowHandle();
			_device = GetDC(_wnd);
			_handle = wglCreateContext(_device);
		}

		Graphics3D::~Graphics3D()
		{
			if(_device != nullptr)
			{
				ReleaseDC(_wnd, _device);
				DeleteDC(_device);
			}

			if(_wnd != nullptr)
				DestroyWindow(_wnd);

			freeFacilities();
		}
	}
}
