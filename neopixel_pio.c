#include "neopixel_pio.h"
#include "ws2818b.pio.h"
#include <stdlib.h> 
#include <time.h>  
// Buffer de pixels
npLED_t leds[LED_COUNT];

// Variáveis para controle da máquina PIO
PIO np_pio;
uint sm;

void npInit(uint pin) {
    uint offset = pio_add_program(pio0, &ws2818b_program);
    np_pio = pio0;

    sm = pio_claim_unused_sm(np_pio, false);
    if (sm < 0) {
        np_pio = pio1;
        sm = pio_claim_unused_sm(np_pio, true);
    }

    ws2818b_program_init(np_pio, sm, offset, pin, 800000.f);

    for (uint i = 0; i < LED_COUNT; ++i) {
        leds[i].R = 0;
        leds[i].G = 0;
        leds[i].B = 0;
    }
}

void npSetLED(uint index, uint8_t r, uint8_t g, uint8_t b) {
    leds[index].R = r;
    leds[index].G = g;
    leds[index].B = b;
}

void npClear() {
    for (uint i = 0; i < LED_COUNT; ++i)
        npSetLED(i, 0, 0, 0);
}

void npWrite() {
    for (uint i = 0; i < LED_COUNT; ++i) {
        pio_sm_put_blocking(np_pio, sm, leds[i].G);
        pio_sm_put_blocking(np_pio, sm, leds[i].R);
        pio_sm_put_blocking(np_pio, sm, leds[i].B);
    }
    sleep_us(500);
}

void corridaCompetitiva() {
    uint8_t cor1_R = 255, cor1_G = 0, cor1_B = 0;  // LED 1 (vermelho)
    uint8_t cor2_R = 0, cor2_G = 0, cor2_B = 255;  // LED 2 (azul)

    uint led1_pos = 0;
    uint led2_pos = 1;

    srand(time(NULL)); // Inicializa a semente aleatória

    while (led1_pos < LED_COUNT - 1 && led2_pos < LED_COUNT - 1) {
        npClear();
        npSetLED(led1_pos, cor1_R, cor1_G, cor1_B);
        npSetLED(led2_pos, cor2_R, cor2_G, cor2_B);
        npWrite();
        
        // Define velocidade aleatória para cada LED
        uint8_t velocidade1 = (rand() % 3) + 1; // Avança entre 1 e 3 LEDs
        uint8_t velocidade2 = (rand() % 3) + 1; 

        // Alterna posições simulando ultrapassagens ocasionais
        if (rand() % 5 == 0) {
            uint temp = led1_pos;
            led1_pos = led2_pos;
            led2_pos = temp;
        }

        led1_pos += velocidade1;
        led2_pos += velocidade2;

        // Evita ultrapassar os limites da fita de LEDs
        if (led1_pos >= LED_COUNT) led1_pos = LED_COUNT - 1;
        if (led2_pos >= LED_COUNT) led2_pos = LED_COUNT - 1;

        sleep_ms(200); 
    }

    // Determina o vencedor
    npClear();
    if (led1_pos >= LED_COUNT - 1) {
        npSetLED(LED_COUNT - 1, cor1_R, cor1_G, cor1_B);
    } else {
        npSetLED(LED_COUNT - 1, cor2_R, cor2_G, cor2_B);
    }
    npWrite();
    sleep_ms(1000); 
}
