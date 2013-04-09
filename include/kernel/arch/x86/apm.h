#ifndef KERNEL_ARCH_x86_APM_H
#define KERNEL_ARCH_x86_APM_H

extern int apm_check_exist(void);
extern void apm_connect(void);
extern void apm_disconnect(void);
extern void apm_enable_power_management(void);
extern void apm_driver_version(void);
extern void apm_enable_device_power_management(void);
extern void apm_engage_power_management(void);
extern void apm_poweroff(void);
extern void apm_suspend(void);
extern void apm_standby(void);

#endif
