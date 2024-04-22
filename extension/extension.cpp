#include "extension.h"
#include "CBasePlayer.h"
#include "networkvar.h"
#include "engine/iserverplugin.h"
#include "engine/IEngineTrace.h"
#include "toolframework/itoolentity.h"
#include <unordered_set>
#include <string>

using namespace SourceMod;
using namespace SourcePawn;

SurfCombatExtension g_SurfCombatExtension;
SMEXT_LINK(&g_SurfCombatExtension);

IServerGameDLL* gameserver = nullptr;
IVEngineServer* engine = nullptr;

bool g_bExtensionEnabled = false;
bool g_bOptimizationsEnabled = false;

std::unordered_set<std::string> criticalNetVars = {
    "m_vecOrigin", "m_vecOrigin[0]", "m_vecOrigin[1]", "m_vecOrigin[2]",
    "m_angRotation", "m_angRotation[0]", "m_angRotation[1]", "m_angRotation[2]",
    "m_flPoseParameter", "m_flPoseParameter[0]", "m_flPoseParameter[1]", "m_flPoseParameter[2]"
};

bool IsCriticalNetworkVar(const SendProp* pProp) {
    return criticalNetVars.find(pProp->GetName()) != criticalNetVars.end();
}

void SendPropVector_Hook(const SendProp* pProp, const void* pStruct, const void* pData, DVariant* pOut, int iElement, int objectID) {
    if (IsCriticalNetworkVar(pProp)) {
        // directly assign vector data to output without compression
        pOut->m_Vector[0] = ((float*)pData)[0];
        pOut->m_Vector[1] = ((float*)pData)[1];
        pOut->m_Vector[2] = ((float*)pData)[2];
    }
    else {
        // default behavior for non-critical properties
        SendPropVector(pProp, pStruct, pData, pOut, iElement, objectID);
    }
}

void SetBaseVelocity_Hook(CBasePlayer* pPlayer, const Vector& vecBaseVelocity) {
    pPlayer->m_vecBaseVelocity = vecBaseVelocity;  // set base velocity
    pPlayer->NetworkStateChanged();                // notify the network state change
}

bool IsValidTeleportation(CBasePlayer* pPlayer) {
    // retrieve the current and old positions of the player
    const Vector& currentOrigin = pPlayer->GetAbsOrigin();
    const Vector& oldOrigin = pPlayer->GetOldOrigin();

    // calculate the distance between the current and previous positions
    float distance = currentOrigin.DistTo(oldOrigin);

    // define a threshold for what is considered a valid teleportation distance
    const float kTeleportationThreshold = 1000.0f; //placeholder

    // is the distance <= the threshold?
    if (distance <= kTeleportationThreshold) {
        return true;
    }

    // setup a ray from the old position to the current position to check for solids
    trace_t trace;
    Ray_t ray;
    ray.Init(oldOrigin, currentOrigin);
    CTraceFilterSimple filter(pPlayer, COLLISION_GROUP_NONE); // ignore ourselves

    // are there any solids between us and the new position?
    enginetrace->TraceRay(ray, MASK_SOLID, &filter, &trace);

    // yes, invalid teleport
    if (trace.fraction < 1.0f) {
        return false;
    }

    // are we inside a solid object maybe?
    enginetrace->TraceRay(ray, MASK_SOLID, &filter, &trace);

    // yes, invalid teleport
    if (trace.startsolid || trace.allsolid) {
        return false;
    }

    // if we made it here, everything is happy
    return true;
}


