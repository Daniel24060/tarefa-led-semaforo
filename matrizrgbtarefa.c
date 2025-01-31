#include <stdio.h>
#include "pico/stdlib.h"

// Configurações dos pinos
#define LED_R 13  // LED RGB (vermelho)
#define LED_Y 11  // LED RGB (amarelo)
#define LED_G 12  // LED RGB (verde)

// Variável para controlar o estado do semáforo
typedef enum {
    VERMELHO,
    AMARELO,
    VERDE
} EstadoSemaforo;

EstadoSemaforo estado_atual = VERMELHO;

// Função de callback para o temporizador
bool timer_callback(struct repeating_timer *t) {
    // Alterna o estado do semáforo
    switch (estado_atual) {
        case VERMELHO:
            gpio_put(LED_R, 0);  // Desliga vermelho
            gpio_put(LED_Y, 1);  // Liga amarelo
            estado_atual = AMARELO;
            printf("LED VERMELHO DESLIGADO\n");
            printf("LED AMARELO LIGADO\n");
            printf("LED VERDE DESLIGADO\n");
            break;
        case AMARELO:
            gpio_put(LED_Y, 0);  // Desliga amarelo
            gpio_put(LED_G, 1);  // Liga verde
            estado_atual = VERDE;
            printf("LED VERMELHO DESLIGADO\n");
            printf("LED AMARELO DESLIGADO\n");
            printf("LED VERDE LIGADO\n");
            break;
        case VERDE:
            gpio_put(LED_G, 0);  // Desliga verde
            gpio_put(LED_R, 1);  // Liga vermelho
            estado_atual = VERMELHO;
            printf("LED VERMELHO LIGADO\n");
            printf("LED AMARELO DESLIGADO\n");
            printf("LED VERDE DESLIGADO\n");
            break;
    }
    return true;  // Continua o temporizador
}

int main() {
    stdio_init_all();
    printf("Projeto BitDogLab - Semáforo com Temporizador\n");

    // Configuração dos LEDs
    gpio_init(LED_R);
    gpio_set_dir(LED_R, GPIO_OUT);
    gpio_init(LED_Y);
    gpio_set_dir(LED_Y, GPIO_OUT);
    gpio_init(LED_G);
    gpio_set_dir(LED_G, GPIO_OUT);

    // Inicializa o semáforo com o LED vermelho ligado
    gpio_put(LED_R, 1);
    gpio_put(LED_Y, 0);
    gpio_put(LED_G, 0);
    printf("LED VERMELHO LIGADO\n");
    printf("LED AMARELO DESLIGADO\n");
    printf("LED VERDE DESLIGADO\n");

    // Configura o temporizador repetitivo
    struct repeating_timer timer;
    add_repeating_timer_ms(3000, timer_callback, NULL, &timer);

    // Loop principal
    uint32_t tempo_inicial = to_ms_since_boot(get_absolute_time());
    while (true) {
        // Imprime uma mensagem a cada segundo
        uint32_t tempo_atual = to_ms_since_boot(get_absolute_time());
        if (tempo_atual - tempo_inicial >= 1000) {
            printf("PASSOU UM SEGUNDO\n");
            tempo_inicial = tempo_atual;
        }
    }

    return 0;
}