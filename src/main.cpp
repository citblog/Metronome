#include <Arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>

// Определение частоты тактирования
#define F_CPU 4800000UL

// Определение частоты звука
#define SOUND_FREQUENCY 4000

// Вычисление необходимого делителя для достижения заданной частоты звука
//#define PRESCALER ((F_CPU / (16 * SOUND_FREQUENCY)) - 1) //(4800000/8)/(2*4000)-1=74

int main(void) {
    // Настройка порта для генерации звука
    DDRB |= (1 << PB1)|(1 << PB0);

    // Настройка таймера
    TCCR0A=0;
    TCCR0B=0;
    TCCR0A |= (1 << WGM01); // Выбор режима CTC (Clear Timer on Compare)
    TCCR0A |=(1<<COM0A0) ; //Toggle OC0A on Compare Match
    TCCR0B |= (1 << CS01) ; // Установка предделителя на 8 |(1 << CS00) 64

   // OCR0A = PRESCALER; // Установка значения для генерации нужной частоты
    OCR0A = 74;

    // Включение прерывания по совпадению таймера с регистром сравнения
    TIMSK0 |= (1 << OCIE0A);

    // Включение глобального разрешения прерываний
    sei();

    while (1) {
        // Ваш код основной программы
    }

    return 0;
}

// Обработчик прерывания
ISR(TIM0_COMPA_vect){
    // Инвертирование состояния пина для генерации звука
    PORTB ^= (1 << PB1);
}
