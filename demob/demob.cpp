#include "demo/contract.h"
#include <iostream>

class DemoB_Component : public IComponent
{
public:
	DemoB_Component() : mRefCount(1) {
		std::cout << "DemoB_Component::DemoB_Component" << std::endl;
	}

	virtual ~DemoB_Component() {
		std::cout << "DemoB_Component::~DemoB_Component" << std::endl;
	}

	virtual void OnComponentAdded(IPlayer* player) {
		std::cout << "DemoB_Component:OnComponentAdded" << std::endl;
	}

	virtual void OnComponentRemoved(IPlayer* player) {
		std::cout << "DemoB_Component:OnComponentRemoved" << std::endl;
	}

public:
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

	virtual GPM_RES STDCALL ToInterface(GPM_TYPE type, IPluginObject** _out_Ptr) {
		if (type == GPM_TYPEOF(IComponent)) {
			*_out_Ptr = static_cast<IComponent*>(this);
			AddRef();
			return GPM_OK;
		}

		if (type == GPM_TYPEOF(IPluginObject)) {
			*_out_Ptr = static_cast<IPluginObject*>(this);
			AddRef();
			return GPM_OK;
		}

		*_out_Ptr = nullptr;
		return GPM_ERR;
	}

private:
	GPM_UINT64 mRefCount;
};

class DemoB_Activator : public IPluginActivator
{
public:
	DemoB_Activator() {}
	virtual ~DemoB_Activator() {}

	virtual void STDCALL Start(IPlugin* plugin) {
		std::cout << "DemoB_Activator:Start" << std::endl;
		mComponent = new DemoB_Component();
		plugin->RegisterObject(GPM_TYPEOF(IComponent), mComponent);
	}

	virtual void STDCALL Stop() {
		std::cout << "DemoB_Activator:Stop" << std::endl;
		mComponent->Release();
		mComponent = nullptr;
	}

private:
	DemoB_Component* mComponent;
};

DEFINE_ACTIVATOR(DemoB_Activator);
