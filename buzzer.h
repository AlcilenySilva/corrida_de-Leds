#ifndef BUZZER_H
#define BUZZER_H


#include "pico/stdlib.h"
#include "hardware/pwm.h"


// Define o pino do buzzer 
#define BUZZER_PIN 21  


// Estrutura para representar uma nota musical
typedef struct {
    int frequency; 
    int duration;  
} Note;


// Declaração da música do Mario e seu tamanho
extern Note mario_theme[];
extern const int mario_theme_size;


// Funções do buzzer
void buzzerInit();
void playNote(int frequency, int duration);
void playMusic(Note *music, int size);


#endif
