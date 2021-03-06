//---------------------------------------------------------------------------

#pragma once

#ifndef CAST_H
#define CAST_H

//---------------------------------------------------------------------------

#include "useif.h"
#include "types.h"

//---------------------------------------------------------------------------

namespace asd
{
	namespace cast
	{
		static inline void perform(empty, empty) {}
	}
	
	function_checker(has_cast_function, cast::perform);
	
	template<class From, class To>
	using can_cast = has_cast_function<To &, const From &>;
	
	namespace cast
	{
		template<class T, class Y, useif<std::is_convertible<Y &&, T>::value>, useif<(based_on<T, Y>::value || based_on<Y, T>::value)>>
		inline T as(Y && x) {
			return static_cast<T>(forward<Y>(x));
		}
		
		template<class T, class Y, useif<can_cast<Y, T>::value>>
		inline remove_reference_t <T> as(Y && x) {
			using Out = remove_reference_t<T>;
			
			Out out;
			perform(out, forward<Y>(x));
			return out;
		}
		
		template<class T, class Y, useif<std::is_convertible<Y &&, T &>::value>, useif<(based_on<T, Y>::value || based_on<Y, T>::value)>>
		inline void to(T & out, Y && x) {
			out = static_cast<T>(forward<Y>(x));
		}
		
		template<class T, class Y, useif<can_cast<Y, T>::value>>
		inline void to(T & out, Y && x) {
			perform(out, forward<Y>(x));
		}
		
		template<class T, class Y, useif<std::is_convertible<Y &&, T &>::value>, useif<(based_on<T, Y>::value || based_on<Y, T>::value)>>
		inline void to(T & out, const Y & x) {
			out = static_cast<T>(x);
		}
		
		template<class T, class Y, useif<can_cast<Y, T>::value>>
		inline void to(T & out, const Y & x) {
			perform(out, x);
		}
		
		template<class T, class Y, useif<
			std::is_pointer<T>::value || std::is_integral<T>::value,
			std::is_pointer<Y>::value && std::is_integral<Y>::value
		>>
		inline T address(Y x) {
			return reinterpret_cast<T>(x);
		}
		
		template<class T, class Y, useif<std::is_polymorphic<T>::value, std::is_polymorphic<Y>::value, (based_on<T, Y>::value || based_on<Y, T>::value)>>
		inline T dynamic(Y && x) {
			return dynamic_cast<T>(forward<Y>(x));
		}
	}
}

//---------------------------------------------------------------------------
#endif