bool LagCompensation_Hook(CBasePlayer* pPlayer, float flCurrentTime) {
    if (IsValidTeleportation(pPlayer)) {
        // get the player's current position and angles
        const Vector& currentOrigin = pPlayer->GetAbsOrigin();
        const QAngle& currentAngles = pPlayer->GetAbsAngles();

        // calculate the player's interpolated position and angles
        Vector interpolatedOrigin;
        QAngle interpolatedAngles;
        gameserver->GetPlayerInterpolatedPosition(pPlayer, flCurrentTime, interpolatedOrigin, interpolatedAngles);

        // calculate the difference between the current and interpolated positions and angles
        Vector deltaOrigin = currentOrigin - interpolatedOrigin;
        QAngle deltaAngles = currentAngles - interpolatedAngles;

        // apply the position and angle adjustments to the player
        pPlayer->SetAbsOrigin(currentOrigin + deltaOrigin);
        pPlayer->SetAbsAngles(currentAngles + deltaAngles);

        // update the player's simulation time
        gameserver->SetPlayerSimulationTime(pPlayer, flCurrentTime);

        return true;
    }
    else {
        // fallback to default lag compensation
        return LagCompensation(pPlayer, flCurrentTime);
    }
}

bool SurfCombatExtension::SDK_OnLoad(char* error, size_t maxlen, bool late) {
    gameserver = gameServerFactory();
    if (!gameserver) {
        snprintf(error, maxlen, "failed to get game server interface");
        return false;
    }

    engine = (IVEngineServer*)g_SMAPI->GetEngineFactory()("VEngineServer023", nullptr);
    if (!engine) {
        snprintf(error, maxlen, "failed to get engine interface");
        return false;
    }

    // hooks
    SH_ADD_HOOK_MEMFUNC(IServerGameDLL, SendPropVector, gameserver, this, &SurfCombatExtension::SendPropVector_Hook, true);
    SH_ADD_HOOK_MEMFUNC(IServerGameDLL, SetBaseVelocity, gameserver, this, &SurfCombatExtension::SetBaseVelocity_Hook, true);
    SH_ADD_HOOK_MEMFUNC(IServerGameDLL, LagCompensation, gameserver, this, &SurfCombatExtension::LagCompensation_Hook, true);

    // natives
    sharesys->AddNatives(myself, g_ExtensionNatives);

    return true;
}

void SurfCombatExtension::SDK_OnUnload() {
    // remove hooks
    SH_REMOVE_HOOK_MEMFUNC(IServerGameDLL, SendPropVector, gameserver, this, &SurfCombatExtension::SendPropVector_Hook, true);
    SH_REMOVE_HOOK_MEMFUNC(IServerGameDLL, SetBaseVelocity, gameserver, this, &SurfCombatExtension::SetBaseVelocity_Hook, true);
    SH_REMOVE_HOOK_MEMFUNC(IServerGameDLL, LagCompensation, gameserver, this, &SurfCombatExtension::LagCompensation_Hook, true);
}

void SurfCombatExtension::SDK_OnAllLoaded() {
    sharesys->RegisterLibrary(myself, "SurfCombat");
    g_pSM->LogMessage(myself, "network optimization is now fully operational.");
}

void SurfCombatExtension::SDK_OnPauseChange(bool paused) {
    if (paused) {
        g_pSM->LogMessage(myself, "network optimization extension has been paused.");
        DisableOptimizations();
    }
    else {
        g_pSM->LogMessage(myself, "network optimization extension has been unpaused.");
        EnableOptimizations();
    }
}

bool SurfCombatExtension::QueryRunning(char* error, size_t maxlen) {
    return true;
}

void SurfCombatExtension::DisableOptimizations() {
    // unhooks
    SH_REMOVE_HOOK_MEMFUNC(IServerGameDLL, SendPropVector, gameserver, this, &SurfCombatExtension::SendPropVector_Hook, true);
    SH_REMOVE_HOOK_MEMFUNC(IServerGameDLL, SetBaseVelocity, gameserver, this, &SurfCombatExtension::SetBaseVelocity_Hook, true);
    SH_REMOVE_HOOK_MEMFUNC(IServerGameDLL, LagCompensation, gameserver, this, &SurfCombatExtension::LagCompensation_Hook, true);

    g_bOptimizationsEnabled = false;
    g_pSM->LogMessage(myself, "network optimizations have been disabled.");
}

