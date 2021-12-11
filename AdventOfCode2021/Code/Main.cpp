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
#include "Utilities/Utilities.h"

#include <iostream>
#include <fstream>
#include <ppl.h>

#define CASE_DAY(day) case day: \
	{ \
		Day##day obj; \
		std::ifstream stream("inputs/" #day ".txt"); \
		obj.ReadInput(stream); \
		std::cout << #day "a: " << obj.RunA() << std::endl; \
		std::cout << #day "b: " << obj.RunB() << std::endl; \
		break; \
	}

int main(int argc, char* argv[])
{
	Concurrency::CurrentScheduler::Create(Concurrency::SchedulerPolicy(1, Concurrency::MaxConcurrency, 8));

	int iterations = ToInt(argv[1]);

	for (int i = 0; i < iterations; i++)
	{
		std::cout << "Solving" << std::endl;
		for (int j = 2; j < argc; j++)
		{
			switch (ToInt(argv[j]))
			{
				CASE_DAY(1);
				CASE_DAY(2);
				CASE_DAY(3);
				CASE_DAY(4);
				CASE_DAY(5);
				CASE_DAY(6);
				CASE_DAY(7);
				CASE_DAY(8);
				CASE_DAY(9);
				CASE_DAY(10);
				CASE_DAY(11);
			}
		}
		std::cout << "Done" << std::endl;
	}
}