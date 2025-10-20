int ledPin = 2;  
char myData = 0;
int motorSpeed = 0; // nilai PWM dari 0–255

int motor1Pin1 = 27;
int motor1Pin2 = 26;
int enable1Pin = 12;

const int freq = 30000;
const int pwmChannel = 0;
const int resolution = 8;

void setup() {
  pinMode(ledPin, OUTPUT);  
  Serial.begin(115200);

  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(enable1Pin, OUTPUT);

  ledcSetup(pwmChannel, freq, resolution);
  ledcAttachPin(enable1Pin, pwmChannel);

  Serial.println("Ready...");
}

void loop() {
  if (Serial.available() > 0) {
    myData = Serial.read();

    // ON
    if (myData == '1') {
      digitalWrite(ledPin, HIGH);
      digitalWrite(motor1Pin1, HIGH);
      digitalWrite(motor1Pin2, LOW);
      ledcWrite(pwmChannel, motorSpeed);
      Serial.println("Motor ON");
    }
    // OFF
    else if (myData == '0') {
      digitalWrite(ledPin, LOW);
      digitalWrite(motor1Pin1, LOW);
      digitalWrite(motor1Pin2, LOW);
      ledcWrite(pwmChannel, 0);
      Serial.println("Motor OFF");
    }
    // Jika dikirimkan angka 3 digit (misalnya 128)
    else if (isdigit(myData)) {
      motorSpeed = (motorSpeed * 10 + (myData - '0')) % 1000; 
      // Menyimpan angka hingga lengkap (0-255)
    }
  }
}
