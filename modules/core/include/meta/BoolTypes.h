//---------------------------------------------------------------------------

#ifndef BOOL_TYPES_H
#define BOOL_TYPES_H

//---------------------------------------------------------------------------

#include "Sfinae.h"

//---------------------------------------------------------------------------

namespace Rapture
{
	template<bool ... V>
	struct is_true : false_type {};

	template<bool ... V>
	struct is_true<true, V...> : is_true<V...> {};

	template<bool ... V>
	struct is_true<false, V...> : false_type {};

	template<>
	struct is_true<> : true_type {};

	template<bool ... V>
	struct is_false : false_type {};

	template<bool ... V>
	struct is_false<false, V...> : true_type {};

	template<bool ... V>
	struct is_false<true, V...> : is_false<V...> {};

	template<>
	struct is_false<> : false_type {};

//---------------------------------------------------------------------------

	template<typename T, typename U>
	struct are_same : is_same<T, U> {};

	template<template<class...> class Tpl, typename ... T, typename ... U>
	struct are_same<Tpl<T...>, Tpl<U...>> : is_same<Tpl<decay_t<T>...>, Tpl<decay_t<U>...>> {};

	template<template<class...> class Tpl, typename T, typename ... U>
	struct are_same<T, Tpl<U...>> : is_same<Tpl<T>, Tpl<decay_t<U>...>> {};

	template<template<class...> class Tpl, typename U, typename ... T>
	struct are_same<Tpl<T...>, U> : is_same<Tpl<decay_t<T>...>, Tpl<U>> {};

	template<typename From, typename To>
	struct are_convertible : is_convertible<From, To> {};

	namespace Internals
	{
		template<typename T, typename U, bool>
		struct are_convertible : false_type {};

		template<template<class...> class Tpl, typename ... T, typename ... U>
		struct are_convertible<Tpl<T...>, Tpl<U...>, true> : is_true<is_convertible<T, U>::value...> {};
	}

	template<template<class...> class Tpl, typename ... T, typename ... U>
	struct are_convertible<Tpl<T...>, Tpl<U...>> : Internals::are_convertible<Tpl<T...>, Tpl<U...>, sizeof...(T) == sizeof...(U)> {};

	//---------------------------------------------------------------------------

#define declare_bool_struct(name, ...)							\
	struct name													\
	{															\
		static const bool value = is_true<__VA_ARGS__>::value;	\
	}

	/**
	*	@brief
	*		Used to select types we need from a template function
	*
	*  Usage:
	*		useif <same_type<target, type>::value> endif
	*		useif <based_on<target, base_type>::value> endif
	*		useif <same_types<tuple<target>, tuple<types>>::value> endif
	*/
	template<class T, class U>
	declare_bool_struct(same_type, is_same<decay_t<T>, decay_t<U>>::value);
	template<class T, class U>
	declare_bool_struct(based_on, is_base_of<decay_t<U>, decay_t<T>>::value);
	template<class T, class U>
	declare_bool_struct(same_types, are_same<decay_t<T>, decay_t<U>>::value);

		/**
		*	@brief
		*		These things are quitely opposite to same_type, based_on etc.
		*/
	template<class T, class U>
	declare_bool_struct(not_same_type, !is_same<decay_t<T>, decay_t<U>>::value);
	template<class T, class U>
	declare_bool_struct(not_based_on, !is_base_of<decay_t<U>, decay_t<T>>::value);
	template<class T, class U>
	declare_bool_struct(not_same_types, !are_same<decay_t<T>, decay_t<U>>::value);

//---------------------------------------------------------------------------

	/**
	 *	Used instead of std::is_constructible to allow "friendship"
	 */
	sfinae_checker(
		is_constructible, (class T, class ... A), (T, A...),
		decltype(T(declval<A>()...))
	);

	/**
	 *	Used instead of std::is_constructible to allow "friendship"
	 */
	sfinae_checker(
		is_list_constructible, (class T, class ... A), (T, A...),
		decltype(T {declval<A>()...})
	);

//---------------------------------------------------------------------------

	template<class T>
	declare_bool_struct(is_determined_class, !std::is_class<T>::value || std::is_base_of<T, T>::value);
	template<class T, class ... A>
	declare_bool_struct(can_construct, is_constructible<T, A...>::value || is_list_constructible<T, A...>::value);
	template<class T, class ... A>
	declare_bool_struct(cant_construct, !can_construct<T, A...>::value);

#define flag_class(name, checker)	\
	struct name {};					\
									\
	template<class T>				\
	declare_bool_struct(checker,	\
		is_base_of<name, T>::value	\
	)
}

//---------------------------------------------------------------------------
#endif