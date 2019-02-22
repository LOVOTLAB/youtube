int button = 7;
int led = 13;

void setup() {
  pinMode(button, INPUT);
  pinMode(led, OUTPUT);  
}

void loop() {
  if(digitalRead(button) == LOW){
    digitalWrite(led, HIGH);    
  } 
  else{         
    digitalWrite(led, LOW);      
  }
}
