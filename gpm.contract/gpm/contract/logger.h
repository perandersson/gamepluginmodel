#pragma once
#include "../typedefs.h"

struct PLUGIN_API ILogger
{
	virtual ~ILogger() {}

	//
	// Loggs an error that occured in the plugin framework
	// 
	// @param message
	//			The error message
	virtual void Error(const char* message) = 0;

	//
	// Loggs a debug message that occured in the plugin framework
	// 
	// @param message
	//			The debug message
	virtual void Debug(const char* message) = 0;
};