/**
*	This is the host application for the demo sample. 
*/

#include <iostream>
#include "demo/contract.h"
#include <gpm/host/plugincontext.h>
#include <algorithm>
#include <cassert>

class DemoPlayer : public IPlayer
{
public:
	DemoPlayer() : mRefCount(1) {

	}
	
	virtual ~DemoPlayer() {

	}
	
	virtual void AddComponent(IComponent* component) {
		component->AddRef();
		mComponents.push_back(component);
		component->OnComponentAdded(this);
	}

	virtual void RemoveComponent(IComponent* component) {
		std::vector<IComponent*>::iterator it = std::find(mComponents.begin(), mComponents.end(), component);
		if (it == mComponents.end())
			return;

		component->OnComponentRemoved(this);
		mComponents.erase(it);
		component->Release();
	}

	virtual void SetPosition(const VECTOR2& position) {
		mPosition = position;
	}

	virtual void GetPosition(PVECTOR2 _out_Position) {
		*_out_Position = mPosition;
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
		if (type == GPM_TYPEOF(IPlayer)) {
			*_out_Ptr = static_cast<IPlayer*>(this);
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
	VECTOR2 mPosition;
	std::vector<IComponent*> mComponents;
	GPM_UINT64 mRefCount;
};

int main()
{
	// Create a plugin context instance
	PluginContext context;

	// Register host-bound objects. Although the interface requires AddRef and Release, it's not really necessary to implement them
	// since we don't have to keep track of the memory between DLL's for these objects. The dynamic loaded libraries are, most likely,
	// unloaded before the scope of the host- bound objects is unloaded.
	DemoPlayer player;
	context.RegisterGlobalObject(GPM_TYPEOF(IPlayer), &player);

	// Load libraries
#ifdef _DEBUG
	context.LoadLibrary("demo.game_d");
	context.LoadLibrary("demoa_d");
	context.LoadLibrary("demob_d");
#else
	context.LoadLibrary("demo.game");
	context.LoadLibrary("demoa");
	context.LoadLibrary("demob");
#endif

	auto object = context.GetObject(GPM_TYPEOF(IGame));
	if (object != nullptr) {
		IGame* game;
		GPM_RES res = object->ToInterface(GPM_TYPEOF(IGame), (IPluginObject**)&game); assert(res == GPM_OK);
		object->Release();
		game->StartGame();
		game->Release();
	}

	// Unload libraries
	context.UnloadLibraries();

	std::cout << "Press any key to continue" << std::endl;
	std::cin.get();
	

	return 0;
}
