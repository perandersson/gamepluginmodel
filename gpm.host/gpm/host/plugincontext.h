#pragma once
#include "gpm/contract.h"
#include "moduleloader.h"
#include <vector>
#include <memory>
#include <list>

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
	typedef std::list<IObjectListener*> ObjectListeners;
	friend class Plugin;

public:
	PluginContext(const GPM_ACTIVATOR_INFO* compilerInfo);
	virtual ~PluginContext();

	//
	// Load the supplied library.
	//
	// @param path
	//			The relative path to the dynamic library (without the suffix).
	void LoadPlugin(const char* path);

	//
	//	Unload all loaded libraries
	//
	void UnloadPlugins();

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
	void Error(const char* message);

	//
	// Perform logging
	//
	// @param message
	void Debug(const char* message);

protected:
	//
	// Notify all the object listeners added to this plugin
	// 
	// @param type
	// @param object
	// @param status
	void NotifyObjectListeners(GPM_TYPE type, IPluginObject* object, ObjectListenerStatus::Enum status);

private:
	//
	// Load a specific library
	//
	// @param path
	void LoadPluginAtPath(const char* path);

	//
	// Verify the supplied library that it's compatible with the host application
	//
	// @param library
	bool VerifyLibraryWithHost(LibraryHandle library, const char* path);

// IPluginContext
public:
	virtual IPluginObject* STDCALL GetObject(GPM_TYPE type);
	virtual IPluginObject* STDCALL GetObject(GPM_TYPE type, const char* filter);
	virtual void STDCALL AddObjectListener(IObjectListener* listener);
	virtual void STDCALL AddObjectListener(IObjectListener* listener, const char* filter);
	virtual void STDCALL RemoveObjectListener(IObjectListener* listener);
	virtual GPM_UINT32 STDCALL GetObjects(GPM_TYPE type, IPluginObject** _out_Objects, GPM_UINT32 objectsSize);
	virtual void STDCALL SetLogger(ILogger* logger);

private:
	const GPM_ACTIVATOR_INFO* mCompilerInfo;
	Libraries mLibraries;
	References mGlobalObjects;
	ObjectListeners mObjectListeners;
	ILogger* mLogger;
};

//
// Helper function used to create a specific type of plugin context
template<class T>
static inline std::shared_ptr<T> CreatePluginContext() {
	static const GPM_ACTIVATOR_INFO info = { { "HOST" }, { GPM_COMPILER_NAME } };
	return std::shared_ptr<T>(new T(&info));
}
