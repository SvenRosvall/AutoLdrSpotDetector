#ifndef ARDUINO_HPP
#define ARDUINO_HPP

typedef unsigned char uint8_t;

const uint8_t A0=20;
const uint8_t A1=21;
const uint8_t A2=22;
const uint8_t A3=23;
const uint8_t A4=24;
const uint8_t A5=25;
const uint8_t A6=26;
const uint8_t A7=27;

enum PinMode {
        OUTPUT,
        INPUT,
        INPUT_PULLUP
};
enum PinState {
        LOW,
        HIGH
};

void analogWrite(int pin, int value);
int analogRead(int pin);
void digitalWrite(int pin, PinState value);
int digitalRead(int pin);
int map(int value, int fromLower, int fromUpper, int toLower, int toUpper);
void pinMode(int pin, PinMode mode);
unsigned long millis();
void delay(int millis);

struct Print
{
        void begin(int baudRate);
        void println(const char *);
};
extern struct Print Serial;

// Hardware values used by ADC free running mode.
extern uint8_t DIDR0;
extern uint8_t ADMUX;
extern uint8_t ADCSRA;
extern uint8_t ADCSRB;
extern uint8_t ADCL;
extern uint8_t ADCH;
void sei();
void bitWrite(int, int, int);
class ADC_vect{} ;

#endif
