#include "PerlinNoise.h"

PerlinNoise::PerlinNoise(double frequency, double amplitude, double persistence, int octaves)
	: _frequency(frequency)
	, _amplitude(amplitude)
	, _persistence(persistence)
	, _octaves(octaves) {
	srand(time(NULL));

	for (int i = 0; i < 256; ++i) {
		p[i] = i;

		_gradX[i] = 2 * (double)rand() / RAND_MAX - (double)1;
		_gradY[i] = 2 * (double)rand() / RAND_MAX - (double)1;
		_gradZ[i] = 2 * (double)rand() / RAND_MAX - (double)1;
	}

	permutate();
}

double PerlinNoise::noise(double x, double y, double z) {
	int X = (int)floorf(x);
	int Y = (int)floorf(y);
	int Z = (int)floorf(z);
	x -= (double)X;
	y -= (double)Y;
	z -= (double)Z;

	double u = fade(x);
	double v = fade(y);
	double w = fade(z);

	int iGrad = p[(X + p[(Y + p[Z & 255]) & 255]) & 255];
	double d000 = _gradX[iGrad] * x + _gradY[iGrad] * y + _gradZ[iGrad] * z;
	iGrad = p[(X + 1 + p[(Y + p[Z & 255]) & 255]) & 255];
	double d001 = _gradX[iGrad] *  (x - 1.0f) + _gradY[iGrad] * y + _gradZ[iGrad] * z;

	iGrad = p[(X + p[(Y + 1 + p[Z & 255]) & 255]) & 255];
	double d010 = _gradX[iGrad] * x + _gradY[iGrad] *  (y - 1.0f) + _gradZ[iGrad] * z;
	iGrad = p[(X + 1 + p[(Y + 1 + p[Z & 255]) & 255]) & 255];
	double d011 = _gradX[iGrad] *  (x - 1.0f) + _gradY[iGrad] *  (y - 1.0f) + _gradZ[iGrad] * z;

	iGrad = p[(X + p[(Y + p[(Z + 1) & 255]) & 255]) & 255];
	double d100 = _gradX[iGrad] * x + _gradY[iGrad] * y + _gradZ[iGrad] *  (z - 1.0f);
	iGrad = p[(X + 1 + p[(Y + p[(Z + 1) & 255]) & 255]) & 255];
	double d101 = _gradX[iGrad] *  (x - 1.0f) + _gradY[iGrad] * y + _gradZ[iGrad] *  (z - 1.0f);

	iGrad = p[(X + p[(Y + 1 + p[(Z + 1) & 255]) & 255]) & 255];
	double d110 = _gradX[iGrad] * x + _gradY[iGrad] *  (y - 1.0f) + _gradZ[iGrad] *  (z - 1.0f);
	iGrad = p[(X + 1 + p[(Y + 1 + p[(Z + 1) & 255]) & 255]) & 255];
	double d111 = _gradX[iGrad] *  (x - 1.0f) + _gradY[iGrad] *  (y - 1.0f) + _gradZ[iGrad] *  (z - 1.0f);

	double xa = lerp(u, d000, d001);
	double xb = lerp(u, d010, d011);
	double xc = lerp(u, d100, d101);
	double xd = lerp(u, d110, d111);
	double ya = lerp(v, xa, xb);
	double yb = lerp(v, xc, xd);
	double value = lerp(w, ya, yb);

	return value;
}

double PerlinNoise::usingOctave(double x, double y, double z) {
	double total = 0;
	double max = 0;
	double frequency = _frequency;
	double amplitude = _amplitude;
	for (int i = 0; i < _octaves; i++) {
		total += noise(x * _frequency, y * _frequency, z * _frequency) * _amplitude;

		max += _amplitude;

		amplitude *= _persistence;
		frequency *= 2;
	}

	return total / max;
}

double PerlinNoise::getFrequency() {
	return _frequency;
}

double PerlinNoise::getAmplitude() {
	return _amplitude;
}

double PerlinNoise::getPersistence() {
	return _persistence;
}

int PerlinNoise::getOctaves() {
	return _octaves;
}

void PerlinNoise::setFrequency(double frequency) {
	_frequency = frequency;
}

void PerlinNoise::setAmplitude(double amplitude) {
	_amplitude = amplitude;
}

void PerlinNoise::setPersistence(double persistence) {
	_persistence = persistence;
}

void PerlinNoise::setOctaves(int octaves) {
	_octaves = octaves;
}

void PerlinNoise::permutate() {
	for (int i = 255; i >= 0; --i) {
		int j = rand() % (i + 1);
		std::swap(p[i], p[j]);
	}
}


double PerlinNoise::fade(double t) {
	return t * t * t * (t * (t * 6 - 15) + 10);
}

double PerlinNoise::lerp(double t, double a, double b) {
	return a + (b - a) * t;
}

double PerlinNoise::grad(int hash, double x, double y, double z) {
	switch (hash % 12) {
	case 0x0: return  x + y;
	case 0x1: return -x + y;
	case 0x2: return  x - y;
	case 0x3: return -x - y;
	case 0x4: return  x + z;
	case 0x5: return -x + z;
	case 0x6: return  x - z;
	case 0x7: return -x - z;
	case 0x8: return  y + z;
	case 0x9: return -y + z;
	case 0xA: return  y - z;
	case 0xB: return -y - z;
	case 0xC: return  y + x;
	case 0xD: return -y + z;
	case 0xE: return  y - x;
	case 0xF: return -y - z;
	default: return 0;
	}
}