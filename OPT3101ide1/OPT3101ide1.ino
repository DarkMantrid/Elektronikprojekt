#include "header.h"

void setup()
{
  Serial.begin(9600);
  myservo.write(Straight); // sätter sensor till Straight
  Wire.begin();
  myservo.attach(servoPin); //sätter igång servo
  // Wait for the serial port to be opened before printing
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
  leftsensor = distances[0]; 
  middlesensor = distances[1]; 
  rightsensor = distances[2]; 
  remoteVal = digitalRead(remotePin); // Läser av insignalen från startmodulen.

  if(remoteVal == HIGH)
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
        if (leftsensor < Safe && leftsensor < rightsensor) 
        {
          angle = map(leftsensor, Safe, NotSafe, Straight, TurnRight); 
          myservo.write(angle);
        }
        else if (rightsensor < Safe && rightsensor < leftsensor) 
        {
          angle = map(rightsensor, Safe, NotSafe, Straight, TurnLeft);
          myservo.write(angle);
        }
        else if (middlesensor < NotSafe && leftsensor < rightsensor) 
        {
          angle = map(rightsensor, Safe, Near, Straight, TurnRight);
          myservo.write(angle);
        }
        else if (middlesensor < NotSafe && rightsensor < leftsensor) 
        {
          angle = map(rightsensor, Safe, Near, Straight, TurnLeft);
          myservo.write(angle);
        }
        else myservo.write(Straight); 
        }
     }
     sensor.nextChannel();
     sensor.startSample();
   }
  }
  else if (remoteVal == LOW)
  {
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
  }
}
