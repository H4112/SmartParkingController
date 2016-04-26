const int outPin = 2; // Using FAST_IO
const int inPin = 3; // Using FAST_IO
const int redLed = 10;
const int orangeLed = 9;
const int greenLed = 8;

enum { FREE, LEAVING, TAKEN };

void setup() {

  Serial.begin(115200);
  pinMode(outPin, OUTPUT_FAST);
  pinMode(inPin, INPUT_FAST);
  pinMode(redLed, OUTPUT);
  pinMode(orangeLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
}

double dist = 2000;
int cpt = 0;
int nbTaken = 0;
int nbFree = 0;

void loop()
{
  // establish variables for duration of the ping, 
  // and the distance result in inches and centimeters:
  long duration;

  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  fastDigitalWrite(outPin, LOW);
  waitMicros(2);
  fastDigitalWrite(outPin, HIGH);
  waitMicros(10);
  fastDigitalWrite(outPin, LOW);

  // The same pin is used to read the signal from the PING))): a HIGH
  // pulse whose duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  duration = pulseIn(inPin, HIGH); // calls fastGpioPciDigitalRead

  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  dist = duration / 58.;
  if(dist < 30)
  {
    nbTaken++;
  }
  else
  {
    nbFree++;
  }
  cpt++;
  if(cpt == 100)
  {
    Serial.println(nbTaken);
    Serial.println(nbFree);
    if(nbTaken - nbFree > 50)
    {
      digitalWrite(redLed, LOW);
      digitalWrite(greenLed, HIGH);
    }
    else if(nbTaken - nbFree > 50)
    {
      digitalWrite(redLed, HIGH);
      digitalWrite(greenLed, LOW);
    }
    
    //exec("curl https://rsauget.fr/" + String(state) + ">/dev/ttyGS0 2>&1");
    cpt = 0;
    nbTaken = 0;
    nbFree = 0;
  }
}

void exec(String cmd) {
  char cmd_str[256];
  cmd.toCharArray(cmd_str, 128);
  system(cmd_str);
}

void waitMicros(int val)
{
  unsigned long a = micros();
  unsigned long b = micros();
  while((b-a) < val)
  {
    b = micros();
    if(a>b)
    {
      break;
    }
  }
}

