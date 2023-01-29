#include <Arduino.h>
#include <Streaming.h>
#include "BucketPinReader.h"

static const uint8_t ADReference = 0x40;

BucketPinReader::Bucket BucketPinReader::buckets[8];

void BucketPinReader::setup()
{
  // Description of bit fields in https://www.arnabkumardas.com/arduino-tutorial/adc-register-description/
  DIDR0 = 0x3F;            // digital inputs disabled
  ADMUX = ADReference;            // measuring on ADC3, use default reference
  ADCSRA = 0x8F;           // AD-converter on, interrupt enabled, prescaler = 16
  ADCSRB = 0x40;           // AD channels MUX on, free running mode
  bitWrite(ADCSRA, 6, 1);  // Start the conversion by setting bit 6 (=ADSC) in ADCSRA
  sei();                   // set interrupt flag
}

int BucketPinReader::readValue(int pin)
{
  Bucket & bucket = buckets[pin - A0];
  int value = bucket.avgValue;
  //Serial << "BucketPinReader pin=" << pin << " bucket=" << pin - A0
  //       << " sumValues=" << bucket.sumValues << " count=" << bucket.count
  //       << " returned value=" << value << endl;
  bucket.isRead = true;
  return value;
}


/*** Interrupt routine ADC ready ***/
static int bucketIx = 0;
void ADConversionInterruptServiceRouting()
{
  int aval;
  aval = ADCL;        // store lower byte ADC first
  aval += ADCH << 8;  // store higher bytes ADC second
  BucketPinReader::Bucket & bucket = BucketPinReader::buckets[bucketIx];
  if (bucket.isRead)
  {
    bucket.sumValues = aval;
    bucket.count = 1;
    bucket.isRead = false;
  }
  else
  {
    bucket.sumValues += aval;
    bucket.count += 1;
  }
  bucket.avgValue = bucket.sumValues / bucket.count;

  //  Serial << "ISR bucket=" << bucketIx << " ADMUX=" << _HEX(ADMUX) << " new value=" << aval
  //         << " count=" << bucket.count << " sumValue=" << bucket.sumValues << endl;

  if (bucketIx >= (A5 - A0))
  {
    // Go back to first bucket.
    bucketIx = 0;
    ADMUX = ADReference;
  }
  else
  {
    // Use next bucket.
    bucketIx++;
    ADMUX = ADReference + bucketIx;
  }
  bitWrite(ADCSRA, 6, 1);  // Start the conversion by setting bit 6 (=ADSC) in ADCSRA
}

ISR(ADC_vect) {
  ADConversionInterruptServiceRouting();
}

BucketPinReader & createBucketPinReader()
{
  static BucketPinReader pinReader;
  return pinReader;
}
