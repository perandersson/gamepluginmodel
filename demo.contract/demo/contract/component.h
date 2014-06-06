#pragma once
#include <gpm/contract.h>
#include "typeids.h"

struct PLUGIN_API IPlayer;

struct PLUGIN_API IComponent : public TPluginInterface<IID_Demo::IComponent, IPluginObject>
{
	virtual STDCALL ~IComponent() {}

	virtual void STDCALL OnComponentAdded(IPlayer* player) = 0;

	virtual void STDCALL OnComponentRemoved(IPlayer* player) = 0;
};
