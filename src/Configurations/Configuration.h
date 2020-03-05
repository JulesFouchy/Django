#pragma once

#include <string>

#include <SDL2/SDL_keycode.h>

class ParticlesSystem;

class Configuration {
public:
	Configuration(const std::string& name)
		: m_nbParticles(-1), m_name(name)
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
	virtual void ImGuiParameters(ParticlesSystem& particlesSystem) {}
	virtual void onKeyPressed(SDL_Scancode scancode, ParticlesSystem& partSystem) {}

	inline std::string getName() { return m_name + " Configuration"; }

protected:
	unsigned int m_nbParticles;
	std::string m_name;
};