#pragma once
#include "pluginobject.h"
#include "plugincontext.h"
#include "plugin.h"

struct PLUGIN_API IPluginActivator
{
	virtual STDCALL ~IPluginActivator() {}

	//
	// Method invoked during the startup phase of the plugin
	//
	// @param plugin
	//			The pointer to the plugin associated with this activator. Each activator has it's own plugin instance
	// @return GPM_OK if the startup sequence succeeded; GPM_ERR otherwise.
	virtual GPM_RES STDCALL Start(IPlugin* plugin) = 0;

	//
	// Method invoked during the shutdown phase of the plugin. This is where the plugin releases it's exposed resources, i.e. release them
	// using the {@code IPluginObject::Release} method.
	//
	// This method is called for all plugins until the actual library is unloaded (the memory is returned to the operating system).
	virtual void STDCALL Stop() = 0;
};

//
// Structure used by the host application to identify the plugin
//

struct GPM_ACTIVATOR_INFO {
	char name[128];
	char compiler[128];
};

// Signature for the activator retrival function used by the plugin framework
typedef IPluginActivator*(*GPM1_GetActivatorFunc)();

// Signature for the function used to retrieve information about the compiler used build the plugin
typedef const GPM_ACTIVATOR_INFO*(*GPM1_GetActivatorInfoFunc)();

//
// Defines which class is the plugin's activator class.
#define DEFINE_ACTIVATOR(ActivatorType) \
extern "C" { \
	ActivatorType gPluginActivator; \
	PLUGIN_API IPluginActivator* CDECL GPM1_GetActivator() { \
		return &gPluginActivator; \
	}\
	static const GPM_ACTIVATOR_INFO _GPM_ActivatorInfo = { { #ActivatorType }, { GPM_COMPILER_NAME } }; \
	PLUGIN_API const GPM_ACTIVATOR_INFO* CDECL GPM1_GetActivatorInfo() { \
		return &_GPM_ActivatorInfo; \
	} \
}
