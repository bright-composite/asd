//---------------------------------------------------------------------------

#ifndef WINDOW_H
#define WINDOW_H

//---------------------------------------------------------------------------

#if	defined(WIN32)
	#include "windows/Window.h"
#elif defined(unix) || defined(__unix)
	#include "x/Window.h"
#endif

//---------------------------------------------------------------------------
#endif