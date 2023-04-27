#include "header.h"

void setup()
{
  Serial.begin(9600);
  myservo.write(140); // sätter sensor till 140
  Wire.begin();
  myservo.attach(servoPin); //sätter igång servo
  // Wait for the serial port to be opened before printing
  // messages (only applies to boards with native USB).
  while (!Serial) {}

  pinMode(remotePin, INPUT); //RemotePin pin till inport
  pinMode(enA, INPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  

  sensor.init();
  if (sensor.getLastError())
  {
    Serial.print(F("Failed to initialize OPT3101: error "));
    Serial.println(sensor.getLastError());
    while (1) {}
  }

  sensor.setContinuousMode();
  sensor.enableDataReadyOutput(1);
  sensor.setFrameTiming(32);
  sensor.setChannel(OPT3101ChannelAutoSwitch);
  sensor.setBrightness(OPT3101Brightness::Adaptive);
  attachInterrupt(digitalPinToInterrupt(dataReadyPin), setDataReadyFlag, RISING);
  sensor.enableTimingGenerator();
}

void loop()
{
  leftsensor = distances[0]; // Döper om vänster sensor från distances[0] till leftsensor
  middlesensor = distances[1]; // Döper om mitt sensor från distances[1] till middlesensor
  rightsensor = distances[2]; // Döper om höger sensor från distances[2] till rightsensor
  remoteVal = digitalRead(remotePin); // Läser av insignalen från startmodulen.

  if(remoteVal)
  {
   digitalWrite(in1, HIGH);
   digitalWrite(in2, LOW);

   if (sensor.isSampleDone())
   {
      sensor.readOutputRegs();
      distances[sensor.channelUsed] = sensor.distanceMillimeters;

      if (sensor.channelUsed == 2)
      {
      
       for (uint8_t i = 0; i < 3; i++)
       {
        if (leftsensor<Safe && leftsensor<rightsensor) // if för höger sväng
        {
          angle = map(leftsensor, Safe, NotSafe, Straight, TurnRight); // map funktion för smidigare sväng
          myservo.write(angle);
        }
        else if (rightsensor<Safe && rightsensor<leftsensor) // if för vänster sväng
        {
          angle = map(rightsensor, Safe, NotSafe, Straight, TurnLeft);
          myservo.write(angle);
        }
        else if (middlesensor<NotSafe && rightsensor<leftsensor) // if för höger sväng men om väggen framför närmar sig
        {
          angle = map(rightsensor, Safe, Near, Straight, TurnRight);
          myservo.write(angle);
        }
        else if (middlesensor<NotSafe && rightsensor>leftsensor) // if för vänster sväng men om väggen framför närmar sig
        {
          angle = map(rightsensor, Safe, Near, Straight, TurnLeft);
          myservo.write(angle);
        }
        else myservo.write(Straight); // bilen kör framåt
        }
     }
     sensor.nextChannel();
     sensor.startSample();
   }
  }
}
