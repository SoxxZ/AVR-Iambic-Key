#define F_CPU 8000000UL
#define __DELAY_BACKWARD_COMPATIBLE__

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/power.h>

int ditdelay = 30;
int dahdelay = 90;
int tone = 660;

void setup();
void loop();
void doDit();
void doDah();
void Tone(float freq, float duration);


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
    
    GIMSK=0x03;
    MCUCR=0x00;
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
    Tone(tone,ditdelay);
    PORTB |= (1<<5);
    _delay_ms(ditdelay);
    PORTB &= ~(1<<5);
}

void doDah(){
    Tone(tone,dahdelay);
    PORTB |= (1<<5);
    _delay_ms(dahdelay);
    PORTB &= ~(1<<5);
}

void Tone(float freq, float duration){
    freq = 1/freq;
    freq = freq/2;
    freq = freq*(1000000);
    int i;
    for(i=0; i<= duration;i=i+1){
        i++;
        PORTD |= (1<<5);
        _delay_us(freq);
        PORTD &= ~(1<<5);
        _delay_us(freq);
    }

} 
