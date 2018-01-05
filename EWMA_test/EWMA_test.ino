void setup() {
  Serial.begin(115200);
   ADCSRA |= 1 << ADPS2;
  ADCSRA &= ~ 1 << ADPS1;
  ADCSRA |= 1 << ADPS0;
  ADMUX |= 1 << ADLAR;
  ADMUX |= 1 << MUX1;
  ADMUX |= 1 << MUX0;
  ADCSRA |= 1 << ADIE;
  ADCSRA |= 1 << ADEN;
  DDRB |= 0X2C;
//  SPCR |= 1 << MSTR ;
//  SPCR |= 1 << SPE;
sei();
  ADCSRA |= 1 << ADSC;
}

void loop() {
  Serial.println("hi");
  ADCSRA |= 1 << ADSC;
}
