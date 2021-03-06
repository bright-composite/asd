//---------------------------------------------------------------------------

#pragma once

#ifndef BENCHMARK_H
#define BENCHMARK_H

//---------------------------------------------------------------------------

#include <string>
#include <chrono>
#include <random>
#include <iostream>

#include <meta/useif.h>

//---------------------------------------------------------------------------

namespace asd
{
	struct benchmark
	{
		benchmark(const char * title, int count = 1) : title(title), count(count) {}
		benchmark(const std::string & title, int count = 1) : title(title), count(count) {}
		
		template <class F>
		void operator << (F && func) {
			using namespace std::chrono;
			static const int N = 100;
			
			long long t = 0;
			
			time_point<high_resolution_clock> last;
			
			for (int i = 0; i < N; ++i) {
				last = high_resolution_clock::now();
				func();
				t += duration_cast<nanoseconds>(high_resolution_clock::now() - last).count();
			}
			
			std::cout << title << ": " << t / N << " ns" << std::endl;
		}

		template <class F>
		int operator() (F && func) {
			using namespace std::chrono;

			time_point<high_resolution_clock> last;

			last = high_resolution_clock::now();
			func();

			return static_cast<int>(duration_cast<nanoseconds>(high_resolution_clock::now() - last).count());
		}

		std::string title;
		int count;
	};
}

//---------------------------------------------------------------------------
#endif
