#pragma once

class ParticlesSystem;
struct ConfigParams;
struct RandomParams;

class Configuration {
public:
	Configuration() = default;
	virtual ~Configuration() = default;

	virtual void applyTo(ParticlesSystem& particlesSystem, const ConfigParams& params, const RandomParams& randParams) = 0;
};