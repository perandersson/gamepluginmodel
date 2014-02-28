#include "demo/contract.h"
#include <iostream>

class DemoA_Component : public IComponent
{
public:
	DemoA_Component() : mRefCount(1) {
		std::cout << "DemoA_Component::DemoA_Component" << std::endl;
	}

	virtual ~DemoA_Component() {
		std::cout << "DemoA_Component::~DemoA_Component" << std::endl;
	}

	virtual void OnComponentAdded(IPlayer* player) {
		std::cout << "DemoA_Component:OnComponentAdded" << std::endl;
	}

	virtual void OnComponentRemoved(IPlayer* player) {
		std::cout << "DemoA_Component:OnComponentRemoved" << std::endl;
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

class DemoA_Activator : public IPluginActivator
{
public:
	DemoA_Activator() {}
	virtual ~DemoA_Activator() {}

	virtual GPM_RES STDCALL Start(IPlugin* plugin) {
		std::cout << "DemoA_Activator:Start" << std::endl;
		mComponent = new DemoA_Component();
		plugin->RegisterObject(GPM_TYPEOF(IComponent), mComponent);
		return GPM_OK;
	}

	virtual void STDCALL Stop() {
		std::cout << "DemoA_Activator:Stop" << std::endl;
		mComponent->Release();
		mComponent = nullptr;
	}

private:
	DemoA_Component* mComponent;
};

DEFINE_ACTIVATOR(DemoA_Activator);
