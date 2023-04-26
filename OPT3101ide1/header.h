// Include the servo library:
#include <Servo.h> // inkluderar bibliotek för servo
#include <OPT3101.h> // inkluderar bibliotek för sensor
#include <Wire.h>
#include <L298N.h>
Servo myservo; // Create a new servo object:
#define servoPin 5 // definerar servos pin

#define enA 8
#define in1 6
#define in2 7

#define remotePin 4 // Pin nummer för start modulens remote pin
int remoteVal = 0; //värde för startmodul

int In1 = 6; //motor low pin
int In2 = 7; //motor high pin
int SPEED = 255; // motor hastighet, högsta.

const uint8_t dataReadyPin = 2; // Pin nummer för sensor läsning
int angle = 90; // Döper svängningsvinkel till angle
int leftsensor = 0;
int middlesensor = 1;
int rightsensor = 2;
// Namn för olika sväng vinklar för lättare ändring av svängvinkel
int Straight = 140; // Döper rakt vinkel
int TurnAngle = 15; // Det här är hur mycket servon kan svänga för varje riktning.
int TurnLeft = Straight - TurnAngle; // Rakt - svängvinkel gör att bilen svänger till vänster om TurnLeft anropas.
int TurnRight = Straight + TurnAngle; // Rakt + svängvinkel gör att bilen svänger till höger om TurnRight anropas.
// namn för avstånd mätning
int Near = 200;
int NotSafe = 450;
int Safe = 600;
OPT3101 sensor; // döper sensorn
int16_t distances[3]; // aktiverar tre punkter på sensor och döper dem till distances
volatile bool dataReady = false;
void setDataReadyFlag()
{
dataReady = true;
}
