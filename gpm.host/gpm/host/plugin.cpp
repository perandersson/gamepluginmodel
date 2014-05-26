#include "plugin.h"
#include <algorithm>

Plugin::Plugin(PluginContext* pluginContext)
: mPluginContext(pluginContext)
{

}

Plugin::~Plugin()
{
	
}

IPluginObject* Plugin::FindObjectByType(GPM_TYPE type) const
{
	References::size_type size = mReferences.size();
	for (References::size_type i = 0; i < size; ++i) {
		if (mReferences[i]->IsType(type)) {
			return mReferences[i]->GetObject();
		}
	}

	return nullptr;
}

void Plugin::UnloadObjects()
{
	mReferences.clear();
}

void Plugin::NotifyObjectListeners(GPM_TYPE type, IPluginObject* object, ObjectListenerStatus::Enum status)
{
	ObjectListeners::iterator it = mObjectListeners.begin();
	ObjectListeners::const_iterator end = mObjectListeners.end();
	for (; it != end; ++it) {
		auto listener = (*it);
		object->AddRef();
		listener->OnObjectChanged(type, object, status);
		object->Release();
	}
}

PluginStatus::Enum STDCALL Plugin::GetStatus() const
{
	return mStatus;
}

void STDCALL Plugin::RegisterObject(GPM_TYPE type, IPluginObject* object)
{
	auto reference = std::shared_ptr<ObjectReference>(new ObjectReference(type, object));
	mReferences.push_back(reference);
	mPluginContext->NotifyObjectListeners(type, object, ObjectListenerStatus::REGISTERED);
}

void STDCALL Plugin::UnregisterObject(IPluginObject* object)
{
	References::size_type referenceCount = mReferences.size();
	for (References::size_type i = 0; i < referenceCount; ++i) {
		auto reference = mReferences[i];
		if (reference->GetObject() == object) {
			References::iterator it = mReferences.begin() + i;
			mReferences.erase(it);
			mPluginContext->NotifyObjectListeners(reference->GetType(), object, ObjectListenerStatus::UNREGISTERED);
			break;
		}
	}
}

void STDCALL Plugin::AddObjectListener(IObjectListener* listener)
{
	mObjectListeners.push_back(listener);
}

void STDCALL Plugin::AddObjectListener(IObjectListener* listener, const char* filter)
{
	// TODO Add support for filter!!!
	mObjectListeners.push_back(listener);
}

void STDCALL Plugin::RemoveObjectListener(IObjectListener* listener)
{
	ObjectListeners::iterator it = std::find(mObjectListeners.begin(), mObjectListeners.end(), listener);
	if (it == mObjectListeners.end())
		return;

	mObjectListeners.erase(it);
}

IPluginObject* STDCALL Plugin::GetObject(GPM_TYPE type)
{
	return mPluginContext->GetObject(type);
}

IPluginObject* STDCALL Plugin::GetObject(GPM_TYPE type, const char* filter)
{
	return mPluginContext->GetObject(type, filter);
}

GPM_UINT32 STDCALL Plugin::GetObjects(GPM_TYPE type, IPluginObject** _out_Objects, GPM_UINT32 objectsSize)
{
	return mPluginContext->GetObjects(type, _out_Objects, objectsSize);
}
