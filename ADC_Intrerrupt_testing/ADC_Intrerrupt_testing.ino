#include <SPI.h>

byte buf0[256];
byte buf1[256];
boolean enable_buf0 = true;
boolean enable_buf1 = false;
boolean write_pointer = true;
unsigned long recByteCount = 0L;
unsigned int bufByteCount = 0;
int pagenumber = 0;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  SPI.setDataMode(0);
  SPI.setBitOrder(MSBFIRST);
  chip_erase();
  ADCSRA |= 1 << ADPS2;
  ADMUX |= 1 << ADLAR;
  ADMUX |= 1 << MUX1;
  ADMUX |= 1 << MUX0;
  ADCSRA |= 1 << ADIE;
  ADCSRA |= 1 << ADEN;
  sei();
  ADCSRA |= 1 << ADSC;
}

void loop() {
  Serial.println("REC");
  while (pagenumber < 256) {
//    Serial.println("maighusgya");
    if (enable_buf0 && !write_pointer) {
      //          Serial.println("Buf0");
      //          for (int i = 0; i < 256; i++) {
      //            Serial.print(buf0[i], DEC);
      //            Serial.print(",");
      //          }
      //          Serial.println();
      pagenumber++;
      _write_page(pagenumber, buf0);
      enable_buf0 = false;
    }
    if (enable_buf1 && write_pointer) {
      //          Serial.println("Buf1");
      //          for (int i = 0; i < 256; i++) {
      //            Serial.print(buf1[i], DEC);
      //            Serial.print(",");
      //          }
      //          Serial.println();
      pagenumber++;
      _write_page(pagenumber, buf1);
      enable_buf1 = false;
    }
  }
  Serial.println("STOP");
  ADCSRA |= 0<<ADIE;
  cli();
  for (int i = 0; i < 256; i++) {
    read_page(i);
  }
  while(1);
}
ISR(ADC_vect) {
//  Serial.println("Interrupt");
  while (bit_is_set(ADCSRA, ADSC));
  //    Serial.println(ADCH, DEC);
  recByteCount++; // increment sample counter
  bufByteCount++;

  if (bufByteCount == 256) {
    bufByteCount = 0;
    enable_buf0 = true;
    enable_buf1 = true;
    write_pointer = !write_pointer;
  }

  if (enable_buf0 && write_pointer) {
    //        Serial.println("writing to buf0");
    buf0[bufByteCount] = ADCH;
  }
  if (enable_buf1 && !write_pointer) {
    //        Serial.println("writing to buf1");
    buf1[bufByteCount] = ADCH;
  }
  ADCSRA |= 1 << ADSC;
}

