#include "plugincontext.h"
#include "moduleloader.h"
#include "pluginlibrary.h"
#include <iostream>
#include <string>
#include <stdarg.h>

namespace {
	std::string ToMessage(const char* message, ...) {
		char tmp[1024];
		va_list arglist;
		va_start(arglist, message);
		vsprintf_s(tmp, 1024, message, arglist);
		va_end(arglist);
		return std::string(tmp);
	}
}

PluginContext::PluginContext()
: mLogger(nullptr)
{

}

PluginContext::~PluginContext()
{
	// Unload libraries incase the host application forgot to invoke the UnloadLibraries method.
	UnloadPlugins();
}

void PluginContext::LoadPlugin(const char* path)
{
	if (path == nullptr) {
		Error("Invalid argument: 'path'");
		return;
	}

	auto library = ModuleLoader::GetLibraryHandle(path);
	if (library == nullptr) {
		Error(ToMessage("Library: '%s' not found", path).c_str());
		return;
	}

	auto getActivator = ModuleLoader::GetFunction<GPM1_GetActivatorFunc>(library, "GPM1_GetActivator");
	if (getActivator == nullptr) {
		ModuleLoader::UnloadLibrary(library);
		Error(ToMessage("Library: '%s' does not have the entrypoint: 'GPM1_GetActivator'", path).c_str());
		return;
	}

	auto activator = (*getActivator)();
	if (activator == nullptr) {
		Error(ToMessage("Invalid entrypoint received for library: '%s'", path).c_str());
		return;
	}

	// Add the new plugin into the opened plugins vector and start it
	auto pluginLibrary = new PluginLibrary(library, activator);
	auto ptr = std::shared_ptr<PluginLibrary>(pluginLibrary);
	mLibraries.push_back(ptr);
#ifdef _DEBUG
	Debug(ToMessage("Plugin activator starting for library: '%s'", path).c_str());
#endif
	ptr->Start(this);
#ifdef _DEBUG
	Debug(ToMessage("Plugin activator started for library: '%s'", path).c_str());
#endif
}

void PluginContext::UnloadPlugins()
{
#ifdef _DEBUG
	Debug("Unloading libraries");
#endif

	mGlobalObjects.clear();

	Libraries::size_type size = mLibraries.size();
	for (Libraries::size_type i = 0; i < size; ++i) {
		mLibraries[i]->Stop();
	}
	mLibraries.clear();

#ifdef _DEBUG
	Debug("Libraries unloaded");
#endif
}

void PluginContext::NotifyObjectListeners(GPM_TYPE type, IPluginObject* object, IObjectListener::Status status)
{
	Libraries::size_type size = mLibraries.size();
	for (Libraries::size_type i = 0; i < size; ++i) {
		mLibraries[i]->NotifyObjectListeners(type, object, status);
	}
}

void PluginContext::RegisterGlobalObject(GPM_TYPE type, IPluginObject* object)
{
	auto reference = std::shared_ptr<ObjectReference>(new ObjectReference(type, object));
	mGlobalObjects.push_back(reference);
	NotifyObjectListeners(type, object, IObjectListener::STATUS_REGISTERED);
}

void PluginContext::UnregisterGlobalObject(IPluginObject* object)
{
	References::size_type referenceCount = mGlobalObjects.size();
	for (References::size_type i = 0; i < referenceCount; ++i) {
		auto reference = mGlobalObjects[i];
		if (reference->GetObject() == object) {
			References::iterator it = mGlobalObjects.begin() + i;
			mGlobalObjects.erase(it);
			NotifyObjectListeners(reference->GetType(), object, IObjectListener::STATUS_UNREGISTERED);
			break;
		}
	}
}

void PluginContext::Error(const char* message)
{
	if (mLogger != nullptr)
		mLogger->Error(message);
	else
		std::cerr << "[ERROR] " << message << std::endl;
}

void PluginContext::Debug(const char* message)
{
	if (mLogger != nullptr)
		mLogger->Error(message);
	else {
#ifdef _DEBUG
		std::cout << "[DEBUG] " << message << std::endl;
#endif
	}
}

IPluginObject* STDCALL PluginContext::GetObject(GPM_TYPE type)
{
	References::size_type referenceCount = mGlobalObjects.size();
	for (References::size_type i = 0; i < referenceCount; ++i) {
		if (mGlobalObjects[i]->IsType(type)) {
			auto object = mGlobalObjects[i]->GetObject();
			object->AddRef();
			return object;
		}
	}

	Libraries::size_type libraryCount = mLibraries.size();
	for (Libraries::size_type i = 0; i < libraryCount; ++i) {
		auto object = mLibraries[i]->FindObjectByType(type);
		if (object != nullptr) {
			object->AddRef();
			return object;
		}
	}
	return nullptr;
}

IPluginObject* STDCALL PluginContext::GetObject(GPM_TYPE type, const char* filter)
{
	// TODO Implement for real

	return GetObject(type);
}

GPM_UINT32 STDCALL PluginContext::GetObjects(GPM_TYPE type, IPluginObject** _out_Objects, GPM_UINT32 objectsSize)
{
	if (_out_Objects == nullptr || objectsSize == 0) {
		return 0;
	}

	// TODO Implement for real

	auto result = GetObject(type);
	_out_Objects[0] = result;
	return 1;
}

void STDCALL PluginContext::SetLogger(ILogger* logger)
{
	mLogger = logger;
}
