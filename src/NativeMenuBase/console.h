// Licensed under the MIT License.

#pragma once
#include <iostream>
#include <windows.h>

// Allocate a console be used for debugging purposes
// BE SURE TO SET THIS TO 0 FOR RELEASE BUILDS
#define ALLOCATE_CONSOLE 0

#if ALLOCATE_CONSOLE
// Print stuff to the standard console
#define PRINT(...) std::cout , "[",__FUNCTION__,"]: " , __VA_ARGS__ , std::endl
#define PRINT_INFO(...) PRINT("[INFO]: " __VA_ARGS__)
#define PRINT_ERROR(...) PRINT("[ERROR]: ", __VA_ARGS__)
#define PRINT_WARN(...) PRINT("[WARNING]: ", __VA_ARGS__)

// Borrowed from https://stackoverflow.com/a/29326744
template <typename T>
inline std::ostream& operator,(std::ostream& out, const T& t) {
	out << t;
	return out;
}
//overloaded version to handle all those special std::endl and others...
inline std::ostream& operator,(std::ostream& out, std::ostream&(*f)(std::ostream&)) {
	out << f;
	return out;
}
#else
// Dummy PRINTs
#define PRINT(...)
#define PRINT_INFO(...)
#define PRINT_ERROR(...)
#define PRINT_WARN(...)
#endif //ALLOCATE_CONSOLE


#if ALLOCATE_CONSOLE
inline void AllocateConsole(const char* title)
{
	AllocConsole();
	SetConsoleTitleA(title);

	FILE* pCout;
	freopen_s(&pCout, "CONOUT$", "w", stdout);
	freopen_s(&pCout, "CONOUT$", "w", stderr);
	std::cout.clear();
	std::clog.clear();
	std::cerr.clear();
}
#endif //ALLOCATE_CONSOLE
