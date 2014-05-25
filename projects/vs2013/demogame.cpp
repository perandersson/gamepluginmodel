#include "demo/contract.h"
#include <iostream>
#include <cassert>

class DemoGame : public IGame
{
public:
	DemoGame() {
		std::cout << "DemoGame::DemoGame" << std::endl;
	}

	virtual ~DemoGame() {
		std::cout << "DemoGame::~DemoGame" << std::endl;
	}

	virtual void STDCALL StartGame() {
		std::cout << "DemoGame::StartGame" << std::endl;
	}
};

class DemoGame_Activator : public IPluginActivator, IObjectListener
{
public:
	DemoGame_Activator() {}
	virtual ~DemoGame_Activator() {}

	virtual GPM_RES STDCALL Start(IPlugin* plugin) {
		std::cout << "DemoGame_Activator::Start" << std::endl;
		mGame = new DemoGame();
		plugin->AddObjectListener(this);
		plugin->RegisterObject(IID_Demo::IGame, mGame);
		return GPM_OK;
	}

	virtual void STDCALL Stop() {
		std::cout << "DemoGame_Activator::Stop" << std::endl;
		mGame->Release();
		mGame = nullptr;
	}

	virtual void STDCALL OnObjectChanged(GPM_TYPE type, IPluginObject* object, Status status) {
		if (status == STATUS_REGISTERED) {
			if (type == IID_Demo::IPlayer) {
				IPlayer* player = interface_cast<IPlayer>(object);  assert(player != nullptr);
				std::cout << "IPlayer is registered" << std::endl;
			}
			else if (type == IID_Demo::IComponent) {
				IComponent* component = interface_cast<IComponent>(object); assert(component != nullptr);
				std::cout << "IComponent is registered" << std::endl;
			}
		}
	}

private:
	DemoGame* mGame;
};

DEFINE_ACTIVATOR(DemoGame_Activator);
