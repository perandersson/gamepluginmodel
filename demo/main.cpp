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
	DemoPlayer() {

	}
	
	virtual ~DemoPlayer() {

	}
	
	virtual void STDCALL AddComponent(IComponent* component) {
		component->AddRef();
		mComponents.push_back(component);
		component->OnComponentAdded(this);
	}

	virtual void STDCALL RemoveComponent(IComponent* component) {
		std::vector<IComponent*>::iterator it = std::find(mComponents.begin(), mComponents.end(), component);
		if (it == mComponents.end())
			return;

		component->OnComponentRemoved(this);
		mComponents.erase(it);
		component->Release();
	}

	virtual void STDCALL SetPosition(const VECTOR2& position) {
		mPosition = position;
	}

	virtual const PVECTOR2 STDCALL GetPosition() {
		return &mPosition;
	}

private:
	VECTOR2 mPosition;
	std::vector<IComponent*> mComponents;
};

int main()
{
	// Create a plugin context instance
	PluginContext context;

	// Register host-bound objects. Although the interface requires AddRef and Release, it's not really necessary to implement them
	// since we don't have to keep track of the memory between DLL's for these objects. The dynamic loaded libraries are, most likely,
	// unloaded before the scope of the host- bound objects is unloaded.
	DemoPlayer player;
	context.RegisterGlobalObject(IID_Demo::IPlayer, &player);

	// Load libraries
#ifdef _DEBUG
	context.LoadPlugin("demo.game_d");
	context.LoadPlugin("demoa_d");
	context.LoadPlugin("demob_d");
#else
	context.LoadPlugin("demo.game");
	context.LoadPlugin("demoa");
	context.LoadPlugin("demob");
#endif

	auto object = context.GetObject(IID_Demo::IGame);
	if (object != nullptr) {
		IGame* game = object->GetInterface<IGame>(); assert(game != nullptr);
		game->StartGame();
		game->Release();
	}

	// Unload libraries
	context.UnloadPlugins();

	std::cout << "Press any key to continue" << std::endl;
	std::cin.get();
	

	return 0;
}
