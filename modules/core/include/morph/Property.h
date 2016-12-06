//---------------------------------------------------------------------------

#pragma once

#ifndef PROPERTY_H
#define PROPERTY_H

//---------------------------------------------------------------------------

#include "Property.hpp"

//---------------------------------------------------------------------------

#include "Morph.h"

//---------------------------------------------------------------------------

namespace asd
{
	using Property = Shared;

	template<class Origin>
	using PropertiesSet = Morph<Origin>;
}

//---------------------------------------------------------------------------
#endif