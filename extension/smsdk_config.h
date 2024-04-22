#ifndef _INCLUDE_SOURCEMOD_EXTENSION_CONFIG_H_
#define _INCLUDE_SOURCEMOD_EXTENSION_CONFIG_H_

/**
 * @file smsdk_config.h
 * @brief contains macros for configuring SourceMod extension behavior
 */

 /* basic information exposed publicly */
#define SMEXT_CONF_NAME            "surf combat network opti"
#define SMEXT_CONF_DESCRIPTION     "optimizes network compression and lag compensation based on xutax kamay research"
#define SMEXT_CONF_VERSION         "1.0"
#define SMEXT_CONF_AUTHOR          "r kelly"
#define SMEXT_CONF_URL             "https://forums.alliedmods.net/showthread.php?p=2766789"
#define SMEXT_CONF_LOGTAG          "SURF_COMBAT"
#define SMEXT_CONF_CODENAME        "surf_combat"

/**
 * @brief expose functionality in the extension's public API
 */
#define SMEXT_ENABLE_FORWARDSYS    
 //#define SMEXT_ENABLE_PLAYERHELPERS 
#define SMEXT_ENABLE_TIMERSYS     
//#define SMEXT_ENABLE_MENUS         

/**
 * @brief interfaces to expose
 */
 //#define SMEXT_ENABLE_DBMANAGER     
 //#define SMEXT_ENABLE_GAMECONF      
#define SMEXT_ENABLE_GAMEHELPERS  
#define SMEXT_ENABLE_MEMUTILS      

/**
 * @brief core configurations
 */
#define SMEXT_CONF_METAMOD         /* enable Metamod:Source support */

/**
  * @brief Metamod:Source features
  */
#define SMEXT_ENABLE_FORWARDSYS    
#define SMEXT_ENABLE_HANDLESYS     
  //#define SMEXT_ENABLE_PLAYERHELPERS 
  //#define SMEXT_ENABLE_DBMANAGER     
  //#define SMEXT_ENABLE_GAMECONF      
#define SMEXT_ENABLE_MEMUTILS      
#define SMEXT_ENABLE_GAMEHELPERS   
#define SMEXT_ENABLE_TIMERSYS      
//#define SMEXT_ENABLE_ADTFACTORY    
//#define SMEXT_ENABLE_THREADER      
//#define SMEXT_ENABLE_LIBSYS        
//#define SMEXT_ENABLE_PLUGINSYS     
//#define SMEXT_ENABLE_MENUS         
//#define SMEXT_ENABLE_ADMINSYS      
//#define SMEXT_ENABLE_TEXTPARSERS   
//#define SMEXT_ENABLE_USERMSGS      
//#define SMEXT_ENABLE_TRANSLATOR    
//#define SMEXT_ENABLE_ROOTCONSOLEMENU 

#endif // _INCLUDE_SOURCEMOD_EXTENSION_CONFIG_H_#pragma once
