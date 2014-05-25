#include "demo/contract.h"
#include <iostream>

class DemoB_Component : public IComponent
{
public:
	DemoB_Component() {
		std::cout << "DemoB_Component::DemoB_Component" << std::endl;
	}

	virtual ~DemoB_Component() {
		std::cout << "DemoB_Component::~DemoB_Component" << std::endl;
	}

	virtual void STDCALL OnComponentAdded(IPlayer* player) {
		std::cout << "DemoB_Component:OnComponentAdded" << std::endl;
	}

	virtual void STDCALL OnComponentRemoved(IPlayer* player) {
		std::cout << "DemoB_Component:OnComponentRemoved" << std::endl;
	}
};

class DemoB_Activator : public IPluginActivator
{
public:
	DemoB_Activator() {}
	virtual ~DemoB_Activator() {}

	virtual GPM_RES STDCALL Start(IPlugin* plugin) {
		std::cout << "DemoB_Activator:Start" << std::endl;
		mComponent = new DemoB_Component();
		plugin->RegisterObject(IID_Demo::IComponent, mComponent);
		return GPM_OK;
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
