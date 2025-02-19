#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/adc.h"

#define LED_B 12
#define LED_G 11
#define LED_R 13
#define BOTAO_JOYSTICK 22
#define BOTAO_A 5
#define joyX 26
#define joyY 27

#define DEBOUNCE_TIME 200000  // 200 ms em microssegundos

static bool estado_ledG = false;
static bool estado_ledR = false;
static bool estado_ledB = false;
static uint32_t last_interrupt_time_joystick = 0;
static uint32_t last_interrupt_time_A = 0;

// Callback única para interrupções
void irq_callback(uint gpio, uint32_t eventos) {
    uint32_t current_time = time_us_32();  // Obtém o tempo atual

    if (gpio == BOTAO_JOYSTICK) {
        if (current_time - last_interrupt_time_joystick < DEBOUNCE_TIME) return;  // Ignora se for muito rápido
        last_interrupt_time_joystick = current_time;

        estado_ledG = !estado_ledG;
        gpio_put(LED_G, estado_ledG);
    } 
    else if (gpio == BOTAO_A) {
        if (current_time - last_interrupt_time_A < DEBOUNCE_TIME) return;  // Ignora se for muito rápido
        last_interrupt_time_A = current_time;

        estado_ledR = !estado_ledR;
        estado_ledB = !estado_ledB;

        gpio_put(LED_R, estado_ledR);
        gpio_put(LED_B, estado_ledB);
    }
}

void full_setup() {

    stdio_init_all();

    //configura o adc
    adc_init();
    adc_gpio_init(joyX);
    adc_gpio_init(joyY);

    // Configura LEDs como saída
    gpio_init(LED_G);
    gpio_set_dir(LED_G, GPIO_OUT);
    gpio_put(LED_G, 0);

    gpio_init(LED_R);
    gpio_set_dir(LED_R, GPIO_OUT);
    gpio_put(LED_R, 0);

    gpio_init(LED_B);
    gpio_set_dir(LED_B, GPIO_OUT);
    gpio_put(LED_B, 0);

    // Configura Botões como entrada com pull-up interno
    gpio_init(BOTAO_A);
    gpio_set_dir(BOTAO_A, GPIO_IN);
    gpio_pull_up(BOTAO_A);

    gpio_init(BOTAO_JOYSTICK);
    gpio_set_dir(BOTAO_JOYSTICK, GPIO_IN);
    gpio_pull_up(BOTAO_JOYSTICK);

    // Configura interrupção única para ambos os botões
    gpio_set_irq_enabled_with_callback(BOTAO_JOYSTICK, GPIO_IRQ_EDGE_FALL, true, &irq_callback);
    gpio_set_irq_enabled(BOTAO_A, GPIO_IRQ_EDGE_FALL, true);
}

int ler_joystick_x(){
    adc_select_input(0);
    return adc_read();
}

int ler_joystick_y(){
    adc_select_input(1);
    return adc_read();
}

int main() {   
    full_setup();

    while (true) {
        int eixo_x = ler_joystick_x();
        int eixo_y = ler_joystick_y();

        printf("Joystick -> X: %d, Y: %d\n", eixo_x, eixo_y);
        sleep_ms(200);  // Pequeno delay para evitar sobrecarga
    }
}
