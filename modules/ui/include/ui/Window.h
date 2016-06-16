//---------------------------------------------------------------------------

#ifndef WINDOW_H
#define WINDOW_H

//---------------------------------------------------------------------------

#include "UISpace.h"

//---------------------------------------------------------------------------

namespace Rapture
{
	class Window;

	enum class WindowState
	{
		Normal,
		Hidden,
		Minimized,
		Maximized
	};

	enum class BorderStyle
	{
		Normal,
		Static,
		Fixed,
		None
	};

	link_class(ui, Window, Class<UISpace>);

	class Window : public UISpace
	{
	public:
		Window(Graphics * graphics, long left, long top, long width, long height, const WideString & caption = L"") : Window(graphics, IntRect {left, top, left + width, top + height}, caption) {}
		Window(Graphics * graphics, long width, long height, const WideString & caption = L"") : Window(graphics, IntSize {width, height}, caption) {}
		Window(Graphics * graphics, const IntSize & size, const WideString & caption = L"") : Window(graphics, IntRect {0, 0, size.x, size.y}, caption) {}
		api(ui) Window(Graphics * graphics, const IntRect & rect, const WideString & caption = L"");

		virtual ~Window() {}

		long outerWidth() const
		{
			return _outerRegion.width();
		}

		long outerHeight() const
		{
			return _outerRegion.height();
		}

		LongSize outerSize() const
		{
			return _outerRegion.size();
		}

		LongRect outerRegion() const
		{
			return _outerRegion;
		}

		api(ui) void centralize();

		virtual void api(ui) registerHotkey(int id, int key, int modifiers = 0) override;
		virtual void api(ui) unregisterHotkey(int id) override;

		bool isFullscreen() const
		{
			return _fullscreen;
		}

		void api(ui) setFullscreen(bool fullscreen);
		void api(ui) toggleFullscreen();

		void api(ui) setState(WindowState state);

		void api(ui) show();
		void api(ui) hide();
		void api(ui) minimize();
		void api(ui) maximize();
		void api(ui) restore();

		BorderStyle borderStyle() const
		{
			return _borderStyle;
		}

		void api(ui) setBorderStyle(BorderStyle style);

		bool isVisible() const
		{
			return _state != WindowState::Hidden;
		}

		WindowState state() const
		{
			return _state;
		}

		void api(ui) setCaption(const WideString & caption);
		WideString api(ui) getCaption();

		void api(ui) close();

	protected:
		void api(ui) makeFullscreen();
		void api(ui) restoreSize();
		void api(ui) applyBorderStyle();

		friend LRESULT api(ui) CALLBACK wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

		bool _isShown = false;

		DWORD _normalStyle;
		LongRect _outerRegion;

		WINDOWPLACEMENT _fullscreenPlacement = {0};
		WINDOWPLACEMENT _windowedPlacement = {0};

		UINT_PTR _timer;
		WindowState _state = WindowState::Hidden;
		BorderStyle _borderStyle = BorderStyle::Normal;

		bind_messages(Window, WindowMessages);
	};

	channels_api(ui, Window, WindowMessages)

	inline bool IsKeyPressed(int v_key)
	{
		return hi_bit_mask::state(GetKeyState(v_key));
	}
}

//---------------------------------------------------------------------------
#endif
