#pragma once
#include "gpm/contract.h"
#include "moduleloader.h"
#include "plugin.h"
#include "plugincontext.h"

class PluginLibrary
{
public:
	PluginLibrary(LibraryHandle library, IPluginActivator* activator);
	~PluginLibrary();

	//
	// Start the plugin and invoke the activator
	void Start(PluginContext* pluginContext);

	//
	// Invoke the activator and stop the plugin
	void Stop();

	//
	// Return the first object found that matches the supplied type in this library
	//
	// @param type
	// @return
	IPluginObject* FindObjectByType(GPM_TYPE type) const;

	//
	// Notify all the object listeners added to this plugin
	void NotifyObjectListeners(GPM_TYPE type, IPluginObject* object, IObjectListener::Status status);

private:
	LibraryHandle mLibrary;

private:
	IPluginActivator* mPluginActivator;
	Plugin* mPlugin;
};