#pragma once

class ParticlesSystem;

class Configuration {
public:
	Configuration(ParticlesSystem& particlesSystem)
		: m_partSystem(particlesSystem)
	{
		generate();
	}
	virtual ~Configuration() = default;

	virtual void embody() = 0;

private:
	virtual void generate() {};

protected:
	ParticlesSystem& m_partSystem;
};