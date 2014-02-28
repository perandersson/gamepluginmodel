#pragma once
#include "pluginobject.h"
#include "logger.h"

//
// This interface symbolizes the entire lifetime of the host application. 
//
// Only one instance of this type exists and it is never removed during a plugin's lifetime.
struct PLUGIN_API IPluginContext
{
	virtual STDCALL ~IPluginContext() {}

	//
	// Retrieves the first object matching the supplied type
	//
	// @param type
	//			The type we are looking for
	// @return A pointer to the plugin object; NULL if no plugin was found.
	virtual IPluginObject* STDCALL GetObject(GPM_TYPE type) = 0;

	//
	// Retrieves the first object matching the supplied type and filter
	//
	// @param type
	//			The type we are looking for
	// @param filter
	//			A search filter
	// @return A pointer to the plugin object; NULL if no plugin was found.
	virtual IPluginObject* STDCALL GetObject(GPM_TYPE type, const char* filter) = 0;

	//
	// Retrieves all the objects matching the supplied type and put the result into _out_Objects
	//
	// @param type
	//			The type we are looking for
	// @param _out_Objects
	//			A memory location where we can put the result.
	// @param objectsSize
	//			The size of the _out_Objects array (The maximum amount of objects we can receive).
	// @return How many objects where found
	virtual GPM_UINT32 STDCALL GetObjects(GPM_TYPE type, IPluginObject** _out_Objects, GPM_UINT32 objectsSize) = 0;

	//
	// Set a logger used by the plugin context
	//
	// @param logger
	virtual void STDCALL SetLogger(ILogger* logger) = 0;
};
