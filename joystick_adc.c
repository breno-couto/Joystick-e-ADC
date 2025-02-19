#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/adc.h"
#include <stdlib.h>

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

// Configura PWM para um pino específico
void setup_pwm(uint gpio) {
    gpio_set_function(gpio, GPIO_FUNC_PWM);
    uint slice = pwm_gpio_to_slice_num(gpio);
    pwm_set_wrap(slice, 4095);  // PWM de 12 bits (0-4095)
    pwm_set_enabled(slice, true);
}

// Define o nível PWM do LED
void set_pwm(uint gpio, uint16_t value) {
    pwm_set_gpio_level(gpio, value);
}

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

    // Configura o ADC
    adc_init();
    adc_gpio_init(joyX);
    adc_gpio_init(joyY);

    // Configura LEDs PWM
    setup_pwm(LED_R);
    setup_pwm(LED_B);

    // Configura LED Verde como saída (on/off)
    gpio_init(LED_G);
    gpio_set_dir(LED_G, GPIO_OUT);
    gpio_put(LED_G, 0);

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

// Lê valores do joystick (ADC)
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

        // Normaliza valores para PWM (0 - 4095)
        uint16_t pwm_vermelho = abs(eixo_x - 2048) * 2; // Quanto mais longe do centro, mais brilho
        uint16_t pwm_azul = abs(eixo_y - 2048) * 2;

        // Aplica PWM aos LEDs
        set_pwm(LED_R, pwm_vermelho);
        set_pwm(LED_B, pwm_azul);

        printf("Joystick -> X: %d, Y: %d | PWM -> R: %d, B: %d\n", eixo_x, eixo_y, pwm_vermelho, pwm_azul);
        sleep_ms(100);
    }
}

