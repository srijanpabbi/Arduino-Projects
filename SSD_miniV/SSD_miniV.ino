//#define A 2
//#define B 3
//#define C 4
//#define D 5
//#define E 6
//#define F 7
//#define G 8
#define D1 9
#define D2 10
#define D3 11
#define D4 12
#define pot A0
//int pot = A0;
char* no[] = {"1111110", "0110000", "1101101", "1111001", "0110011", "1011011", "1011111", "1110000", "1111111", "1111011"};

void setup() {
  // put your setup code here, to run once:
  for (int i = 2; i <= 12; i++) {
    pinMode(i, OUTPUT);
  }
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
  for (int i = 2; i <= 8; i++) {
    if (no[b][i - 2] == '1') {
      digitalWrite(i, HIGH);
      delayMicroseconds(500);
      //delay(5);
      digitalWrite(i, LOW);
    }
    //    else digitalWrite(i,LOW);
  }
}

void displayDigitEfficient(int b){
  switch(b){
    case 1: 
  }
  
}

void analogDisplay() {
  while (1) {
    int analog = analogRead(A0);
    for (int j = 0; j < 20; j++) {
      int val = analog;
      for (int i = 9; i <= 12 && val!=0; i++) {
        digitalWrite(i, HIGH);
        int dig = val % 10;
        val = val / 10;
        displayDigit(dig);
        digitalWrite(i, LOW);
      }
      val = analog;
    }
  }
}

void countDisplay() {
  int count = 0;
  while (count < 1024) {
    for (int j = 0; j < 100; j++) {
      int val = count;
      for (int i = 9; i <= 12 && val != 0; i++) {
        digitalWrite(i, HIGH);
        int dig = val % 10;
        val = val / 10;
        displayDigit(dig);
        digitalWrite(i, LOW);
      }
      val = count;
    }
    count++;
  }
}



void loop() {
  // put your main code here, to run repeatedly:
//analogDisplay();
  countDisplay();

}


