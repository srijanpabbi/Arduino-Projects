
#define D1 9
#define D2 10
#define D3 11
#define D4 12
#define pot A0
//int pot = A0;
char no[] = {B00111111, B00000110, B01011011, B01001111, B01100111, B01101101, B01111101, B00000111, B01111111, B01101111};

void setup() {
  // put your setup code here, to run once:
  for (int i = 2; i <= 12; i++) {
    pinMode(i, OUTPUT);
  }
  DDRD = B00000000;
  DDRB = B00000000;
  pinMode(pot, INPUT);
  //  pinMode(9,OUTPUT);
  //  digitalWrite(9,HIGH);
}
 void displayDigit(int b) {
  //  for(int i = 2; i<=8;i++){
  //    digitalWrite(i,HIGH);
  //    delay(500);
  //    digitalWrite(i,LOW);
  //  }
//  for (int i = 2; i <= 8; i++) {
//    if (no[b][i - 2] == '1') {
//      digitalWrite(i, HIGH);
//      delayMicroseconds(500);
//      //delay(5);
//      digitalWrite(i, LOW);
//    }
    //    else digitalWrite(i,LOW);
//  }
}

void displayDigitEfficient(int b){
//  switch(b){
//    case 1: 
//  }
    PORTD = no[b];
    delay(500);
    PORTD = B00000000;
  
}


int count = 0;
void loop() {
  // put your main code here, to run repeatedly:
//analogDisplay();
//  countDisplay();
if(count == 10)count = 0;
displayDigitEfficient(count);
count++;
//delay(100);


}


