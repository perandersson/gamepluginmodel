#pragma once
#include "gpm/contract.h"
#include <vector>
#include <memory>

class Plugin;
class PluginLibrary;
class ObjectReference;

//
// The most basic implementation of the plugin context interface. This simply loads dynamic libraries
// at whatever path is supplied to it.
class PluginContext : public IPluginContext
{
	typedef std::vector<std::shared_ptr<PluginLibrary>> Libraries;
	typedef std::vector<std::shared_ptr<ObjectReference>> References;
	friend class Plugin;

public:
	PluginContext();
	virtual ~PluginContext();

	//
	// Load the supplied library.
	//
	// @param path
	//			The relative path to the dynamic library (without the suffix).
	void LoadLibrary(const char* path);

	//
	//	Unload all loaded libraries
	//
	void UnloadLibraries();

	//
	// Register a global object and expose it into the plugin framework
	// 
	// @param type
	//			The type of the supplied object
	// @param object
	//			The object we want to register
	void RegisterGlobalObject(GPM_TYPE type, IPluginObject* object);

	//
	// Unregisters a global plugin object.
	//
	// @param object
	//			The object we want to unregister
	void UnregisterGlobalObject(IPluginObject* object);

	//
	// Perform logging
	//
	// @param message
	virtual void Log(const char* message);

protected:
	//
	// Notify all the object listeners added to this plugin
	// 
	// @param type
	// @param object
	// @param status
	void NotifyObjectListeners(GPM_TYPE type, IPluginObject* object, IObjectListener::Status status);

// IPluginContext
public:
	virtual IPluginObject* STDCALL GetObject(GPM_TYPE type);
	virtual IPluginObject* STDCALL GetObject(GPM_TYPE type, const char* filter);
	virtual GPM_UINT32 STDCALL GetObjects(GPM_TYPE type, IPluginObject** _out_Objects, GPM_UINT32 objectsSize);

private:
	Libraries mLibraries;
	References mGlobalObjects;
};
