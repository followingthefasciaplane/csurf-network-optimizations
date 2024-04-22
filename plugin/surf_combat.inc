#if defined _surf_combat_included
    #endinput
#endif
#define _surf_combat_included

/**
 * natives
 */
native void SurfCombat_EnableExtension();
native void SurfCombat_DisableExtension();
native void SurfCombat_EnableOptimizations();
native void SurfCombat_DisableOptimizations();

/**
 * forwards
 */
forward void SurfCombat_OnExtensionEnabled();
forward void SurfCombat_OnExtensionDisabled();
forward void SurfCombat_OnOptimizationsEnabled();
forward void SurfCombat_OnOptimizationsDisabled();

/**
 * methodmaps
 */
methodmap SurfCombatPlayer __nullable__ < Player {
    public SurfCombatPlayer(int index) {
        return view_as<SurfCombatPlayer>(index);
    }

    property bool IsExtensionEnabled {
        public get() {
            return SurfCombat_IsExtensionEnabled(this.index);
        }
    }

    property bool AreOptimizationsEnabled {
        public get() {
            return SurfCombat_AreOptimizationsEnabled(this.index);
        }
    }
}

/**
 * natives - leave this stuff alone
 */
public Extension __ext_surf_combat =
{
    name = "surf combat network opti",
    file = "surf_combat.ext",
#if defined AUTOLOAD_EXTENSIONS
    autoLoad = 1,
#else
    autoLoad = 0,
#endif
#if defined REQUIRE_EXTENSIONS
    required = 1,
#else
    required = 0,
#endif
};

#if !defined REQUIRE_EXTENSIONS
    public __ext_surf_combat_SetNTVOptional()
    {
        MarkNativeAsOptional("SurfCombat_EnableExtension");
        MarkNativeAsOptional("SurfCombat_DisableExtension");
        MarkNativeAsOptional("SurfCombat_EnableOptimizations");
        MarkNativeAsOptional("SurfCombat_DisableOptimizations");
        MarkNativeAsOptional("SurfCombat_IsExtensionEnabled");
        MarkNativeAsOptional("SurfCombat_AreOptimizationsEnabled");
    }
#endif