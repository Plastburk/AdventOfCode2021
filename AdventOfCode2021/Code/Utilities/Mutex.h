#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

class Mutex
{
public:
	Mutex()
	{
#pragma warning(push)
#pragma warning(disable:6031)
		InitializeCriticalSectionAndSpinCount(&CriticalSection, 1024);
#pragma warning(pop)
	}

	~Mutex()
	{
		DeleteCriticalSection(&CriticalSection);
	}

	CRITICAL_SECTION CriticalSection;
};

class Lock
{
public:
	Lock(Mutex& mutex) : CriticalSection(&mutex.CriticalSection)
	{
		EnterCriticalSection(CriticalSection);
	}

	~Lock()
	{
		LeaveCriticalSection(CriticalSection);
	}

	CRITICAL_SECTION* CriticalSection;
};