#pragma once
#include "contract/objectlistener.h"
#include "contract/plugin.h"
#include "contract/pluginactivator.h"
#include "contract/plugincontext.h"
#include "contract/pluginobject.h"
#include "contract/logger.h"

struct IID_GamePluginModel {
	enum Enum {
		IPluginObject = 1,

		// Next free ID
		Next
	};
};
