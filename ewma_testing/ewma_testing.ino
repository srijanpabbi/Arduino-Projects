uint8_t val = 0;
float current_ip = 0;
float previous_ip = 0;
//float lambda = 0.01;
float lambda = 0.01;

inline float ewma() {
  current_ip = lambda * val + (1 - lambda) * previous_ip;
  previous_ip = current_ip;
  return previous_ip;
}

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
  sei();
  ADCSRA |= 1 << ADSC;
}

void loop() {
  // put your main code here, to run repeatedly:

}
ISR(ADC_vect) {
  val = abs(ADCH-;
//  val = ADCH; 
//val = (ADCH - 105) & 0xEF;
//val = 2*ADCH - 105;
//  Serial.println(ADCH - 0x01101001);
  Serial.println(ewma());
//Serial.println(ADCH);
  ADCSRA |= 1 << ADSC;
}
)
