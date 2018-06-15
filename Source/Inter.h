#pragma once 
#include "../JuceLibraryCode/JuceHeader.h"

enum class Options {
	RESET,
	OCTAVES,
	PERSISTENCE,
	WIDTH,
	HEIGHT,
	DEPTH,
	ROCKINESS,
	MODE,
	FILL,
	LINES,
	TOPC,
	FARC,
	NEARC,
	BACKGROUND_COLOR
};

class Inter {
public:
	enum class Modes {
		DEFAULT,
		STATIC,
		FLY,
		CHANGE
	};

	~Inter(){}
	virtual void sendData(Options option, double d = 0.0, int i = 0, Modes mode = Modes::DEFAULT, Colour = Colour(0)) = 0;
	virtual bool isFree() = 0;
};

struct BackgoundColor {
	~BackgoundColor(){}
	virtual void setColor(Colour c) = 0;
};
