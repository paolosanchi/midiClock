#include <TapTempo.h>

class TapTempo;
typedef void (*buttonEventHandler)(unsigned long&);

TapTempo::TapTempo()
{
	setup(3);
}

void TapTempo::setup(unsigned long defaultResetTimeSpan)
{
	setup(defaultResetTimeSpan, 200000);
}

void TapTempo::setup(unsigned long defaultResetTimeSpan, unsigned long minTapDiffTimeSpan)
{
	setup(defaultResetTimeSpan, minTapDiffTimeSpan, 1);
}

void TapTempo::setup(unsigned long defaultResetTimeSpan, unsigned long minTapDiffTimeSpan, int beatSubdivisions)
{
	_defaultResetTimeSpan = defaultResetTimeSpan * 1000000;
	_minTapDiffTimeSpan = minTapDiffTimeSpan;
	_beatSubdivisions = beatSubdivisions;
	_tapCounter = 0;
	reset();
}

void TapTempo::flush()
{	
	// check for timer timeout
	unsigned long currentTime = micros();
	if (currentTime > _resetTime && _tapCounter > 0)
	{
		reset();
	}
	
	// if someone is listening
	if (_beat)
	{
		if(currentTime > _beatTime && _lastValue != 0)
		{					
			_beat(_subBeatCounter == 0);
			_subBeatCounter = ++_subBeatCounter % _beatSubdivisions;
			updateBeatTime(currentTime);
		}
	}
}

void TapTempo::reset()
{
    Serial.println("Reset!");
	// restart tap counter
	tapTimeFilter.reset();
	_tapCounter = 0;
	_resetTimeSpan = _defaultResetTimeSpan;
	_subBeatCounter = 0;
}

int TapTempo::getLastValue()
{
	return _lastValue;
}

bool TapTempo::tap()
{		   	
	unsigned long currentTapTime = micros();	
	unsigned long tapDiff = currentTapTime - _lastTapTime;
	bool newValue = false;
	
	// filter too short taps
	if(tapDiff >= _minTapDiffTimeSpan)
	{		
		flush();	// in case the client forget it :)
		
		_tapCounter++;
		newValue = _tapCounter > 1;
		if(newValue)
		{		
			_lastValue = tapTimeFilter.reading(int(tapDiff / 1000));
						
			if (_tapCounter >= 3)
			{
				// fire the value updated event
				if (_valueUpdated) { _valueUpdated(_lastValue); }
			}
					
			_resetTimeSpan = long(_lastValue) * 1000 * 2;
		}
		
		// store current time
		_lastTapTime = currentTapTime;
		_resetTime = currentTapTime + _resetTimeSpan;
		updateBeatTime(currentTapTime);
	}
	
	return newValue;
}

void TapTempo::updateBeatTime(unsigned long currentTime)
{
	_beatTime = currentTime + ((long(_lastValue) * 1000) / _beatSubdivisions);
}

void TapTempo::valueUpdatedHandler(valueEventHandler handler)
{
	_valueUpdated = handler;
}

void TapTempo::beatHandler(beatEventHandler handler)
{
	_beat = handler;
}
