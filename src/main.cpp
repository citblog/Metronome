//#include <Arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/sleep.h>

//#define F_CPU 4800000UL
// Определение частоты звука
#define SOUND_FREQUENCY 2000
// Вычисление необходимого делителя для достижения заданной частоты звука
// #define PRESCALER ((F_CPU / (16 * SOUND_FREQUENCY)) - 1) //(4800000/8)/(2*2000)-1=149
#define SOUND_T 1000  // 1000*0.00025=0.25
#define PAUSE_T 39000 //00 // 39000*0.00025=9.75
#define CYCLE 61

//volatile uint8_t play_sound = 0;  // Флаг воспроизведения звука
volatile uint8_t cycle_count = 0; // Счетчик циклов
volatile uint16_t tik = 0;

void setup_CTC()
{
    TCCR0A = 0; 
    TCCR0B = 0;
    TCCR0A |= (1 << WGM01); // Выбор режима CTC (Clear Timer on Compare)
                            //  TCCR0A |= (1 << COM0A0); // Toggle OC0A on Compare Match
    TCCR0B |= (1 << CS01);  // Установка предделителя на 8 |(1 << CS00) 64
    OCR0A = 149;             // период 0,00025 сек
   // DDRB |= (1 << PB0);     // выход OC0A
    DDRB |= (1 << PB2); 
    // включение прерывания по совпадению
    TIMSK0 |= (1 << OCIE0A);
     sei();
  
}

void setup_interrupts()
{
    // для прерывания
    MCUCR &= ~(1 << ISC00);
    MCUCR &= ~(1 << ISC01);
    DDRB &= ~(1 << PB1);  // вход
    PORTB |= (1 << PB1);  // подтягивающтй резистор
    GIMSK |= (1 << PCIE); // external pin interrupt is enabled
    PCMSK |= (1 << PCINT1);
    asm("sei"); 
}

// Это - указатель на функцию - т.е. ячейка памяти, которая содержит адрес начала функции.
// Этому указателю присваиваем нулевое значение (вернее - указатель на нулевой адрес).
// При обращении к функции по указателю МК перейдет на команду с адресом равным значению указателя
// функции - в данном случае: указан адрес 0 как адрес начала функции.
// void (*Reset)(void) = 0;

int main(void)
{
    setup_CTC();
    setup_interrupts();
   // play_sound = 1;

    while (cycle_count < CYCLE)
    {
    }
        // Вход в спящий режим
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_bod_disable();
    sleep_mode();

    return 0;
}

ISR(TIM0_COMPA_vect)
{
    tik++;
    // PORTB ^= (1 << PB2);
 
    if (tik < SOUND_T)
    {
        PORTB ^= (1 << PB2);
    }

    else if ((tik > SOUND_T) && (tik < (SOUND_T + PAUSE_T)))
    {
        PORTB &= ~(1 << PB2);
      
    }
    else if (tik > (SOUND_T + PAUSE_T))
    {
        tik = 0;
        cycle_count++;
    }
   
}


ISR(PCINT0_vect)
{
    
    if (!(PINB & (1 << PB1)))

    {
        _delay_ms(50);

        set_sleep_mode(SLEEP_MODE_PWR_DOWN);
        sleep_bod_disable();
        sleep_mode();
    }

   
}
