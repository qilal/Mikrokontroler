int ledPin = 2;  
String inputString = "";
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
  while (Serial.available() > 0) {
    char c = Serial.read();

    if (c != '\n' && c != '\r') {
      inputString += c;
    } else if (inputString.length() > 0) {
      // --- Proses perintah ---
      if (inputString == "1") {
        digitalWrite(ledPin, HIGH);
        digitalWrite(motor1Pin1, HIGH);
        digitalWrite(motor1Pin2, LOW);
        ledcWrite(pwmChannel, motorSpeed);
        Serial.println("Motor ON");
      } 
      else if (inputString == "0") {
        digitalWrite(ledPin, LOW);
        digitalWrite(motor1Pin1, LOW);
        digitalWrite(motor1Pin2, LOW);
        ledcWrite(pwmChannel, 0);
        Serial.println("Motor OFF");
      } 
      else {
        int speedVal = inputString.toInt();
        if (speedVal >= 0 && speedVal <= 255) {
          motorSpeed = speedVal;
          ledcWrite(pwmChannel, motorSpeed);
          Serial.print("PWM Speed: ");
          Serial.println(motorSpeed);
        }
      }
      inputString = ""; // reset buffer
    }
  }
}
