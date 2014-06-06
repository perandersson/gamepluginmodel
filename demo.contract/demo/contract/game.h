#pragma once
#include <gpm/contract.h>
#include "player.h"

struct PLUGIN_API IGame : public TPluginInterface<IID_Demo::IGame>
{
	virtual STDCALL ~IGame() {}

	virtual void STDCALL StartGame() = 0;
};
