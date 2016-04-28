const int controllerId = 1;

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

double sum = 0;
int cpt = 0;
const int tick = 70;
unsigned long takenSince = 0;
const unsigned long orangeDelay = 1 * 1000;// ms
int state = FREE;
int targetState = FREE;
int nbOccurs = 0;
double carHeight = 0;
double carHeightSum = 0;
int nbCarHeight = 0;
int nbLeaving = 0;

void loop()
{
  // establish variables for duration of the ping, 
  // and the distance result in inches and centimeters:
  long duration;

  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  fastDigitalWrite(outPin, LOW);
  delayMicroseconds(2);
  fastDigitalWrite(outPin, HIGH);
  delayMicroseconds(10);
  fastDigitalWrite(outPin, LOW);

  // The same pin is used to read the signal from the PING))): a HIGH
  // pulse whose duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  duration = pulseIn(inPin, HIGH); // calls fastGpioPciDigitalRead

  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  double dist = duration / 58.;
  if(dist > 60) dist = 60;
  //Serial.println(dist);

  sum += dist;
  
  cpt++;
  
  if(cpt == tick)
  {
    double avg = sum / tick;
    Serial.print("avg : ");
    Serial.println(avg);
    Serial.print("car height :");
    Serial.println(carHeight);
    Serial.print("current state: ");
    Serial.println(state);
    if(avg < carHeight - 1 && (millis() - takenSince) > orangeDelay)
    {
      if(targetState == LEAVING)
      {
        nbOccurs++;
      }
      else
      {
        targetState = LEAVING;
        nbOccurs = 1;
      }
    }
    else if(avg < 30)
    {
      if(targetState == TAKEN)
      {
        nbOccurs++;
      }
      else
      {
        targetState = TAKEN;
        nbOccurs = 1;
      }
    }
    else
    {
      if(targetState == FREE)
      {
        nbOccurs++;
      }
      else
      {
        targetState = FREE;
        nbOccurs = 1;
      }
    }

    if(state == TAKEN && (millis() - takenSince) > orangeDelay && carHeight == 0)
    {
      if(nbCarHeight == 10)
      {
        carHeight = carHeightSum / nbCarHeight;
      }
      else
      {
        carHeightSum += avg;
        nbCarHeight++;
      }
      
    }
    Serial.print("target state: ");
    Serial.println(targetState);
    Serial.print("occurs: ");
    Serial.println(nbOccurs);
    if(nbOccurs == 5)
    {
      if(state == FREE && targetState == TAKEN)
      {
        takenSince = millis();
      }
      if(targetState == FREE)
      {
        carHeight = 0;
        carHeightSum = 0;
        nbCarHeight = 0;
      }
      state = targetState;
      exec("curl -X PUT -d'etat=" + String(state) + "' 'https://parking.rsauget.fr:8080/sensors/" + String(controllerId) + "/' >/dev/ttyGS0 2>&1");
    }
    
    switch(state)
    {
      case TAKEN:
        digitalWrite(redLed, HIGH);
        digitalWrite(orangeLed, LOW);
        digitalWrite(greenLed, LOW);
        break;
      case LEAVING:
        digitalWrite(redLed, HIGH);
        digitalWrite(orangeLed, HIGH);
        digitalWrite(greenLed, LOW);
        break;
      case FREE:
        digitalWrite(redLed, LOW);
        digitalWrite(orangeLed, LOW);
        digitalWrite(greenLed, HIGH);
        break;
    }

    cpt = 0;
    sum = 0;
  }
  delay(10);
}

void exec(String cmd) {
  char cmd_str[256];
  cmd.toCharArray(cmd_str, 128);
  Serial.println(cmd_str);
  system(cmd_str);
}

