#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "PerlinNoise.h"

class PlaygroundComponent
	: public Component
{
public:
	PlaygroundComponent();
	~PlaygroundComponent();
	void resized() override;
	void paint(Graphics &g) override;
	void mouseMove(const MouseEvent &e) override;
	/*void initialise() override;
	void shutdown() override;
	void render() override;*/

private:
	//OpenGLContext _openGLContext;
	PerlinNoise _noise;
};

