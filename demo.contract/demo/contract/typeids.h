#pragma once
#include <gpm/contract.h>

//
// Plugin types
//

struct IID_Demo {
	enum Enum {
		IPlayer = IID_GamePluginModel::Next,
		IComponent,
		IGame,

		// Next free ID
		Next
	};
};
