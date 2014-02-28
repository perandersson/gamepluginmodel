#include "demo/contract.h"
#include <iostream>
#include <cassert>

class DemoGame : public IGame
{
public:
	DemoGame() : mRefCount(1) {
		std::cout << "DemoGame::DemoGame" << std::endl;
	}

	virtual ~DemoGame() {
		std::cout << "DemoGame::~DemoGame" << std::endl;
	}

	virtual void StartGame() {
		std::cout << "DemoGame::StartGame" << std::endl;
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
		if (type == GPM_TYPEOF(IGame)) {
			*_out_Ptr = static_cast<IGame*>(this);
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

class DemoGame_Activator : public IPluginActivator, IObjectListener
{
public:
	DemoGame_Activator() {}
	virtual ~DemoGame_Activator() {}

	virtual GPM_RES STDCALL Start(IPlugin* plugin) {
		std::cout << "DemoGame_Activator::Start" << std::endl;
		mGame = new DemoGame();
		plugin->AddObjectListener(this);
		plugin->RegisterObject(GPM_TYPEOF(IGame), mGame);
		return GPM_OK;
	}

	virtual void STDCALL Stop() {
		std::cout << "DemoGame_Activator::Stop" << std::endl;
		mGame->Release();
		mGame = nullptr;
	}

	virtual void STDCALL OnObjectChanged(GPM_TYPE type, IPluginObject* object, Status status) {
		if (status == Status::STATUS_REGISTERED) {
			if (type == GPM_TYPEOF(IPlayer)) {
				IPlayer* player;
				GPM_RES res = object->ToInterface(GPM_TYPEOF(IPlayer), (IPluginObject**)&player); assert(res == GPM_OK);
				player->Release();
				std::cout << "IPlayer is registered" << std::endl;
			}
			else if (type == GPM_TYPEOF(IComponent)) {
				IComponent* component;
				GPM_RES res = object->ToInterface(GPM_TYPEOF(IComponent), (IPluginObject**)&component); assert(res == GPM_OK);
				component->Release();
				std::cout << "IComponent is registered" << std::endl;
			}
		}
	}

private:
	DemoGame* mGame;
};

DEFINE_ACTIVATOR(DemoGame_Activator);
