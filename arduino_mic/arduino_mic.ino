int a=0;
void setup() {
  // put your setup code here, to run once:
  pinMode(A0,INPUT);
  pinMode(6,OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  a=analogRead(A0);
  //a=(255.0/1024)*a;
  Serial.println(a);
  analogWrite(6,a);
}
