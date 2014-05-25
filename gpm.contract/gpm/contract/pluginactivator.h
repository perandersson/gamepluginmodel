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
	// @return GPM_OK if the startup sequence succeeded; GPM_ERR otherwise.
	virtual GPM_RES STDCALL Start(IPlugin* plugin) = 0;

	//
	// Method invoked during the shutdown phase of the plugin. This is where the plugin releases it's exposed resources, i.e. release them
	// using the {@code IPluginObject::Release} method.
	//
	// This method is called for all plugins until the actual library is unloaded (the memory is returned to the operating system).
	virtual void STDCALL Stop() = 0;
};

// Signature for the activator retrival function used by the plugin framework
typedef IPluginActivator*(*GPM1_GetActivatorFunc)();

//
// Defines which class is the plugin's activator class.
#define DEFINE_ACTIVATOR(ActivatorType) \
extern "C" { \
	PLUGIN_API IPluginActivator* CDECL GPM1_GetActivator() { \
		std::cout << "Getting activator: "#ActivatorType << std::endl; \
		return new ActivatorType(); \
	}\
}
