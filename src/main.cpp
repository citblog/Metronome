#include <Arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/sleep.h>

#define F_CPU 4800000UL
// Определение частоты звука
#define SOUND_FREQUENCY 4000
// Вычисление необходимого делителя для достижения заданной частоты звука
// #define PRESCALER ((F_CPU / (16 * SOUND_FREQUENCY)) - 1) //(4800000/8)/(2*4000)-1=74
#define SOUND_T 400   //0  // 4000*0.000125=0.5
#define PAUSE_T 520 //00 // 52000*0.000125=6.5
#define CYCLE 3

volatile uint8_t play_sound = 0;  // Флаг воспроизведения звука
volatile uint8_t cycle_count = 0; // Счетчик циклов
uint16_t tik = 0;

void setup_CTC()
{
    TCCR0A = 0;
    TCCR0B = 0;
    TCCR0A |= (1 << WGM01); // Выбор режима CTC (Clear Timer on Compare)
                            //  TCCR0A |= (1 << COM0A0); // Toggle OC0A on Compare Match
    TCCR0B |= (1 << CS01);  // Установка предделителя на 8 |(1 << CS00) 64
    OCR0A = 74;             // период 0,000125 сек
   // DDRB |= (1 << PB0);     // выход OC0A
    DDRB |= (1 << PB2); 
    // включение прерывания по совпадению
    TIMSK0 |= (1 << OCIE0A);
     sei();
  //  DDRB |= (1 << PB4); // выход
                        // PORTB |= (1 << PB4); //1
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
/*
void sound_on()
{
    TCCR0A |= (1 << COM0A0);
}
void sound_off()
{
    TCCR0A &= ~(1 << COM0A0);
}
*/
// Это - указатель на функцию - т.е. ячейка памяти, которая содержит адрес начала функции.
// Этому указателю присваиваем нулевое значение (вернее - указатель на нулевой адрес).
// При обращении к функции по указателю МК перейдет на команду с адресом равным значению указателя
// функции - в данном случае: указан адрес 0 как адрес начала функции.
// void (*Reset)(void) = 0;

int main(void)
{
    setup_CTC();
    setup_interrupts();
    play_sound = 1;

    while (cycle_count < CYCLE)
    {
    }
    /// play_sound = 0;
    // Вход в спящий режим
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_bod_disable();
    sleep_mode();

    return 0;
}

ISR(TIM0_COMPA_vect)
{
    tik++;
    // PORTB ^= (1 << PB4);

    if (tik <= SOUND_T)
    {
        PORTB ^= (1 << PB2);
        /*
         if (!play_sound)
         {
             PORTB ^=(1<<PB0);
             //sound_on();
             play_sound = 1;
         }
         */
    }

    if ((tik > SOUND_T) && (tik < (SOUND_T + PAUSE_T)))
    {
        PORTB &= ~(1 << PB2);
        /* if (play_sound)
         {
             sound_off();
             play_sound = 0;
         }
         */
    }
    else
    {
        tik = 0;
        cycle_count++;
    }
}
ISR(PCINT0_vect)
{
    /*  if (!(PINB & (1 << PB2)))
     {
         sleep_disable();
        // play_sound = 1;
        // cycle_count = 0;
        // asm("sei");
         Reset();
     }
     */
    if (!(PINB & (1 << PB1)))

    {
        _delay_ms(50);

        set_sleep_mode(SLEEP_MODE_PWR_DOWN);
        sleep_bod_disable();
        sleep_mode();
    }

    /*
    MCUCR |= (1<<SM1);  //режим Power-down
    MCUCR &= ~(1<<SM0);
    MCUCR |= (1<<SE);   // recommended to write the Sleep Enable (SE) bit to one just before the execution of
                        //the SLEEP instruction and to clear it immediately after waking up.
    //GIMSK |= (1<<INT0);  //external pin interrupt is enabled
    //PCMSK |= (1<<PCINT2);
    sleep_cpu();
    asm("sleep");
   */
}
