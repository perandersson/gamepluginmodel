#pragma once
#include "pluginobject.h"
#include "plugincontext.h"
#include "objectlistener.h"

struct PLUGIN_API PluginStatus
{
	enum Enum {
		//
		// Symbolizes that the plugin is starting up. It is in this step that 
		// services are registered
		STARTING = 0,

		//
		// Symbolizes that the plugin is active and all it's services registered
		// inside the plugin engine
		ACTIVE = 1,

		//
		// The plugin is stopping and is currently removing it's internal resources
		STOPPING = 2,

		//
		// The plugin has stopped, it is not unloaded from memory though since references
		// are still being kept by other plugins.
		STOPPED = 3,
	};
};

//
// Interface that represents the entry- and exit point of a plugin.
//
// This instance is never removed during the lifetime of the plugin that owns it.
struct PLUGIN_API IPlugin
{
	virtual STDCALL ~IPlugin() {}

	//
	// @return The status for this plugin
	virtual PluginStatus::Enum STDCALL GetStatus() const = 0;

	//
	// Register a new plugin object. 
	//
	// It is the plugin that owns the memory of the object. This means that the plugin is responsible 
	// to clean the memory up during the STATUS_STOPPING phase. All objects are automatically
	// unregistered during the plugins STATUS_STOPPING phase.
	//
	// @param type
	//			The type of the supplied service
	// @param object
	//			The object we want to register
	virtual void STDCALL RegisterObject(GPM_TYPE type, IPluginObject* object) = 0;

	//
	// Unregisters an exposed plugin object. 
	//
	// The memory for this object might still be in use, so remember to not delete the
	// object manually, but instead call the {@code IPluginObject1::Release} method instead. The memory will automatically
	// be released either when the object's life-cycle has ended (reference counter reaches 0) or whenever the plugin is stopped.
	//
	// @param object
	//			The object we want to unregister
	virtual void STDCALL UnregisterObject(IPluginObject* object) = 0;

	//
	// Listen for object registrations. 
	//
	// @param listener
	//			The listener instance
	virtual void STDCALL AddObjectListener(IObjectListener* listener) = 0;

	//
	// Listen for object registrations given a predetermained filter. 
	//
	// @param listener
	//			The listener instance
	// @param filter
	//			The object filter
	virtual void STDCALL AddObjectListener(IObjectListener* listener, const char* filter) = 0;

	//
	// Remove an object listener
	//
	// @param listener
	//			The listener instance
	virtual void STDCALL RemoveObjectListener(IObjectListener* listener) = 0;

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
};
