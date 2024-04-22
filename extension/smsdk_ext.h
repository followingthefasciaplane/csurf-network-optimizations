// smsdk_ext.h

#ifndef _INCLUDE_SOURCEMOD_EXTENSION_BASESDK_H_
#define _INCLUDE_SOURCEMOD_EXTENSION_BASESDK_H_

#include "smsdk_config.h"
#include <IExtensionSys.h>
#include <IHandleSys.h>
#include <sp_vm_api.h>
#include <sm_platform.h>
#include <ISourceMod.h>
#if defined SMEXT_ENABLE_FORWARDSYS
#include <IForwardSys.h>
#endif // SMEXT_ENABLE_FORWARDSYS
#if defined SMEXT_ENABLE_PLAYERHELPERS
#include <IPlayerHelpers.h>
#endif // SMEXT_ENABLE_PLAYERHELPERS
#if defined SMEXT_ENABLE_DBMANAGER
#include <IDBDriver.h>
#endif // SMEXT_ENABLE_DBMANAGER
#if defined SMEXT_ENABLE_GAMECONF
#include <IGameConfigs.h>
#endif // SMEXT_ENABLE_GAMECONF
#if defined SMEXT_ENABLE_MEMUTILS
#include <IMemoryUtils.h>
#endif // SMEXT_ENABLE_MEMUTILS
#if defined SMEXT_ENABLE_GAMEHELPERS
#include <IGameHelpers.h>
#endif // SMEXT_ENABLE_GAMEHELPERS
#if defined SMEXT_ENABLE_TIMERSYS
#include <ITimerSystem.h>
#endif // SMEXT_ENABLE_TIMERSYS
#if defined SMEXT_ENABLE_ADTFACTORY
#include <IADTFactory.h>
#endif // SMEXT_ENABLE_ADTFACTORY
#if defined SMEXT_ENABLE_THREADER
#include <IThreader.h>
#endif // SMEXT_ENABLE_THREADER
#if defined SMEXT_ENABLE_LIBSYS
#include <ILibrarySys.h>
#endif // SMEXT_ENABLE_LIBSYS
#if defined SMEXT_ENABLE_PLUGINSYS
#include <IPluginSys.h>
#endif // SMEXT_ENABLE_PLUGINSYS
#if defined SMEXT_ENABLE_MENUS
#include <IMenuManager.h>
#endif // SMEXT_ENABLE_MENUS
#if defined SMEXT_ENABLE_ADMINSYS
#include <IAdminSystem.h>
#endif // SMEXT_ENABLE_ADMINSYS
#if defined SMEXT_ENABLE_TEXTPARSERS
#include <ITextParsers.h>
#endif // SMEXT_ENABLE_TEXTPARSERS
#if defined SMEXT_ENABLE_USERMSGS
#include <IUserMessages.h>
#endif // SMEXT_ENABLE_USERMSGS
#if defined SMEXT_ENABLE_TRANSLATOR
#include <ITranslator.h>
#endif // SMEXT_ENABLE_TRANSLATOR
#if defined SMEXT_ENABLE_ROOTCONSOLEMENU
#include <IRootConsoleMenu.h>
#endif // SMEXT_ENABLE_ROOTCONSOLEMENU

#if defined SMEXT_CONF_METAMOD
#include <ISmmPlugin.h>
#ifndef META_NO_HL2SDK
#include <eiface.h>
#endif // META_NO_HL2SDK
#endif // SMEXT_CONF_METAMOD

#if !defined METAMOD_PLAPI_VERSION
#include <metamod_wrappers.h>
#endif // METAMOD_PLAPI_VERSION

using namespace SourceMod;
using namespace SourcePawn;

class SDKExtension :
#if defined SMEXT_CONF_METAMOD
    public ISmmPlugin,
#endif
    public IExtensionInterface
{
public:
    virtual bool OnExtensionLoad(IExtension* me, IShareSys* sys, char* error, size_t maxlength, bool late) = 0;
    virtual void OnExtensionUnload() = 0;
    virtual void OnExtensionsAllLoaded() = 0;

#if defined SMEXT_CONF_METAMOD
    // ISmmPlugin
    virtual bool Load(PluginId id, ISmmAPI* ismm, char* error, size_t maxlength, bool late) = 0;
    virtual bool Unload(char* error, size_t maxlength) = 0;
    virtual bool Pause(char* error, size_t maxlength) = 0;
    virtual bool Unpause(char* error, size_t maxlength) = 0;
#endif
};

#endif // _INCLUDE_SOURCEMOD_EXTENSION_BASESDK_H_