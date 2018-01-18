/*
*
* A very trivial PPM to USB Joystick converter, for teensy controllers
* Time between falling edges is measured and converted to joystick axis.
* Additionnally, the ranges are auto scaled.
*
*/

// Maximum number of channels
#define NB_PEAKS 15
#define PPM_PIN  2

unsigned long int before;

typedef struct {
  int val;
  int min;
  int max;
} PEAK;

PEAK peaks[NB_PEAKS];

//specifing arrays and variables to store values

void readPin()  {

  static int i = 0;
  unsigned int now = micros(); //store time value a when pin value is falling
  unsigned diff = now-before;  //calculating time inbetween two peaks
  before = now;        // 

  if (diff > 6000) {
    i=0;
    return;
  }

  if (i++>NB_PEAKS)
    i = 0;

  peaks[i].val=diff;

}

unsigned int valToPos(int i) {
  PEAK * pPeak = &peaks[i];
  int range = pPeak->max-pPeak->min;

  if (range == 0)
    return 0;
  
  return ((pPeak->val-pPeak->min)*1024)/range;
}

bool valToButton(int i) {
  int v = valToPos(i);
  if (v > 512)
    return true;
  else
    return false;
}
             
void getPeaks(){

  for (int i=0;i<NB_PEAKS;i++) {
    PEAK * pPeak = &peaks[i];
    if (pPeak->min>pPeak->val && pPeak->val != 0)
      pPeak->min = pPeak->val;

    if (pPeak->max < pPeak->val)
      pPeak->max = pPeak->val;
      
 #ifdef DEBUG
    Serial.print(x[i]);

    Serial.print("(");
    Serial.print(xMin[i]);
    Serial.print(",");
    Serial.print(xMax[i]);
    Serial.print(")->");
    Serial.print(valToPos(i));

    Serial.print("\t");
 #endif

    
  }
#ifdef DEBUG
  Serial.println("");
#endif // DEBUG

  Joystick.X(valToPos(1));
  Joystick.Y(valToPos(2));
  Joystick.Z(valToPos(3));
  Joystick.Zrotate(valToPos(4));

  Joystick.sliderLeft(valToPos(5));
  Joystick.sliderRight(valToPos(6));

  Joystick.button(1, valToButton(7));
  Joystick.button(2, valToButton(8));
  Joystick.button(3, valToButton(9));


}     


void setup()
{
#ifdef DEBUG
  Serial.begin(9600);
#endif // DEBUG  
  pinMode(PPM_PIN, INPUT_PULLUP);
  Joystick.useManualSend(true);
  for (int i=0; i<NB_PEAKS;i++) {
    PEAK * pPeak = &peaks[i];
    pPeak->min=1000;
    pPeak->max=2000;
  }
  
  attachInterrupt(digitalPinToInterrupt(2), readPin , FALLING);
}

void loop()
{
  getPeaks();

  Joystick.send_now();
  
  delay(20);  // do not print too fast!
}
