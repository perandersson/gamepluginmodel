#include "plugincontext.h"
#include "moduleloader.h"
#include "pluginlibrary.h"
#include <iostream>

PluginContext::PluginContext()
{

}

PluginContext::~PluginContext()
{
	// Unload libraries incase the host application forgot to invoke the UnloadLibraries method.
	UnloadLibraries();
}

void PluginContext::LoadLibrary(const char* path)
{
	if (path == nullptr) {
		// SetLastError(PL_ERRCODE_INVALIDARGUMENT);
		return;
	}

	auto library = ModuleLoader::GetLibraryHandle(path);
	if (library == nullptr) {
		//SetLastError(PL_ERRCODE_LIBRARYNOTFOUND);
		return;
	}

	auto getActivator = ModuleLoader::GetFunction<GPM1_GetActivatorFunc>(library, "GPM1_GetActivator");
	if (getActivator == nullptr) {
		ModuleLoader::UnloadLibrary(library);
		//SetLastError(PL_ERRCODE_MISSINGENTRYPOINT);
		return;
	}

	auto activator = (*getActivator)();
	if (activator == nullptr) {
		// SetLastError(PL_ERRCODE_ENTRYPOINTFAILURE);
		return;
	}

	// Add the new plugin into the opened plugins vector and start it
	auto pluginLibrary = new PluginLibrary(library, activator);
	auto ptr = std::shared_ptr<PluginLibrary>(pluginLibrary);
	mLibraries.push_back(ptr);
	ptr->Start(this);
}

void PluginContext::UnloadLibraries()
{
	mGlobalObjects.clear();

	Libraries::size_type size = mLibraries.size();
	for (Libraries::size_type i = 0; i < size; ++i) {
		mLibraries[i]->Stop();
	}
	mLibraries.clear();
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

void PluginContext::Log(const char* message)
{
	std::cout << "PLUGINCONTEXT: " << message << std::endl;
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
		auto object = mLibraries[i]->FindObject(type);
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
