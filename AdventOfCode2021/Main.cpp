#include <iostream>
#include <fstream>
#include "Day1.h"

#define CASE_DAY(day) case day: \
	Day##day obj; \
	auto inputA = ReadFile("inputs/" #day "a.txt"); \
	auto inputB = ReadFile("inputs/" #day "b.txt"); \
	std::cout << #day "a: " << obj.RunA(inputA) << std::endl; \
	std::cout << #day "b: " << obj.RunB(inputB) << std::endl; \
	break;

inline std::string ReadFile(const char* file)
{
	std::ifstream stream(file);
	return std::string(std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>());
}

int main(int argc, char* argv[])
{
	int iterations = atoi(argv[1]);

	for (int i = 0; i < iterations; i++)
	{
		std::cout << "Solving" << std::endl;
		for (int j = 2; j < argc; j++)
		{
			switch (atoi(argv[j]))
			{
				CASE_DAY(1);
			}
		}
		std::cout << "Done" << std::endl;
	}
}