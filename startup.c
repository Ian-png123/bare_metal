/* startup.c - vector table and reset handler for STM32F411 */

#include <stdint.h>

/* Symbols provided by the linker script */
extern uint32_t _estack;
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _etext;
extern uint32_t _sbss;
extern uint32_t _ebss;

/* Our entry point */
int main(void);

/* Forward-declare the reset handler so we can reference it in the table */
void Reset_Handler(void);

/* Weak default handler for all other interrupts */
void Default_Handler(void);

/*
 * The vector table.
 * Word 0 = initial stack pointer (top of RAM)
 * Word 1 = reset vector (address of Reset_Handler)
 * Words 2..N = other exception/interrupt vectors
 *
 * __attribute__((section(".isr_vector"))) forces this array into
 * the .isr_vector section that the linker script places first.
 */
__attribute__((section(".isr_vector"), used))
void (* const vector_table[])(void) = {
    (void (*)(void))&_estack,   /* 0x00: initial SP */
    Reset_Handler,              /* 0x04: Reset */
    Default_Handler,            /* 0x08: NMI */
    Default_Handler,            /* 0x0C: HardFault */
    Default_Handler,            /* 0x10: MemManage */
    Default_Handler,            /* 0x14: BusFault */
    Default_Handler,            /* 0x18: UsageFault */
    0, 0, 0, 0,                 /* 0x1C-0x28: reserved */
    Default_Handler,            /* 0x2C: SVCall */
    Default_Handler,            /* 0x30: DebugMonitor */
    0,                          /* 0x34: reserved */
    Default_Handler,            /* 0x38: PendSV */
    Default_Handler,            /* 0x3C: SysTick */
    /* External IRQs (IRQ0..IRQn) would follow here */
};

/* The reset handler: the very first C code that runs */
void Reset_Handler(void)
{
    /* 1. Copy .data from Flash to RAM */
    uint32_t *src = &_etext;
    uint32_t *dst = &_sdata;
    while (dst < &_edata) {
        *dst++ = *src++;
    }

    /* 2. Zero .bss */
    dst = &_sbss;
    while (dst < &_ebss) {
        *dst++ = 0;
    }

    /* 3. Jump to main. If it returns, hang. */
    main();
    for (;;) { }
}

/* Any unhandled interrupt lands here and spins forever */
void Default_Handler(void)
{
    for (;;) { }
}
