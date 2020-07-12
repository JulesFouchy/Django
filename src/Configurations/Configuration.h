#pragma once

class ParticlesSystem;
struct ConfigParams;

class Configuration {
public:
	Configuration() = default;
	virtual ~Configuration() = default;

	virtual void applyTo(ParticlesSystem& particlesSystem, const ConfigParams& params) = 0;
};