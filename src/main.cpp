//#include <Arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/sleep.h>

#define F_CPU 4800000UL

// Определение частоты звука
#define SOUND_FREQUENCY 4000

// Вычисление необходимого делителя для достижения заданной частоты звука
#define PRESCALER ((F_CPU / (16 * SOUND_FREQUENCY)) - 1) //(4800000/8)/(2*4000)-1=74

// volatile uint8_t play_sound = 0;  // Флаг воспроизведения звука
volatile uint8_t cycle_count = 0; // Счетчик циклов
// volatile uint8_t sleep_flag = 0;  // Флаг спящего режима

void setup_CTC()
{
    TCCR0A = 0;
    TCCR0B = 0;
 //   TCCR0A |= (1 << WGM01);  // Выбор режима CTC (Clear Timer on Compare)
    TCCR0A |= (1 << COM0A0); // Toggle OC0A on Compare Match
    TCCR0B |= (1 << CS01);   // Установка предделителя на 8 |(1 << CS00) 64
    OCR0A=PRESCALER; // Установка значения для генерации нужной частоты
    //OCR0A = 74;
    DDRB |= (1 << PB0);
}
void sound_on()
{
    TCCR0A |= (1 << COM0A0);
}
void sound_off()
{
    TCCR0A &= ~(1 << COM0A0);
}

int main(void)
{
    setup_CTC();
    while (cycle_count < 100)
    {
        sound_on();
        _delay_ms(1000);
    

        sound_off();
        _delay_ms(1000);

        cycle_count++;
    }

    return 0;
}
/*
// Обработчик прерывания
ISR(TIM0_COMPA_vect){
    // Инвертирование состояния пина для генерации звука
    PORTB ^= (1 << PB1);
}
*/