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
double ssum = 0;
int cpt = 0;
const int tick = 100;

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
  double dist = duration / 58.;
  
  sum += dist;
  ssum += (dist*dist);
  
  cpt++;
  
  if(cpt == tick)
  {
    double avg = sum / tick;
    double var = ssum / tick - (avg * avg);
    Serial.print("avg : ");
    Serial.println(avg);
    Serial.print("var : ");
    Serial.println(var);
    if(avg < 30)
    {
      digitalWrite(redLed, HIGH);
      digitalWrite(greenLed, LOW);
      if(var > 1)
      {
        digitalWrite(orangeLed, HIGH);
      }
      else
      {
        digitalWrite(orangeLed, LOW);
      }
      
    }
    else
    {
      digitalWrite(redLed, LOW);
      digitalWrite(greenLed, HIGH);
      digitalWrite(orangeLed, LOW);
    }

    //exec("curl https://rsauget.fr/" + String(state) + ">/dev/ttyGS0 2>&1");
    cpt = 0;
    sum = 0;
    ssum = 0;
  }
  delay(1);
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

