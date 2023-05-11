#include "cpu_i.h"

//writing into SPR
static void mtspr(u32 spr, u32 value)
{
	asm("l.mtspr\t\t%0,%1,0": : "r" (spr), "r" (value));
}

//reading SPR
static u32 mfspr(u32 spr)
{
	u32 value;
	asm("l.mfspr\t\t%0,%1,0" : "=r" (value) : "r" (spr));
	return value;
}

extern char stack_limit_addr[];
extern char stack_base_addr[];
#if 0
static s32 cpu_stack_monitor(u32 message, u32 aux)
{
	u32 *stack = (u32 *)stack_limit_addr;
	u32  size = 0;
	while ((stack < (u32 *)stack_base_addr) && (*stack == 0))
	{
		stack++;
		size += 4;
	}
	printk("stack free:%dbyte\n", size);
	return OK;
}
#endif
//initialize cpu interrupts
void cpu_init(void)
{
	//regiser stack status check daemon service
//	daemon_register_service(cpu_stack_monitor);

	//enable interrupts in supervisor register
	mtspr(SPR_SR, (mfspr(SPR_SR) | SPR_SR_IEE));

	//...
}


s32 cpu_disable_int(void)
{
	s32 cpsr;

	cpsr = mfspr(SPR_SR);
	mtspr(SPR_SR, cpsr & ~(SPR_SR_IEE | SPR_SR_TEE));
	return cpsr;
}

void cpu_enable_int(s32 cpsr)
{
	mtspr(SPR_SR, cpsr);
}
/*
//return value by making a syscall
static void exit(void)
{

	LOG("system exit\n");
	while (1);
}
*/
