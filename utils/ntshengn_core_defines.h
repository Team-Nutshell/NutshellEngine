#pragma once
#include <iostream>

#define NTSHENGN_CORE_INFO(message) \
	do { \
		std::cout << "\33[1m\33[39mCORE \33[34mINFO\33[39m\33[0m: " + std::string(message) << std::endl; \
	} while(0)

#define NTSHENGN_CORE_WARNING(message) \
	do { \
		std::cout << "\33[1m\33[39mCORE \33[93mWARNING\33[39m\33[0m: " + std::string(message) << std::endl; \
	} while(0)

#define NTSHENGN_CORE_ERROR(message) \
	do { \
		std::cout << "\33[1m\33[39mCORE \33[31mERROR\33[39m\33[0m: " + std::string(message) << std::endl; \
		exit(1); \
	} while(0)