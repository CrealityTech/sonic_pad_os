#ifndef __SUN50IW11P1_H__
#define __SUN50IW11P1_H__
/* cpu spec files defined */
#define CPU_NUM_MAX     2
#define CPU0LOCK        "/dev/null"
#define ROOMAGE         "/dev/null"
#define CPUFREQ_AVAIL   "/sys/devices/system/cpu/cpu0/cpufreq/policy0/scaling_available_frequencies"
#define CPUFREQ         "/sys/devices/system/cpu/cpu0/cpufreq/policy0/cpuinfo_cur_freq"
#define CPUFREQ_MAX     "/sys/devices/system/cpu/cpu0/cpufreq/policy0/scaling_max_freq"
#define CPUFREQ_MIN     "/sys/devices/system/cpu/cpu0/cpufreq/policy0/scaling_min_freq"
#define CPUONLINE       "/sys/devices/system/cpu/online"
#define CPUHOT          "/dev/null"
#define CPU0GOV         "/sys/devices/system/cpu/cpu0/cpufreq/policy0/scaling_governor"
/* gpu spec files defined */
#define GPUFREQ         "/dev/null"
#define GPUCOMMAND      "/dev/null"
/* ddr spec files defined */
#define DRAMFREQ_AVAIL  "/dev/null"
#define DRAMFREQ        "/dev/null"
#define DRAMFREQ_MAX    "/dev/null"
#define DRAMFREQ_MIN    "/dev/null"
#define DRAMMODE        "/dev/null"
/* task spec files defined */
#define TASKS           "/dev/null"
/* touch screen runtime suspend */
#define TP_SUSPEND      "/dev/null"

#endif
