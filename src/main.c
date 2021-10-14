#define F_CPU 16000000UL
#define __DELAY_BACKWARD_COMPATIBLE__

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/power.h>
#include <util/atomic.h>

int ditdelay = 60;
int dahdelay = 180;
int tone = 100;
volatile unsigned long timer1_millis; 

void setup();
void loop();
void doDit();
void doDah();
void Tone(float freq, float duration);
void init_millis(unsigned long f_cpu);
unsigned long millis();


int main() {
    setup();
    
    while(1){
        loop();    
    }
    
    return(0);
}

void setup(){
    DDRB = DDRB | (1<<5); //LED OUTPUT
    DDRD = DDRD | (1<<5); //BUZZER OUTPUT
    DDRD = DDRD & ~(1<<2);// INT0 INPUT
    DDRD = DDRD & ~(1<<3);//INT1 OUTPUT 
    PORTD |= (1<<2); //INT0 PULLUP
    PORTD |= (1<<3); //INT1 PULLUP
    
    EIMSK=0x03;
    MCUCR=0x00;

    init_millis(F_CPU);

    sei();
}

void loop(){

}

ISR(INT0_vect){
    doDit();

}

ISR(INT1_vect){
    doDah();
}

void doDit(){
    PORTB |= (1<<5);
    Tone(tone,ditdelay);
    PORTB &= ~(1<<5);
    _delay_ms(3*ditdelay);
}

void doDah(){
    PORTB |= (1<<5);
    Tone(tone,dahdelay);
    PORTB &= ~(1<<5);
    _delay_ms(3*dahdelay);
}

void Tone(float freq, float duration){
    freq = 1/freq;
    freq = freq/2;
    freq = freq * 1000000;
    float time_start = millis();
    while (millis() - time_start <= duration){
        PORTD |= (1<<5);
        _delay_us(freq);
        PORTD &= ~(1<<5);
        _delay_us(freq);
    }
} 




//NOTE: A unsigned long holds values from 0 to 4,294,967,295 (2^32 - 1). It will roll over to 0 after reaching its maximum value.

ISR(TIMER1_COMPA_vect)
{
  timer1_millis++;  
}

void init_millis(unsigned long f_cpu){
  unsigned long ctc_match_overflow;
  
  ctc_match_overflow = ((f_cpu / 1000) / 8); //when timer1 is this value, 1ms has passed
    
  // (Set timer to clear when matching ctc_match_overflow) | (Set clock divisor to 8)
  TCCR1B |= (1 << WGM12) | (1 << CS11);
  
  // high byte first, then low byte
  OCR1AH = (ctc_match_overflow >> 8);
  OCR1AL = ctc_match_overflow;
 
  // Enable the compare match interrupt
  TIMSK1 |= (1 << OCIE1A);
 
  //REMEMBER TO ENABLE GLOBAL INTERRUPTS AFTER THIS WITH sei(); !!!
}

unsigned long millis(){
  unsigned long millis_return;
 
  // Ensure this cannot be disrupted
  ATOMIC_BLOCK(ATOMIC_FORCEON) {
    millis_return = timer1_millis;
  }
  return millis_return;
} 