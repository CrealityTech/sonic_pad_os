/*
 * Config the features of startup demo programs.
 */

#ifndef __CONFIG_H__
#define __CONFIG_H__

// Please put the defines shared by makefile projects and AndeSight projects
#if 0
#define __NDS32		1
#define __nds32		1
#define __NDS32_AB2_2	1
#define __NDS32_AB2_2__	1
#define __NDS32_BASELINE_V3	1
#define __NDS32_BASELINE_V3__	1
#define __NDS32_CMOV__    1
#define __NDS32_EL    1
#define __NDS32_EL__    1
#define __NDS32_ISA_V3__   1
#define __NDS32_EXT_EX9    1
#define __NDS32_EXT_EX9__  1
#define __NDS32_EXT_IFC    1
#define __NDS32_EXT_IFC__  1
#define __NDS32_EXT_PERF		1
#define __NDS32_EXT_PERF2		1
#define __NDS32_EXT_PERF2__		1
#define __NDS32_EXT_PERF__		1
#define __NDS32_GP_DIRECT__		1
#define __NDS32_ISA_VECTOR_SIZE_4__   1
#endif

#ifndef	CFG_MAKEFILE

// The defines are only used by AndeSight projects

//----------------------------------------------------------------------------------------------------

// Users can configure the defines in this area
// to match different environment setting


//#define	CFG_16MB		// platform is 16MB, if it isn't defined, platform is 4GB

#define BUILD_MODE	BUILD_LOAD

//#define CFG_EVIC
//----------------------------------------------------------------------------------------------------
// The followings are predefined settings
// Please do not modify them

#define	BUILD_LOAD	1		// The program is loaded by GDB or eBIOS
#define	BUILD_BURN	2		// The program is burned to the flash, but run in RAM
							// demo-ls2 use BURN mode
#define	BUILD_XIP	3		// The program is burned to the flash and run in the flash
							// To use this mode, xip linker script files must be used
							// demo-ls1 use XIP mode
#define	BUILD_SIMU	4		// The program is run in the simulation environment

#ifdef DEMO_LS1
	// demo-ls1 needs to use the XIP mode
	#undef BUILD_MODE
	#define BUILD_MODE	BUILD_XIP
#endif

#ifdef DEMO_LS2
    // demo-ls2 needs to use the BURN mode
    #undef BUILD_MODE
    #define BUILD_MODE  BUILD_BURN
#endif

#ifndef NDS32_EXT_EX9
	// This toolchain cannot support EX9
	#define CONFIG_NO_NDS32_EXT_EX9
#endif

#if BUILD_MODE == BUILD_BURN
	// Burn mode
	#define CFG_LLINIT      // do low level init
	#define CFG_REMAP       // do remap
#elif BUILD_MODE == BUILD_XIP
	// XIP mode
	#define CFG_LLINIT      // do low level init
#elif BUILD_MODE == BUILD_SIMU
	// Simu mode
	#define	CFG_SIMU
	#define CFG_LLINIT      // do low level init
#else
	// Load mode
#endif

#ifdef NDS32_BASELINE_V3M
	// v3m toolchain only support 16MB
	#undef CFG_16MB
	#define CFG_16MB
#endif

#endif // CFG_MAKEFILE

#endif // __CONFIG_H__
