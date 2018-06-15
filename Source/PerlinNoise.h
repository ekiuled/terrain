#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include <ctime>
#include <cmath>

class PerlinNoise {
public:
	PerlinNoise(double frequency = 1.0, double amplitude = 1.0, double persistence = 0.5, int octaves = 3);

	double noise(double x, double y = 0.0, double z = 0.0);
	double usingOctave(double x, double y = 0.0, double z = 0.0);

	double getFrequency();
	double getAmplitude();
	double getPersistence();
	int getOctaves();

	void setFrequency(double frequency);
	void setAmplitude(double amplitude);
	void setPersistence(double persistence);
	void setOctaves(int octaves);
	void permutate();

private:
	double fade(double t);
	double lerp(double t, double a, double b);
	double grad(int hash, double x, double y, double z);


	double _frequency;
	double _amplitude;
	double _persistence;
	int _octaves;
	int p[256];
	double _gradX[256];
	double _gradY[256];
	double _gradZ[256];

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PerlinNoise)
};