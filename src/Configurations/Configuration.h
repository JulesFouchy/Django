#pragma once

class ParticlesSystem;

class Configuration {
public:
	Configuration() = default;
	virtual ~Configuration() = default;

	virtual void reroll(unsigned int nbParticles) {};
	virtual void applyTo(ParticlesSystem& particlesSystem) = 0;
};