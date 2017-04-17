//---------------------------------------------------------------------------

#pragma once

#ifndef META_MACRO_H
#define META_MACRO_H

//---------------------------------------------------------------------------

#include <cstddef>
#include <utility>
#include <algorithm>
#include "Preprocessor.h"
#include "Flag.h"

#if defined(__x86_64) || defined(_WIN64)
#define ARCH_X64
#else
#define ARCH_X86
#endif // defined

#define api(module) asd_##module##_api

#ifndef __min
    #define __min(x, y) (((x) < (y)) ? (x) : (y))
#endif

#ifndef __max
    #define __max(x, y) (((x) > (y)) ? (x) : (y))
#endif

#ifdef _MSC_VER
#define forceinline __forceinline
#else
#define forceinline inline
#define __vectorcall
#define __thiscall
#endif

#ifdef WIN32
#define asd_import_api __declspec(dllimport)
#define asd_export_api __declspec(dllexport)
#else
#define asd_import_api
#define asd_export_api
#endif

#define apistruct(module) template struct api(module)
#define apiclass(module)  template class  api(module)

namespace asd
{
    using std::forward;
    using std::move;
	using std::pair;
	using std::initializer_list;

	template<class T, class Y>
	constexpr int compare(const T & x, const Y & y)
	{
		return x > y ? 1 : x < y ? -1 : 0;
	}

	template<class T, class Y>
	constexpr int icomp(const T & x, const Y & y)
	{
		return x >= y ? 1 : -1;
	}

	template<class T>
	constexpr bool between(T value, T min, T max)
	{
		return value >= min && value < max;
	}

	template<class X, class A>
	constexpr auto align(X x, A a)
	{
		return ((x - 1) | (a - 1)) + 1;
	}

	template<class X, class Y, class A>
	constexpr auto aligned_add(X x, Y y, A a)
	{
		x = x + y + a - 1;
		x = x - x % a;

		return x;
	}

	template<class T>
	size_t max_index(initializer_list<T> list)
	{
		return std::distance(list.begin(), std::max_element(list.begin(), list.end()));
	}

	template<class T>
	size_t min_index(initializer_list<T> list)
	{
		return std::distance(list.begin(), std::min_element(list.begin(), list.end()));
	}

	template<class T>
	pair<size_t, size_t> minmax_index(initializer_list<T> list)
	{
		auto minmax = std::minmax_element(list.begin(), list.end());
		return {std::distance(list.begin(), minmax.first), std::distance(list.begin(), minmax.second)};
	}

	template<class T, class F, class ... A, class = decltype(declval<const T>() != nullptr)>
	auto acquire(const T & object, F functor, A &&... args) -> decltype(functor(object, forward<A>(args)...))
	{
		return object != nullptr ? functor(object, forward<A>(args)...) : 0;
	}

	template<class T, class F, class ... A, class = decltype(declval<T>() != nullptr)>
	auto acquire(T && object, F functor, A &&... args) -> decltype(functor(forward<T>(object), forward<A>(args)...))
	{
		return object != nullptr ? functor(forward<T>(object), forward<A>(args)...) : 0;
	}

	template<class T, class F, class ... A>
	auto acquire(T * object, F functor, A &&... args) -> decltype(functor(object, forward<A>(args)...))
	{
		return object != nullptr ? functor(object, forward<A>(args)...) : 0;
	}

	template<class T>
	void * void_ptr(T * ptr)
	{
		return reinterpret_cast<void *>(ptr);
	}

	template<class T>
	const void * void_ptr(const T * ptr)
	{
		return reinterpret_cast<const void *>(ptr);
	}

	template<class T>
	void * void_ptr(T & val)
	{
		return reinterpret_cast<void *>(&val);
	}

	template<class T>
	const void * void_ptr(const T & val)
	{
		return reinterpret_cast<const void *>(&val);
	}

	template<class T>
	T * null()
	{
		return static_cast<T *>(nullptr);
	}

	template<class T>
	constexpr size_t addr(const T & val)
	{
		return reinterpret_cast<size_t>(void_ptr(val));
	}

	template<class T>
	constexpr size_t addr(const T * ptr)
	{
		return reinterpret_cast<size_t>(void_ptr(ptr));
	}

	template<int offset, typename I>
	constexpr auto get_bit(I value)
	{
		return (value >> offset) & 0x1;
	}

	template<typename I>
	constexpr auto get_bit(I value, const int offset)
	{
		return (value >> offset) & 0x1;
	}

	template<typename T, typename ... A>
	T * construct(void * place, A &&... args)
	{
		return new (place) T(forward<A>(args)...);
	}
}

/**
 *	Use it to implement declarations of class-members in the faster way
 */
#define implement(x, ...) decltype(x) x##__VA_ARGS__;

 /**
  *	Use it to wrap template parameters and prevent incorrect preprocessing
  */
#define macrowrap(...) __VA_ARGS__

/**
 *	Short for-loop
 */
#define repeat(var, times) for(remove_cv_t<decltype(times)> var = 0; var < times; ++var)

#define inscope()

#define deny_copy(... /* Class */) \
	__VA_ARGS__(const __VA_ARGS__ &) = delete; \
	__VA_ARGS__ & operator = (const __VA_ARGS__ &) = delete

#define member_cast(member, ... /* type */)		\
	operator __VA_ARGS__ & () &					\
	{											\
		return member;							\
	}											\
												\
	operator const __VA_ARGS__ & () const &		\
	{											\
		return member;							\
	}											\
												\
	operator __VA_ARGS__ && () &&				\
	{											\
		return move(member);					\
	}											\

//---------------------------------------------------------------------------
#endif
