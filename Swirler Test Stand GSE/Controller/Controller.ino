const byte SWITCH_PIN = 2;
const byte LED_PIN = 3;

bool state = false;
bool buttonState = HIGH;
bool lastReading = HIGH;

unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

void setup()
{
  Serial.begin(9600);
  Serial1.begin(115200);

  pinMode(SWITCH_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);

  updateLED();
}

void loop()
{
  bool reading = digitalRead(SWITCH_PIN);

  if (reading != lastReading)
  {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay)
  {
    if (reading != buttonState)
    {
      buttonState = reading;

      if (buttonState == LOW)
      {
        state = !state;
        updateLED();

        if (state == true)
        {
          send_data(1, 1);
          Serial.println("State TRUE -> sent 1");
        }

        if (state == false)
        {
          send_data(0, 1);
          Serial.println("State FALSE -> sent 0");
        }
      }
    }
  }

  lastReading = reading;
}

void updateLED()
{
  digitalWrite(LED_PIN, state);
}

void send_data(int i, int valuelength)
{
  String mymessage;
  mymessage = "AT+SEND=0," + String(valuelength) + "," + String(i);
  Serial1.println(mymessage);
  Serial.println(i);
}