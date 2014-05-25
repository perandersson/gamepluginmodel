#include "demo/contract.h"
#include <iostream>

class DemoA_Component : public IComponent
{
public:
	DemoA_Component() {
		std::cout << "DemoA_Component::DemoA_Component" << std::endl;
	}

	virtual ~DemoA_Component() {
		std::cout << "DemoA_Component::~DemoA_Component" << std::endl;
	}

	virtual void STDCALL OnComponentAdded(IPlayer* player) {
		std::cout << "DemoA_Component:OnComponentAdded" << std::endl;
	}

	virtual void STDCALL OnComponentRemoved(IPlayer* player) {
		std::cout << "DemoA_Component:OnComponentRemoved" << std::endl;
	}
};

class DemoA_Activator : public IPluginActivator
{
public:
	DemoA_Activator() {}
	virtual ~DemoA_Activator() {}

	virtual GPM_RES STDCALL Start(IPlugin* plugin) {
		std::cout << "DemoA_Activator:Start" << std::endl;
		mComponent = new DemoA_Component();
		plugin->RegisterObject(IID_Demo::IComponent, mComponent);
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
