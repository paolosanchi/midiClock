#ifndef TapTempo_h
#define TapTempo_h
#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h> 
#else
#include <WProgram.h> 
#endif

#include <movingAvg.h>

typedef void (*valueEventHandler)(int);
typedef void (*beatEventHandler)(bool);

class TapTempo
{
	public:
		TapTempo();
		
		void setup(unsigned long defaultResetTimeSpan);
		void setup(unsigned long defaultResetTimeSpan, unsigned long minTapDiffTimeSpan);
		void setup(unsigned long defaultResetTimeSpan, unsigned long minTapDiffTimeSpan, int beatSubdivisions);

		bool tap();		
		void flush();		
		void reset();		
		void valueUpdatedHandler(valueEventHandler handler);
		void beatHandler(beatEventHandler handler);
		int getLastValue();
		
	private:
		unsigned int _lastValue;
		unsigned int _tapCounter;
		unsigned int _subBeatCounter;
		unsigned long _lastTapTime;
		unsigned long _resetTime;
		unsigned long _defaultResetTimeSpan;
		unsigned long _minTapDiffTimeSpan;
		unsigned long _resetTimeSpan;
		unsigned long _beatTime;
		int _beatSubdivisions;
		movingAvg tapTimeFilter;
		valueEventHandler _valueUpdated;
		beatEventHandler _beat;
		
		void updateBeatTime(unsigned long currentTime);
};

#endif
