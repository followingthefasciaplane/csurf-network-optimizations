// surf_combat.sp

#include <sourcemod>
#include <sdktools>
#include <surf_combat>
#include <AutoExecConfig>

#pragma semicolon 1
#pragma newdecls required

ConVar g_CvarEnablePlugin;
ConVar g_CvarEnableOptimizations;

public Plugin myinfo = {
    name = "surf combat network opti",
    author = "r kelly",
    description = "optimizes network compression and lag compensation based on xutax kamay research",
    version = "1.0",
    url = "https://forums.alliedmods.net/showthread.php?p=2766789"
};

public void OnPluginStart() {
    // cvars
    AutoExecConfig_SetFile("surf_combat");
    AutoExecConfig_SetCreateFile(true);

    g_CvarEnablePlugin = AutoExecConfig_CreateConVar("sm_surf_combat_enable", "1", "enable the network optimization plugin");
    g_CvarEnableOptimizations = AutoExecConfig_CreateConVar("sm_surf_combat_optimizations", "1", "enable network optimizations");

    AutoExecConfig_ExecuteFile();
    AutoExecConfig_CleanFile();

    // commands
    RegConsoleCmd("sm_surf_combat_status", Command_SurfCombatStatus);

    // forwards
    AddForwardToCoreOnce(FF_SurfCombat_OnExtensionEnabled, OnSurfCombatExtensionEnabled);
    AddForwardToCoreOnce(FF_SurfCombat_OnExtensionDisabled, OnSurfCombatExtensionDisabled);
    AddForwardToCoreOnce(FF_SurfCombat_OnOptimizationsEnabled, OnSurfCombatOptimizationsEnabled);
    AddForwardToCoreOnce(FF_SurfCombat_OnOptimizationsDisabled, OnSurfCombatOptimizationsDisabled);
}

public Action Command_SurfCombatStatus(int client, int args) {
    // current status
    ReplyToCommand(client, "network optimization status:");
    ReplyToCommand(client, "plugin enabled: %s", g_CvarEnablePlugin.BoolValue ? "yes" : "no");
    ReplyToCommand(client, "optimizations enabled: %s", g_CvarEnableOptimizations.BoolValue ? "yes" : "no");
    
    return Plugin_Handled;
}

public void OnConfigsExecuted() {
    if (g_CvarEnablePlugin.BoolValue) {
        EnableExtension();
    } else {
        DisableExtension();
    }
}

void EnableExtension() {
    SurfCombat_EnableExtension();
    
    if (g_CvarEnableOptimizations.BoolValue) {
        SurfCombat_EnableOptimizations();
    }
}

void DisableExtension() {
    SurfCombat_DisableExtension();
}

public void OnPluginEnd() {
    DisableExtension();
}

// forward callbacks
public void OnSurfCombatExtensionEnabled() {
    PrintToServer("network opti extension has been enabled.");
}

public void OnSurfCombatExtensionDisabled() {
    PrintToServer("network opti extension has been disabled.");
}

public void OnSurfCombatOptimizationsEnabled() {
    PrintToServer("network optimizations have been enabled.");
}

public void OnSurfCombatOptimizationsDisabled() {
    PrintToServer("network optimizations have been disabled.");
}