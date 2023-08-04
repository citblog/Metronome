// #include <Arduino.h>
//_delay_ не принимают таких значений
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/sleep.h>

#define F_CPU 4800000UL
// Определение частоты звука
#define SOUND_FREQUENCY 4000
// Вычисление необходимого делителя для достижения заданной частоты звука
// #define PRESCALER ((F_CPU / (16 * SOUND_FREQUENCY)) - 1) //(4800000/8)/(2*4000)-1=74
#define SOUND_T 500
#define PAUSE_T 6500
#define CYCLE 100

volatile uint8_t play_sound = 0;  // Флаг воспроизведения звука
volatile uint8_t cycle_count = 0; // Счетчик циклов


void setup_CTC()
{
    TCCR0A = 0;
    TCCR0B = 0;
    TCCR0A |= (1 << WGM01); // Выбор режима CTC (Clear Timer on Compare)
    ////   TCCR0A |= (1 << COM0A0); // Toggle OC0A on Compare Match
    TCCR0B |= (1 << CS01);  // Установка предделителя на 8 |(1 << CS00) 64
    OCR0A = 74;
    DDRB |= (1 << PB0); // выход OC0A

    ////  DDRB |= (1 << PB4); //выход
    ///  PORTB &= ~(1 << PB4); //0
}

void setup_interrupts()
{
    // для прерывания
    MCUCR&=~(1<<ISC00);
    MCUCR&=~(1<<ISC01);
    DDRB &= ~(1 << PB1);  // вход
    PORTB |= (1 << PB1);  // подтягивающтй резистор
    GIMSK |= (1 << PCIE); // external pin interrupt is enabled
    PCMSK |= (1 << PCINT1);
    asm("sei");
}
void sound_on()
{
    TCCR0A |= (1 << COM0A0);
}
void sound_off()
{
    TCCR0A &= ~(1 << COM0A0);
}
// Это - указатель на функцию - т.е. ячейка памяти, которая содержит адрес начала функции.
// Этому указателю присваиваем нулевое значение (вернее - указатель на нулевой адрес).
// При обращении к функции по указателю МК перейдет на команду с адресом равным значению указателя
// функции - в данном случае: указан адрес 0 как адрес начала функции.
//void (*Reset)(void) = 0;

int main(void)
{
    setup_CTC();
    setup_interrupts();
    play_sound = 1;

    while (cycle_count < CYCLE)
    {
        //   sei();
        sound_on();
        _delay_ms(SOUND_T);

        sound_off();
        _delay_ms(PAUSE_T);

        cycle_count++;
    }
    play_sound = 0;
    // Вход в спящий режим
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_bod_disable();
    sleep_mode();
   
    return 0;
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
        	_delay_ms (50);
        
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
