#include <iostream>
#include <vector>
#include <fstream>
#include "Utilities.h"
#include "Day1.h"

#define CASE_DAY(day) case day: \
	Day##day obj; \
	auto input = ReadFile("inputs/" #day ".txt"); \
	std::cout << #day "a: " << obj.RunA(input) << std::endl; \
	std::cout << #day "b: " << obj.RunB(input) << std::endl; \
	break;

inline std::vector<std::string> ReadFile(const char* file)
{
	std::ifstream stream(file);
	std::vector<std::string> lines;
	lines.reserve(3000);
	std::string line;
	while (std::getline(stream, line))
	{
		lines.push_back(line);
	}
	return lines;
}

int main(int argc, char* argv[])
{
	int iterations = ToInt(argv[1]);

	for (int i = 0; i < iterations; i++)
	{
		std::cout << "Solving" << std::endl;
		for (int j = 2; j < argc; j++)
		{
			switch (ToInt(argv[j]))
			{
				CASE_DAY(1);
			}
		}
		std::cout << "Done" << std::endl;
	}
}