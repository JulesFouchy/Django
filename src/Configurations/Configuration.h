#pragma once

class ParticlesSystem;

class Configuration {
public:
	Configuration() = default;
	virtual ~Configuration() = default;

	virtual bool reroll(unsigned int nbParticles) { return false; } // returns whether this actually changes anything
	virtual void applyTo(ParticlesSystem& particlesSystem) = 0;
};