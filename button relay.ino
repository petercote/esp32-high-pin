const int button = 5;
const int relayPin1 = 4;
bool relay1State;

uint8_t debounceThreshold = 40;  //Set the debounce threshold for the button
const int relayThreshold = 300;

void setup() {
  Serial.begin(115200);  //Start serial communication
  Serial.println("Starting");
  pinMode(button, INPUT_PULLUP);  //Set the button pin as an input with internal pull-up resistor
  pinMode(relayPin1, OUTPUT);
  relay1State = false;
}

void loop() {
  uint32_t TimeSinceLastButtonReading;
  uint32_t relay1_startTime;
  uint32_t relay1_OpenTime;
  int buttonReading;
  uint32_t currentMillis = millis();
  static uint32_t TimeOfLastButtonReading = currentMillis;
  static int buttonState = digitalRead(button);

  //Calculate the time since the last button reading
  TimeSinceLastButtonReading = currentMillis - TimeOfLastButtonReading;  //If enough time has passed for debounce, read the button state
  if (TimeSinceLastButtonReading > debounceThreshold) {
    buttonReading = digitalRead(button);
    if (buttonReading != buttonState) {  //If the button state has changed, update the button state variable
      buttonState = buttonReading;
      if (buttonState == LOW) {  //If the button is pressed, do this
        if (relay1State == false) {
          Serial.println("Q PRESSED, RELAY 1 OPEN");
          digitalWrite(relayPin1, HIGH); //activate relay
          relay1State = true;
          relay1_startTime = millis();
        }
      }
      TimeOfLastButtonReading = currentMillis;  //sets time of last button reading to ensure time has passed before a subsequent button reading occurs
    }
  }

  if (relay1State == true) {
    relay1_OpenTime = (millis() - relay1_startTime);
    if (relay1_OpenTime > relayThreshold) {
      Serial.print("Relay Open Time Is ");
      Serial.println(relay1_OpenTime);
      digitalWrite(relayPin1, LOW);
      relay1State = false;
      Serial.println("RELAY 1 CLOSED");
    }
  }
}
