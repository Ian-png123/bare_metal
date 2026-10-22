/* linker.ld - STM32F411CEU6 (WeAct Blackpill) */

ENTRY(Reset_Handler)

_estack = ORIGIN(RAM) + LENGTH(RAM);   /* stack grows down from top of RAM */

MEMORY
{
    FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 512K
    RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 128K
}

SECTIONS
{
    /* Vector table must be the very first thing in Flash */
    .isr_vector :
    {
        KEEP(*(.isr_vector))
    } > FLASH

    /* Code + read-only data */
    .text :
    {
        *(.text)
        *(.text*)
        *(.rodata)
        *(.rodata*)
        _etext = .;
    } > FLASH

    /* Initialized data: stored in Flash, copied to RAM at startup */
    .data : AT(_etext)
    {
        _sdata = .;
        *(.data)
        *(.data*)
        _edata = .;
    } > RAM

    /* Zero-initialized data: lives only in RAM */
    .bss :
    {
        _sbss = .;
        *(.bss)
        *(.bss*)
        *(COMMON)
        _ebss = .;
    } > RAM
}
