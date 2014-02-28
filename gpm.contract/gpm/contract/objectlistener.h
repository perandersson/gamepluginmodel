#pragma once
#include "pluginobject.h"

/**
*	Interface used when the plugin developer wants to know whenever new objects are
*	exposed from the plugin framework.
*
*	The implementation is bound to the dynamic libaries plugin only, which means that
*	the listener is automatically unregistered during the plugin's SHUTDOWN phase.
*/
struct PLUGIN_API IObjectListener
{
	enum PLUGIN_API Status {
		/**
		*	Status if a service is registered
		*/
		STATUS_REGISTERED = 0,

		/**
		*	Status if a service is unregistered
		*/
		STATUS_UNREGISTERED = 1
	};

	virtual STDCALL ~IObjectListener() {}

	/**
	*	Notifies when the status a service is changed within the plugin framework
	*
	*	@param type
	*			The object type
	*	@param object
	*			The object that changed it's status. Remember to call {@code IPluginObject::AddRef} if you're planning
	*			on saving the pointer of the object and {@code IPluginObject::Release} whenever the object is no longer
	*			in use. 
	*	@param status
	*			The objects new status
	*/
	virtual void STDCALL OnObjectChanged(GPM_TYPE type, IPluginObject* object, Status status) = 0;
};

