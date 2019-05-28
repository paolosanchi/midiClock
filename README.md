midiClock
=========

An arduino sketch to send a midi clock signal to a midi device. It Implements a tap tempo feature to set the bpm.

It is implemented using the "MIDI Shield" (search it on google to buy or build your one) and the MIDILibrary. Button.h is used to handle the button press, and a modified version of movingAvg.
At last it uses TapTempo, that manages the logic to evaluate the bpm, by pressing the button.

The libraries folder contains all the dependencies:
-----
- http://playground.arduino.cc/Main/MIDILibrary
- https://github.com/JChristensen/movingAvg
- http://playground.arduino.cc/Code/Button
- https://github.com/paolosanchi/TapTempo

It is inspired by this:
- https://github.com/gretel/MIDIsync
