// extension.h

#ifndef _INCLUDE_SOURCEMOD_EXTENSION_PROPER_H_
#define _INCLUDE_SOURCEMOD_EXTENSION_PROPER_H_

#include "smsdk_ext.h"

class SurfCombatExtension : public SDKExtension {
public:
    virtual bool SDK_OnLoad(char* error, size_t maxlen, bool late);
    virtual void SDK_OnUnload();
    virtual void SDK_OnAllLoaded();
    virtual void SDK_OnPauseChange(bool paused);
    virtual bool QueryRunning(char* error, size_t maxlen);

#if defined SMEXT_CONF_METAMOD
    virtual bool SDK_OnMetamodLoad(ISmmAPI* ismm, char* error, size_t maxlen, bool late);
    virtual bool SDK_OnMetamodUnload(char* error, size_t maxlen);
    virtual bool SDK_OnMetamodPauseChange(bool paused, char* error, size_t maxlen);
#endif

private:
    void DisableOptimizations();
    void EnableOptimizations();
    void EnableExtension();
    void DisableExtension();

    void SendPropVector_Hook(const SendProp* pProp, const void* pStruct, const void* pData, DVariant* pOut, int iElement, int objectID);
    void SetBaseVelocity_Hook(CBasePlayer* pPlayer, const Vector& vecBaseVelocity);
    bool LagCompensation_Hook(CBasePlayer* pPlayer, float flCurrentTime);
    bool IsExtensionEnabled(int client);
    bool AreOptimizationsEnabled(int client);
};

#endif // _INCLUDE_SOURCEMOD_EXTENSION_PROPER_H_