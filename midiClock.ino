#include <TapTempo.h>
#include <movingAvg.h>
#include <Button.h>
#include <MIDI.h>

#define LEDRED 7
#define LEDTAP 6
#define SWPIN  2
#define TAP 3

Button btn = Button(SWPIN, BUTTON_PULLDOWN);
TapTempo tap;

void setup()
{
  pinMode(LEDRED, OUTPUT);
  pinMode(LEDTAP, OUTPUT);
  pinMode(SWPIN, INPUT);
  
  digitalWrite(SWPIN, HIGH);  
  
  tap.setup(TAP, 200000, 24);
  tap.valueUpdatedHandler(valueChanged);
    
  btn.clickHandler(onTempoClick);
  
  MIDI.begin(MIDI_CHANNEL_OMNI);  
  //Serial.begin(9600);
}

void loop()
{  
  MIDI.read();  
  clock();
  
  tap.flush();
  btn.isPressed();
}

void onTempoClick(Button &b)
{
  //Serial.println("TAP! ");
  tap.tap();
}

unsigned long nextClockTime = 0;
unsigned long clockDelay = 20000;
int tickCount = 0;
int lastAnalog = 0;
int analog = 0;


void clock(){  
  unsigned long currentTime = micros();  	
  if(currentTime > nextClockTime)
  {
    MIDI.sendRealTime(Clock);
    
    nextClockTime = currentTime + clockDelay;
  
    tickCount++;
    if(tickCount == 24)
    {
      tickCount = 0;
      digitalWrite(LEDRED, LOW);
    }
    else
    {
      digitalWrite(LEDRED, HIGH);
      digitalWrite(LEDTAP, HIGH);
    }     
  }
}

void valueChanged(int val)
{    
    //Serial.print("New value! ");
    //Serial.println(val, DEC);
    MIDI.sendRealTime(SystemReset);
    
    clockDelay = long(val) * 1000 / 24;
        
    nextClockTime = micros() + clockDelay;
    tickCount = 0;
    digitalWrite(LEDTAP, LOW);
}

