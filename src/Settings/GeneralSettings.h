#pragma once

class GeneralSettings {
public:
	static void Initialize();

	static inline int NbOfParticles() { return NB_OF_PARTICLES; }
	static void SetNbOfParticles(int N);
private:
	static int NB_OF_PARTICLES;
};