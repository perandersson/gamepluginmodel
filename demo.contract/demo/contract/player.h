#pragma once
#include <gpm/contract.h>
#include "component.h"
#include "vector2.h"

DECLARE_INTERFACE(IPlayer, IPluginObject)
{
	virtual void AddComponent(IComponent* component) = 0;

	virtual void RemoveComponent(IComponent* component) = 0;

	virtual void SetPosition(const VECTOR2& position) = 0;

	virtual void GetPosition(PVECTOR2 _out_Position) = 0;
};
