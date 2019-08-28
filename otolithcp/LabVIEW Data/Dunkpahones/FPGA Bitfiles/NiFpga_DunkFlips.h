/*
 * Generated with the FPGA Interface C API Generator 15.0.0
 * for NI-RIO 15.0.0 or later.
 */

#ifndef __NiFpga_DunkFlips_h__
#define __NiFpga_DunkFlips_h__

#ifndef NiFpga_Version
   #define NiFpga_Version 1500
#endif

#include "NiFpga.h"

/**
 * The filename of the FPGA bitfile.
 *
 * This is a #define to allow for string literal concatenation. For example:
 *
 *    static const char* const Bitfile = "C:\\" NiFpga_DunkFlips_Bitfile;
 */
#define NiFpga_DunkFlips_Bitfile "NiFpga_DunkFlips.lvbitx"

/**
 * The signature of the FPGA bitfile.
 */
static const char* const NiFpga_DunkFlips_Signature = "69310815E550D6D5B8486B5E909077E6";

typedef enum
{
   NiFpga_DunkFlips_IndicatorI32_Output1 = 0x3C,
   NiFpga_DunkFlips_IndicatorI32_Output2 = 0x48,
   NiFpga_DunkFlips_IndicatorI32_Output3 = 0x18,
   NiFpga_DunkFlips_IndicatorI32_Output4 = 0x10,
} NiFpga_DunkFlips_IndicatorI32;

typedef enum
{
   NiFpga_DunkFlips_IndicatorU32_ActualSamplePeriodProcessticks = 0x30,
   NiFpga_DunkFlips_IndicatorU32_ActualSamplePeriodWaveticks = 0x70,
} NiFpga_DunkFlips_IndicatorU32;

typedef enum
{
   NiFpga_DunkFlips_ControlBool_HighPass = 0x1E,
   NiFpga_DunkFlips_ControlBool_LowPass = 0x22,
} NiFpga_DunkFlips_ControlBool;

typedef enum
{
   NiFpga_DunkFlips_ControlU32_FIFOSignal = 0x14,
   NiFpga_DunkFlips_ControlU32_Minsignaltimegapsamples = 0x34,
   NiFpga_DunkFlips_ControlU32_RequestedSamplePeriodticks = 0x6C,
} NiFpga_DunkFlips_ControlU32;

typedef enum
{
   NiFpga_DunkFlips_TargetToHostFifoI32_Output = 1,
} NiFpga_DunkFlips_TargetToHostFifoI32;

#endif
