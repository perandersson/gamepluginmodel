#pragma once
#include "gpm/contract.h"
#include "objectreference.h"
#include "plugincontext.h"
#include <vector>
#include <list>
#include <memory>

/**
*
*/
class Plugin : public IPlugin
{
	typedef std::vector<std::shared_ptr<ObjectReference>> References;
	typedef std::list<IObjectListener*> ObjectListeners;

public:
	Plugin(PluginContext* pluginContext);
	virtual ~Plugin();

	/**
	*	Retrieves the first plugin matching the supplied type
	*
	*	@param type
	*	@return
	*/
	IPluginObject* FindObjectByType(GPM_TYPE type) const;

	/**
	*	Unload all the registered objects in this plugin
	*/
	void UnloadObjects();

	/**
	*	Notify all the object listeners added to this plugin
	*/
	void NotifyObjectListeners(GPM_TYPE type, IPluginObject* object, IObjectListener::Status status);

public:
	virtual Status STDCALL GetStatus() const;
	virtual void STDCALL RegisterObject(GPM_TYPE type, IPluginObject* object);
	virtual void STDCALL UnregisterObject(IPluginObject* object);
	virtual void STDCALL AddObjectListener(IObjectListener* listener);
	virtual void STDCALL AddObjectListener(IObjectListener* listener, const char* filter);
	virtual void STDCALL RemoveObjectListener(IObjectListener* listener); 
	virtual IPluginObject* STDCALL GetObject(GPM_TYPE type);
	virtual IPluginObject* STDCALL GetObject(GPM_TYPE type, const char* filter);
	virtual GPM_UINT32 STDCALL GetObjects(GPM_TYPE type, IPluginObject** _out_Objects, GPM_UINT32 objectsSize);

private:
	PluginContext* mPluginContext;
	Status mStatus;
	References mReferences;
	ObjectListeners mObjectListeners;
};

