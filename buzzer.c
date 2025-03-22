#include "buzzer.h"

// Música do Super Mario (frequência em Hz e duração em ms)
Note mario_theme[] = {
    {660, 100}, {660, 100}, {0, 100}, {660, 100}, {0, 100},
    {510, 100}, {660, 100}, {770, 100}, {0, 300}, {380, 100}, {0, 300},
    {510, 100}, {0, 200}, {380, 100}, {0, 200}, {320, 100}, {0, 300},
    {440, 100}, {480, 100}, {450, 100}, {430, 100}, {380, 100},
    {660, 100}, {760, 100}, {860, 100}, {700, 100}, {760, 100}, {660, 100},
    {520, 100}, {580, 100}, {480, 100},

    {500, 100}, {570, 100}, {500, 100}, {400, 100}, {500, 100}, {570, 100}, {660, 100},
    {700, 100}, {760, 100}, {860, 100}, {760, 100}, {660, 100}, {570, 100}, {500, 100},
    {400, 100}, {300, 100}, {350, 100}, {400, 100}, {500, 100}, {570, 100}, {660, 100},
    {760, 100}, {660, 100}, {570, 100}, {500, 100}
};

// Tamanho da música
const int mario_theme_size = sizeof(mario_theme) / sizeof(Note);

// Inicializa o buzzer no pino definido
void buzzerInit() {
    gpio_set_function(BUZZER_PIN, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(BUZZER_PIN);
    pwm_set_wrap(slice_num, 12500);
    pwm_set_chan_level(slice_num, PWM_CHAN_A, 0);
    pwm_set_enabled(slice_num, true);
}

// Toca uma única nota no buzzer
void playNote(int frequency, int duration) {
    uint slice_num = pwm_gpio_to_slice_num(BUZZER_PIN);

    if (frequency == 0) {
        pwm_set_gpio_level(BUZZER_PIN, 0);  // Silencia o buzzer
    } else {
        uint32_t clock_divider = 4.0f;
        uint32_t wrap_value = (125000000 / clock_divider) / frequency;

        pwm_set_clkdiv(slice_num, clock_divider);
        pwm_set_wrap(slice_num, wrap_value);
        pwm_set_gpio_level(BUZZER_PIN, wrap_value / 2);  // 50% de duty cycle
    }

    sleep_ms(duration);
    pwm_set_gpio_level(BUZZER_PIN, 0);  // Silencia após a nota
    sleep_ms(20);  // Pequena pausa entre as notas
}

// Toca uma música (array de notas)
void playMusic(Note *music, int size) {
    for (int i = 0; i < size; i++) {
        playNote(music[i].frequency, music[i].duration);
    }
}
