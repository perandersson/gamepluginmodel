#pragma once
#include <gpm/contract.h>

struct PLUGIN_API IPlayer;

DECLARE_INTERFACE(IComponent, IPluginObject)
{
	virtual void OnComponentAdded(IPlayer* player) = 0;

	virtual void OnComponentRemoved(IPlayer* player) = 0;
};
