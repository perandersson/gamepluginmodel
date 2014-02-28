#include "pluginlibrary.h"
#include "moduleloader.h"
#include <cassert>

PluginLibrary::PluginLibrary(LibraryHandle library, IPluginActivator* activator)
: mLibrary(library), mPluginActivator(activator), mPlugin(nullptr)
{
	assert(library != nullptr);
	assert(mPluginActivator != nullptr);
}

PluginLibrary::~PluginLibrary()
{
	if (mPlugin) {
		delete mPlugin;
		mPlugin = nullptr;
	}

	if (mLibrary != nullptr) {
		ModuleLoader::UnloadLibrary(mLibrary);
		mLibrary = nullptr;
	}
}

void PluginLibrary::Start(PluginContext* pluginContext)
{
	mPlugin = new Plugin(pluginContext);
	mPluginActivator->Start(mPlugin);
}

void PluginLibrary::Stop()
{
	mPluginActivator->Stop();
	mPlugin->UnloadObjects();
}

IPluginObject* PluginLibrary::FindObject(GPM_TYPE type) const
{
	return mPlugin->FindObject(type);
}

void PluginLibrary::NotifyObjectListeners(GPM_TYPE type, IPluginObject* object, IObjectListener::Status status)
{
	mPlugin->NotifyObjectListeners(type, object, status);
}
