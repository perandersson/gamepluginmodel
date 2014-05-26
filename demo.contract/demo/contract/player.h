#pragma once
#include <gpm/contract.h>
#include "typeids.h"
#include "component.h"
#include "vector2.h"

struct PLUGIN_API IPlayer : public TPluginInterface<IID_Demo::IPlayer>
{
	virtual STDCALL ~IPlayer() {}

	virtual void STDCALL AddComponent(IComponent* component) = 0;

	virtual void STDCALL RemoveComponent(IComponent* component) = 0;

	virtual void STDCALL SetPosition(const VECTOR2* position) = 0;

	virtual const PVECTOR2 STDCALL GetPosition() = 0;
};
