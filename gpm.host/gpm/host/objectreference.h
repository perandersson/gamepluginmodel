#pragma once
#include "gpm/contract.h"

class ObjectReference
{
public:
	ObjectReference(GPM_TYPE type, IPluginObject* object);
	~ObjectReference();

	GPM_TYPE GetType() const;
	IPluginObject* GetObject() const;
	bool IsType(GPM_TYPE type) const;

private:
	GPM_TYPE mType;
	IPluginObject* mObject;
};