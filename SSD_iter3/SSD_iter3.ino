volatile int digit = 1;
volatile unsigned int a;
int button_chk = 0;
#include <TimerOne.h>
char no[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
//char no[] = {B00111111, B00000110, B01011011, B01001111, B01100111, B01101101, B01111101, B00000111, B01111111, B01101111};
//void setup()
//{
//  DDRD = 0xFF;
//  Timer1.initialize(2500);
//  Timer1.attachInterrupt( timerIsr ); // attach the service routine here
//}

int c2[3] = {A2, A3, A4};
int c3[3] = {A2, A4, A3};
int c1[3] = {A3, A2, A4};
int c5[3] = {A3, A4, A2};
int c6[3] = {A4, A2, A3};
int c4[3] = {A4, A3, A2};

void charlie(int c[3]) {
  pinMode(c[0], OUTPUT);
  pinMode(c[1], OUTPUT);
  pinMode(c[2], INPUT);
  digitalWrite(c[0], HIGH);
  digitalWrite(c[1], LOW);
  //  delay(100);
}


void setup() {
  DDRB = 0x3F;
  DDRD = 0xFF;
  
  //  Timer1.attachInterrupt( timerIsr ); // attach the service routine here
  //  pinMode(pot, INPUT);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A4, OUTPUT);
  PORTC = 0x00;
}

void displayDigitEfficient(int b) {
  PORTD = no[b];
  delay(500);
//  PORTD = 0x00;
}

void mode1() {
  Timer1.initialize(2500);
  Timer1.attachInterrupt( timerIsr );
  while (analogRead(A1) > 100) {
    a = analogRead(A0);
  }
  detachInterrupt(timerIsr);
  Timer1.stop();
  delay(5);
}

void mode2() {
  a = 0;
  Timer1.initialize(2500);
  Timer1.attachInterrupt( timerIsr2 );
  while (analogRead(A1) > 100) {
    if (a == 65535) {
      a = 0;
    } else {
      button_chk = analogRead(A1);
      if (button_chk >= 675 && button_chk <= 685) {
        a++;
      }
    }
  }
  detachInterrupt(timerIsr2);
  Timer1.stop();
  delay(5);
}

void mode3() {
  PORTB = 0x00;
  while (analogRead(A1) > 100) {
    charlie(c1);
    delay(analogRead(A0) / 10);
    charlie(c2);
    delay(analogRead(A0) / 10);
    charlie(c3);
    delay(analogRead(A0) / 10);
    charlie(c4);
    delay(analogRead(A0) / 10);
    charlie(c5);
    delay(analogRead(A0) / 10);
    charlie(c6);
    delay(analogRead(A0) / 10);

  }
  PORTC = 0x00;
}

void mode4(){
  while (analogRead(A1) > 100){
    if(analogRead(A0) <= 170){
      charlie(c2);
      delay(1);
    }else if(analogRead(A0) <= 341){
      charlie(c2);
      delay(1);
      charlie(c3);
      delay(1);
    }else if(analogRead(A0) <= 511){
      charlie(c2);
      delay(1);
      charlie(c3);
      delay(1);
      charlie(c4);
      delay(1);
    }else if(analogRead(A0) <= 682){
      charlie(c2);
      delay(1);
      charlie(c3);
      delay(1);
      charlie(c4);
      delay(1);
      charlie(c5);
      delay(1);
    }else if(analogRead(A0) <= 852){
      charlie(c2);
      delay(1);
      charlie(c3);
      delay(1);
      charlie(c4);
      delay(1);
      charlie(c5);
      delay(1);
      charlie(c6);
      delay(1);
    }else if(analogRead(A0) <= 1023){
      charlie(c2);
      delay(1);
      charlie(c3);
      delay(1);
      charlie(c4);
      delay(1);
      charlie(c5);
      delay(1);
      charlie(c6);
      delay(1);
      charlie(c1);
      delay(1);
    }

    PORTC = 0x00;
  }
  PORTC = 0x00;
}

void mode5(){
  a = 0;
  Timer1.initialize(2500);
  Timer1.attachInterrupt( timerIsr2 );
  while (analogRead(A1) > 100) {
    Timer1.setPeriod(analogRead(A0)*33);
    if (a == 65535) {
      a = 0;
    } else {
      button_chk = analogRead(A1);
      if (button_chk >= 675 && button_chk <= 685) {
        a++;
      }
    }
  }
  detachInterrupt(timerIsr2);
  Timer1.stop();
  delay(5);
}

void loop() {
  mode1();
  while (analogRead(A1) <= 50);
  delay(5);
  mode2();
  while (analogRead(A1) <= 50);
  delay(5);
  mode3();
  while (analogRead(A1) <= 50);
  delay(2);
  mode4();
  while (analogRead(A1) <= 50);
  delay(5);
  mode5();
  while (analogRead(A1) <= 50);
  delay(5);
}

// --------------------------
//  Custom ISR Timer Routine
// --------------------------
void timerIsr() {
  if (digit == 5) {
    digit = 1;
  }
  PORTB = 0x00;
  if (digit == 1) {
    PORTB = 0x01;
    displayDigitEfficient(a % 10);
    digit++;
  }
  else if (digit == 2) {
    PORTB = 0x02;
    displayDigitEfficient((a % 100) / 10);
    digit++;
  }
  else if (digit == 3) {
    PORTB = 0x04;
    displayDigitEfficient((a % 1000) / 100);
    digit++;
  }
  else if (digit == 4) {
    PORTB = 0x08;
    displayDigitEfficient((a % 10000) / 1000);
    digit++;
  }
}

void timerIsr2() {
  if (digit == 5) {
    digit = 1;
  }
  PORTB = 0x00;
  if (digit == 1) {
    PORTB = 0x01;
    displayDigitEfficient(a % 16);
  }
  else if (digit == 2) {
    PORTB = 0x02;
    displayDigitEfficient((a / 16) % 16);
  }
  else if (digit == 3) {
    PORTB = 0x04;
    displayDigitEfficient((a / 256) % 16);
  }
  else if (digit == 4) {
    PORTB = 0x08;
    displayDigitEfficient((a / 4096) % 16);
  }
  digit++;
}