void SurfCombatExtension::EnableOptimizations() {
    // hooks
    SH_ADD_HOOK_MEMFUNC(IServerGameDLL, SendPropVector, gameserver, this, &SurfCombatExtension::SendPropVector_Hook, true);
    SH_ADD_HOOK_MEMFUNC(IServerGameDLL, SetBaseVelocity, gameserver, this, &SurfCombatExtension::SetBaseVelocity_Hook, true);
    SH_ADD_HOOK_MEMFUNC(IServerGameDLL, LagCompensation, gameserver, this, &SurfCombatExtension::LagCompensation_Hook, true);

    g_bOptimizationsEnabled = true;
    g_pSM->LogMessage(myself, "network optimizations have been enabled.");
}

void SurfCombatExtension::EnableExtension() {
    g_bExtensionEnabled = true;
    g_pSM->LogMessage(myself, "network optimization extension has been enabled.");
    g_forwardSys.FireOnce(FF_SurfCombat_OnExtensionEnabled);
}

void SurfCombatExtension::DisableExtension() {
    g_bExtensionEnabled = false;
    g_pSM->LogMessage(myself, "network optimization extension has been disabled.");
    g_forwardSys.FireOnce(FF_SurfCombat_OnExtensionDisabled);
}

bool SurfCombatExtension::IsExtensionEnabled(int client) {
    return g_bExtensionEnabled;
}

bool SurfCombatExtension::AreOptimizationsEnabled(int client) {
    return g_bOptimizationsEnabled;
}

#if defined SMEXT_CONF_METAMOD
bool SurfCombatExtension::SDK_OnMetamodLoad(ISmmAPI* ismm, char* error, size_t maxlen, bool late) {
    return true;
}

bool SurfCombatExtension::SDK_OnMetamodUnload(char* error, size_t maxlen) {
    return true;
}

bool SurfCombatExtension::SDK_OnMetamodPauseChange(bool paused, char* error, size_t maxlen) {
    return true;
}
#endif

// extension natives
SN_NATIVE_FUNC(SurfCombat_EnableExtension) {
    g_SurfCombatExtension.EnableExtension();
    return 0;
}

SN_NATIVE_FUNC(SurfCombat_DisableExtension) {
    g_SurfCombatExtension.DisableExtension();
    return 0;
}

SN_NATIVE_FUNC(SurfCombat_EnableOptimizations) {
    g_SurfCombatExtension.EnableOptimizations();
    return 0;
}

SN_NATIVE_FUNC(SurfCombat_DisableOptimizations) {
    g_SurfCombatExtension.DisableOptimizations();
    return 0;
}

SN_NATIVE_FUNC(SurfCombat_IsExtensionEnabled) {
    int client = SourcePawn::SpawnedEntIsTaken(hPlugin, hParams);
    return g_SurfCombatExtension.IsExtensionEnabled(client);
}

SN_NATIVE_FUNC(SurfCombat_AreOptimizationsEnabled) {
    int client = SourcePawn::SpawnedEntIsTaken(hPlugin, hParams);
    return g_SurfCombatExtension.AreOptimizationsEnabled(client);
}

const sp_nativeinfo_t g_ExtensionNatives[] = {
    {"SurfCombat_EnableExtension", SurfCombat_EnableExtension},
    {"SurfCombat_DisableExtension", SurfCombat_DisableExtension},
    {"SurfCombat_EnableOptimizations", SurfCombat_EnableOptimizations},
    {"SurfCombat_DisableOptimizations", SurfCombat_DisableOptimizations},
    {"SurfCombat_IsExtensionEnabled", SurfCombat_IsExtensionEnabled},
    {"SurfCombat_AreOptimizationsEnabled", SurfCombat_AreOptimizationsEnabled},
    {nullptr, nullptr}
};