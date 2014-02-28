#pragma once
#include <gpm/contract.h>
#include "player.h"

DECLARE_INTERFACE(IGame, IPluginObject)
{
	virtual void StartGame() = 0;
};
