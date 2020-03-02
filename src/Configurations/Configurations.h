#pragma once

class ParticlesSystem;

class Configuration {
public:
	Configuration() = default;
	virtual ~Configuration() = default;

	virtual void embody(ParticlesSystem& particlesSystem) = 0;
	virtual void generate(unsigned int nbParticles) {};
};