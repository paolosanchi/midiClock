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
  tap.beatHandler(beat);
    
  btn.clickHandler(onTempoClick);
  
  MIDI.begin(MIDI_CHANNEL_OMNI);  
  //Serial.begin(9600);
}

void loop()
{  
  MIDI.read();  
  
  tap.flush();
  btn.isPressed();
}

void onTempoClick(Button &b)
{
  //Serial.println("TAP! ");
  tap.tap();
}

void beat(bool mainBeat)
{
  MIDI.sendRealTime(Clock);
 
  if(mainBeat)
      digitalWrite(LEDRED, LOW);
  else 
  {
    digitalWrite(LEDRED, HIGH);
    digitalWrite(LEDTAP, HIGH);
  }     
}

void valueChanged(int val)
{    
    //Serial.print("New value! ");
    //Serial.println(val, DEC);
    
    MIDI.sendRealTime(Stop);
    MIDI.sendRealTime(Start);
    
    digitalWrite(LEDTAP, LOW);
}

