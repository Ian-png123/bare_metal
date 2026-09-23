/* main.c - blink two external LEDs on PA0 and PA1 */

#include <stdint.h>

#define RCC_BASE    0x40023800UL
#define GPIOA_BASE  0x40020000UL

#define RCC_AHB1ENR  (*(volatile uint32_t *)(RCC_BASE + 0x30))
#define GPIOA_MODER   (*(volatile uint32_t *)(GPIOA_BASE + 0x00))
#define GPIOA_OTYPER  (*(volatile uint32_t *)(GPIOA_BASE + 0x04))
#define GPIOA_OSPEEDR (*(volatile uint32_t *)(GPIOA_BASE + 0x08))
#define GPIOA_PUPDR   (*(volatile uint32_t *)(GPIOA_BASE + 0x0C))
#define GPIOA_BSRR    (*(volatile uint32_t *)(GPIOA_BASE + 0x18))
#define GPIOA_IDR (*(volatile uint32_t *)(GPIOA_BASE + 0x10))
#define BIT  (1UL << 0)

/* LED pin assignments */
#define LED0_PIN  4   /* PA4 this pin has two bits [0,0]*/
#define LED1_PIN  5   /* PA5 this pin has two bits [0,0]*/ 
#define BUTT_PIN  6

/* Atomic set/reset helpers for GPIOA */
#define LED0_SET    (1UL << LED0_PIN)
#define LED0_RESET  (1UL << (LED0_PIN + 16))
#define LED1_SET    (1UL << LED1_PIN)
#define LED1_RESET  (1UL << (LED1_PIN + 16))

/* Configure a pin as push-pull output, low speed, no pull */
static void config_output(uint32_t pin)
{
    /* MODER: 01 = general purpose output */
    GPIOA_MODER &= ~(3UL << (pin * 2)); // [00]
    GPIOA_MODER |=  (1UL << (pin * 2)); // [01]

    /* OTYPER: 0 = push-pull */
    GPIOA_OTYPER &= BIT;
}
static void config_input(uint32_t pin){
    GPIOA_MODER &= ~(3UL << (pin * 2));
        // PUPDR: 01 = pull-up
    GPIOA_PUPDR &= ~(3UL << (pin * 2)); // Clear both bits
    GPIOA_PUPDR |=  (1UL << (pin * 2)); // Set first bit [01]
}
static uint32_t digital_read(uint32_t pin){
    uint32_t reading = GPIOA_IDR & (1UL << pin) ? 1 : 0; /* use temary operator to find out the state of the butoon*/
    return reading;
}
static void delay(volatile uint32_t ms)
{
    volatile uint32_t count = ms * 4000;
    while(count--){
        __asm__ volatile("nop");
    }

}

int main(void)
{
    /* 1. Enable GPIOA clock (RCC_AHB1ENR bit 0) */
    RCC_AHB1ENR |= BIT;

    /* Small delay so the clock is stable */
    delay(100);

    /*2. set the configuration for the pins */
    config_output(LED0_PIN);
    config_output(LED1_PIN);
    config_input(BUTT_PIN);
    /* 3. Alternate the two LEDs */
    for (;;) {
        if(BUTT_PIN == 0){
        /* LED0 on, LED1 off */
        GPIOA_BSRR = (1UL << LED0_PIN);
        }else{
        /* LED0 off, LED1 on */
        GPIOA_BSRR = (1UL << LED1_PIN); // a thousand millisecond is one-sec
        }
    }
}
