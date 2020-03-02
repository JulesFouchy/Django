#pragma once

class ParticlesSystem;

class Configuration {
public:
	Configuration()
		: m_nbParticles(-1)
	{}
	virtual ~Configuration() = default;

	virtual bool setup(unsigned int nbParticles) {
		// returns wheter nbParticles changed
		bool change = (m_nbParticles != nbParticles);
		m_nbParticles = nbParticles;  
		return change;
	};

	virtual bool reroll() { return false; } // returns whether this actually changed anything
	virtual void applyTo(ParticlesSystem& particlesSystem) = 0;

protected:
	unsigned int m_nbParticles;
};