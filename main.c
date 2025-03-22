#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "pico/stdlib.h"
#include "neopixel_pio.h"
#include "buzzer.h"

int main() {
    stdio_init_all();
    npInit(LED_PIN);
    buzzerInit();
    
    srand(time(NULL)); 

    uint led1_pos = 0;  // LED vermelho
    uint led2_pos = 1;  // LED azul

    while (1) {
        npClear();

        // Cada LED tem 50% de chance de avançar
        if (rand() % 2 == 0) {
            led1_pos = (led1_pos + 1) % LED_COUNT;
        }
        if (rand() % 2 == 0) {
            led2_pos = (led2_pos + 1) % LED_COUNT;
        }

        // Acende os LEDs nas novas posições
        npSetLED(led1_pos, 255, 0, 0);  // Vermelho
        npSetLED(led2_pos, 0, 0, 255);  // Azul
        npWrite();

        // Toca uma nota correspondente ao movimento
        playNote(mario_theme[(led1_pos + led2_pos) % mario_theme_size].frequency, 
                 mario_theme[(led1_pos + led2_pos) % mario_theme_size].duration);

        // Verifica se algum LED chegou ao final
        if (led1_pos == LED_COUNT - 1 || led2_pos == LED_COUNT - 1) {
            uint vencedor = (led1_pos == LED_COUNT - 1) ? led1_pos : led2_pos;
            uint perdedor = (led1_pos == LED_COUNT - 1) ? led2_pos : led1_pos;
            uint8_t r = (vencedor == led1_pos) ? 255 : 0;
            uint8_t g = (vencedor == led1_pos) ? 0 : 0;
            uint8_t b = (vencedor == led1_pos) ? 0 : 255;

            // Piscar apenas o LED vencedor
            for (int i = 0; i < 3; i++) {  
                npClear();
                npSetLED(vencedor, r, g, b); 
                npSetLED(perdedor, 0, 0, 255);  
                npWrite();
                sleep_ms(200);
                npClear();
                npWrite();
                sleep_ms(200);
            }

          
            led1_pos = 0;
            led2_pos = 1;
        }

        sleep_ms(300);
    }
}
