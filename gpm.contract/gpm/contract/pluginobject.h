#pragma once
#include "../typedefs.h"

//
// Base class for all objects exposed from the plugin framework. Whenever a new plugin is requesting
// a IPluginObject1 resource then {@code IPluginObject::AddRef} is called automatically. The plugin
// is then responsible for releasing it's pointer using {@code IPluginObject::Release} whenever it's not
// used anymore. 
//
// Plugins can Release their IPluginObject references whenever they want, but they must release it during
// their SHUTDOWN phase.
struct PLUGIN_API IPluginObject
{
	virtual STDCALL ~IPluginObject() {}

	//
	// Increments the internal reference counter for this object.
	//
	// @return The new reference count
	virtual GPM_UINT64 STDCALL AddRef() = 0;

	//
	// Decrements the internal reference counter for this object. The owner plugin is
	// free to release this objects internal memory whenever it reaches 0 - if it want's to.
	//
	// @return The new reference count.
	virtual GPM_UINT64 STDCALL Release() = 0;

	//
	// Converts this type into a new type - if possible. This is up the the one exposing
	// the original pointer to begin with.
	//
	// Reason for using this method instead of performing dynamic_cast is because different
	// types are defined differently depending on which compiler is used when building the plugin.
	//
	// @param type
	//			The new type
	// @param _out_Ptr
	//			A memory location where we put the new pointer
	// @return GPM_OK if successfull; GPM_ERR otherwise;
	virtual GPM_RES STDCALL ToInterface(GPM_TYPE type, IPluginObject** _out_Ptr) = 0;
};
