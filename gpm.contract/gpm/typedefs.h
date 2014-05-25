#pragma once

#ifdef PLUGIN_EXPORTS

#ifdef _MSC_VER
#define PLUGIN_API __declspec(dllexport)
#elif __GNUC__
#define PLUGIN_API __attribute__ ((dllexport))
#else
#error Unknown Compiler
#endif
#else

#ifdef _MSC_VER
#define PLUGIN_API __declspec(dllimport)
#elif __GNUC__
#define PLUGIN_API __attribute__ ((dllimport))
#else
#error Unknown Compiler
#endif

#endif

#ifdef _MSC_VER
#define STDCALL __stdcall
#define CDECL __cdecl
#elif __GNUC__
#define STDCALL __attribute__((stdcall))
#define CDECL __attribute__((cdecl))
#else
#error Unknown Compiler
#endif

//
// Built-in types
//

// A game plugin model type
typedef long long GPM_TYPE;

// A result returned by the plugin framework's interface.
typedef int GPM_RES;

// An error code delivered by the plugin framework
typedef int GPM_ERRCODE;

// Symbolizes an unsigned long in the plugin framework
typedef unsigned long GPM_UINT64;

// Symbolizes an unsigned int in the plugin framework
typedef unsigned int GPM_UINT32;

#define GPM_OK 1
#define GPM_ERR 0

//
// Declare a new interface useful for the plugin framework.
//
// @param Name
//			The interface name
// @param Inherits
//			The class that the new interface inherits from. If nothing then specify IPluginObject
#define DECLARE_INTERFACE(Name, Inherits) struct PLUGIN_API Name : public Inherits

//
// Releases the supplied object, if it's not null
#define SAFE_RELEASE(Object) if(Object != nullptr) Object->Release();