#include "Day1.h"
#include "Day2.h"
#include "Day3.h"
#include "Day4.h"
#include "Day5.h"
#include "Day6.h"
#include "Day7.h"
#include "Day8.h"
#include "Day9.h"
#include "Day10.h"
#include "Day11.h"
#include "Day12.h"
#include "Day13.h"
#include "Utilities/Utilities.h"

#include <iostream>
#include <fstream>
#include <ppl.h>

#define CASE_DAY(day, formatA, formatB) case day: \
	{ \
		Day##day obj; \
		std::ifstream stream("inputs/" #day ".txt"); \
		obj.ReadInput(stream); \
		auto a = obj.RunA(); \
		auto b = obj.RunB(); \
		printf(#day "a: " ##formatA "\n" #day "b: " ##formatB "\n", a, b); \
		break; \
	}

#define CASE_DAY_2(day, formatA) case day: \
	{ \
		Day##day obj; \
		std::ifstream stream("inputs/" #day ".txt"); \
		obj.ReadInput(stream); \
		printf(#day "a: " ##formatA "\n" #day "b: ", obj.RunA()); \
		obj.RunB(); \
		break; \
	}

int main(int argc, char* argv[])
{
	Concurrency::CurrentScheduler::Create(Concurrency::SchedulerPolicy(1, Concurrency::MaxConcurrency, 8));

	int iterations = atoi(argv[1]);

	for (int i = 0; i < iterations; i++)
	{
		std::cout << "Solving" << std::endl;
		for (int j = 2; j < argc; j++)
		{
			switch (atoi(argv[j]))
			{
				CASE_DAY(1, "%d", "%d");
				CASE_DAY(2, "%d", "%d");
				CASE_DAY(3, "%d", "%d");
				CASE_DAY(4, "%d", "%d");
				CASE_DAY(5, "%d", "%d");
				CASE_DAY(6, "%lld", "%lld");
				CASE_DAY(7, "%d", "%d");
				CASE_DAY(8, "%d", "%d");
				CASE_DAY(9, "%d", "%d");
				CASE_DAY(10, "%d", "%lld");
				CASE_DAY(11, "%d", "%d");
				CASE_DAY(12, "%d", "%d");
				CASE_DAY_2(13, "%d");
			}
		}
		std::cout << "Done" << std::endl;
	}
}