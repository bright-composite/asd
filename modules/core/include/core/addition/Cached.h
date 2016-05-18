//---------------------------------------------------------------------------

#ifndef CACHED_H
#define CACHED_H

//---------------------------------------------------------------------------

#include <core/container/Map.h>
#include <core/Exception.h>

//---------------------------------------------------------------------------

namespace Rapture
{
	template<class K, class T>
	class Cached
	{
	public:
		typedef K Key;
		typedef T Value;

		template<class ... A>
		static auto & get(const K & key, A &&... args)
		{
			static HashMap<K, T, Cached> _cache;

			auto & h = _cache[key];

			if(h == nullptr)
				h.init(key, forward<A>(args)...);

			return h;
		}

	protected:
		Cached() {}
		~Cached() {}
	};

	template<class K, class T>
	class Precached
	{
	public:
		typedef K Key;
		typedef T Value;

		static bool has(const K & key)
		{
			auto & _cache = cache();
			return _cache.find(key) != _cache.end();
		}

		static auto & get(const K & key)
		{
			auto & _cache = cache();
			auto & h = _cache[key];

			if(h == nullptr)
				throw Exception("Can't retreive value with the key \"", key, "\" from cache");
			
			return h;
		}

	protected:
		Precached(const K & key)
		{
			auto & _cache = cache();
			_cache.insert({key, Handle<T, Precached>(static_cast<T *>(this))});
		}

		~Precached() {}

		static auto & cache()
		{
			static HashMap<K, T, Precached> _cache;
			return _cache;
		}
	};
}

//---------------------------------------------------------------------------
#endif
