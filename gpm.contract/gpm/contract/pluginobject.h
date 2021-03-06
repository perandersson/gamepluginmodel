#pragma once
#include "../typedefs.h"
#include "plugin.h"
#include "plugincontext.h"

//
// Base class for all objects exposed from the plugin framework. Whenever a new plugin is requesting
// a IPluginObject resource then {@code IPluginObject::AddRef} is called automatically. The plugin
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
	// Retrieves the unique ID for this type
	virtual GPM_TYPE STDCALL GetTypeID() = 0;

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
	virtual GPM_RES STDCALL GetInterface(GPM_TYPE type, IPluginObject** _out_Ptr) = 0;
};

template<GPM_TYPE ID>
struct PLUGIN_API TPluginInterface : IPluginObject
{
protected:
	GPM_UINT64 mRefCount;

public:
	static const GPM_TYPE TypeID = ID;

public:
	TPluginInterface() : mRefCount(1)
	{
	}

	virtual STDCALL ~TPluginInterface()
	{
	}

	virtual GPM_UINT64 STDCALL AddRef() {
		return ++mRefCount;
	}

	virtual GPM_UINT64 STDCALL Release() {
		if (--mRefCount == 0) {
			delete this;
			return 0;
		}
		return mRefCount;
	}

	virtual GPM_TYPE STDCALL GetTypeID() {
		return TypeID;
	}

	virtual GPM_RES STDCALL GetInterface(GPM_TYPE type, IPluginObject** _out_Ptr) {
		switch (type) {
		case TypeID:
			*_out_Ptr = this;
			AddRef();
			return GPM_OK;
		default:
			*_out_Ptr = nullptr;
			return GPM_ERR;
		}
	}

	//
	// Method for simplifying the object registration. This ensures that you always supply the correct whenever registering an object. It also
	// ensures that an object registers all it's ID's (whenever inheritance is used).
	//
	// This replaces:
	// plugin->RegisterObject(IID_YourType::IMyInterface, mGame);
	virtual void RegisterObject(IPlugin* plugin) {
		plugin->RegisterObject(TypeID, this);
	}
};

template<GPM_TYPE ID, class Base>
struct PLUGIN_API TInheritedPluginInterface : Base
{
public:
	static const GPM_TYPE TypeID = ID;

public:
	virtual GPM_TYPE STDCALL GetTypeID() {
		return TypeID;
	}

	virtual void RegisterObject(IPlugin* plugin) {
		plugin->RegisterObject(TypeID, this);
		Base::RegisterObject(plugin);
	}

public:
	virtual GPM_RES STDCALL GetInterface(GPM_TYPE type, IPluginObject** _out_Ptr) {
		switch (type) {
		case TypeID:
			*_out_Ptr = this;
			AddRef();
			return GPM_OK;
		default:
			return Base::GetInterface(type, _out_Ptr);
		}
	}
};

//
// Converts this into the supplied type - if possible. If you are planning on saving the pointer in your local memory area then
// call {@code IPluginObject::AddRef} and then release whenever you're done with it.
//
// @param type
//			The type we want to convert this object into
// @return A valid pointer if possible; nullptr otherwise
template<class T>
static T* interface_cast(IPluginObject* obj) {
	if (obj == nullptr)
		return nullptr;

	T* ptr = nullptr;
	if (obj->GetInterface(T::TypeID, (IPluginObject**)(&ptr)) != GPM_OK) {
		return nullptr;
	}
	ptr->Release();
	return ptr;
}
