//---------------------------------------------------------------------------

#pragma once

#ifndef FRUSTUM_H
#define FRUSTUM_H

//---------------------------------------------------------------------------

#include "Plane.h"

//---------------------------------------------------------------------------

namespace Rapture
{
	template<class T>
	class Frustum
	{
	public:
		Frustum(const Matrix<T> & vp)
		{
			auto m = vp.transposition();
			auto & p = *_planes;

			for(int i = 0; i < 3; ++i)
			{
				(p[i * 2 + 0] = m[i] + m[3]).normalize();
				(p[i * 2 + 1] = m[i] - m[3]).normalize();
			}
		}

	protected:
		AlignedArray<Plane<T>, 6> _planes[6]; // left, right, bottom, top, near, far
	};
}

//---------------------------------------------------------------------------
#endif