#pragma once

class GeneralSettings {
public:
	static inline int NbOfParticles() { return NB_OF_PARTICLES; }
private:
	static const int NB_OF_PARTICLES = 2000;
};