#define F_CPU 16000000UL
#define __DELAY_BACKWARD_COMPATIBLE__

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/power.h>

int ditdelay = 60;
int dahdelay = 180;
int tone = 440;

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
    
    EIMSK=0x03;
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
    PORTB |= (1<<5);
    Tone(tone,ditdelay);
    PORTB &= ~(1<<5);
    _delay_ms(3*ditdelay);
}

void doDah(){
    PORTB |= (1<<5);
    Tone(tone,dahdelay);
    PORTB &= ~(1<<5);
    _delay_ms(3*ditdelay);
}

void Tone(float freq, float duration){
    freq = 1/freq;
    freq = freq/2;
    freq = freq*(1000000);
    int i = 0;
    while(i <= duration){
        PORTD |= (1<<5);
        _delay_us(freq);
        PORTD &= ~(1<<5);
        _delay_us(freq);
        _delay_us(1000);
        i=i+1;
    }
} 
