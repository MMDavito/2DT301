int ledRED = 0;
int ledYELLOW=1;
int ledGREEN=2;
int ledBY=3;

void setup () {
  
  pinMode(ledRED, OUTPUT);
  pinMode(ledYELLOW, OUTPUT);
  pinMode(ledGREEN, OUTPUT);
  pinMode(ledBY, OUTPUT);
  
}

void loop () {
  
  digitalWrite(ledRED, HIGH);
  delay(500);
  digitalWrite(ledRED, LOW);
  digitalWrite(ledYELLOW, HIGH);
  delay(500);
  digitalWrite(ledYELLOW, LOW);
  digitalWrite(ledGREEN, HIGH);
  delay(500);
  digitalWrite(ledGREEN, LOW);
  digitalWrite(ledBY, HIGH);
  delay(500);
  digitalWrite(ledBY, LOW);
  delay(500);
}
