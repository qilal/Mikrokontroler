int ledPin = 2;  
char myData = 0;

int motor1Pin1 = 27;
int motor1Pin2 = 26;
int enable1Pin = 12;

const int freq = 30000;
const int pwmChannel = 0;
const int resolution = 8;
int dutyCycle = 200;

void setup() {
  pinMode(ledPin, OUTPUT);  
  Serial.begin(115200);

  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(enable1Pin, OUTPUT);

  ledcSetup(pwmChannel, freq, resolution);
  ledcAttachPin(enable1Pin, pwmChannel);

  Serial.println("Ready to receive commands...");
}

void MotorOn() {
  dutyCycle = 205;
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  ledcWrite(pwmChannel, dutyCycle);
}

void MotorOff() {
  dutyCycle = 0;
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  ledcWrite(pwmChannel, dutyCycle);
}

void loop() {
  myData = Serial.read();

  if (myData == '1') {  
    Serial.println("Motor ON");
    digitalWrite(ledPin, HIGH);
    MotorOn();
  } 
  else if (myData == '0') {  
    Serial.println("Motor OFF");
    digitalWrite(ledPin, LOW);
    MotorOff();
  }
}
