#include "objectreference.h"

ObjectReference::ObjectReference(GPM_TYPE type, IPluginObject* object)
:mType(type), mObject(object)
{
	object->AddRef();
}

ObjectReference::~ObjectReference()
{
	mObject->Release();
}

GPM_TYPE ObjectReference::GetType() const
{
	return mType;
}

IPluginObject* ObjectReference::GetObject() const
{
	return mObject;
}

bool ObjectReference::IsType(GPM_TYPE type) const
{
	return type == mType;
}
