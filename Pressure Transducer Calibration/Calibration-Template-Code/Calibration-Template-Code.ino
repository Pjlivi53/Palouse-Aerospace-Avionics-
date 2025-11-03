//Typically at the start of code you include the neccesarry header files much like how you import neccesary modules in python, different but simmilar
/*
Avionics Team
Pressure Transducer Calibration Template
*/
int AnalogPin = //Place which pin you will be connecting to, chose from any of the Analog pins (They start with A then a number, for example A0)
int BaudRate = //This is the rate of your serial port, I recomend a minimum of 9600

void setup(){  //this only runs once during the begging
  // put your setup code here, to run once:
  Serial.begin(BaudRate);//This starts your serial port.
  Serial.println("ADC, Vout");
  delay(1000); //delay(milliseconds), "pauses" the code for a certain number of milli seconds, this delay delays the code by 1 second or 1000 milli-seconds
}

void loop(){//this continously runs
  int ADC;
  int Vout;//this data type is incorrect what type should it be instead
  if(){    //place a logical statement that if true the code reads from the arduino, if false it skips what is inside the if statement and loops back to the top of void loop()
    
    ADC = analogRead(AnalogPin);

    Vout = (ADC/1023.0) * 5.0;//equation for converting 

    Serial.println(ADC);
    Serial.print(",");
    Serial.print(Vout);
  }

  //should you put a delay here?
}
