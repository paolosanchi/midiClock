#include <TapTempo.h>
#include <movingAvg.h>
#include <Button.h>

#define TAP 2

TapTempo tap;
Button btn = Button(TAP, BUTTON_PULLDOWN);

void setup()
{  
  Serial.begin(9600);
  Serial.println("Init..");
  
  tap.setup(TAP, 200000, 3);
  tap.valueUpdatedHandler(valueChanged);  
  tap.beatHandler(beat);
  
  btn.clickHandler(onTempoClick);  
  
  Serial.println("START!");
}

void loop()
{  
  tap.flush();
  btn.isPressed();
}

void onTempoClick(Button &b)
{
  Serial.println(" ");
  Serial.println("TAP!");
  tap.tap();
}

void beat(bool mainBeat)
{  
  if(mainBeat)
    Serial.println("BEAT!");
  else  {
    Serial.println("SUB-BEAT!");
  }
}

void valueChanged(int val)
{
    Serial.print("New value!");
    Serial.println(val, DEC);
}