String line;

const byte LED_PIN = 3;

void setup() {
  Serial1.begin(115200);
  Serial.begin(9600);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  Serial1.print("AT\r\n");
  delay(100);
}

void loop() {

  if (Serial1.available()) {

    line = Serial1.readStringUntil('\n');

    if (line.startsWith("+RCV=")) {
      Serial.println(line);
      line.remove(0,5);

      int a,b,x,c,d;

      sscanf(line.c_str(), "%d,%d,%d,%d,%d", &a, &b, &x, &c, &d);

      digitalWrite(LED_PIN, x);
    }
  }
}