#include "GeneralSettings.h"

#include "Particles/ParticlesSystem.h"

int GeneralSettings::NB_OF_PARTICLES;

void GeneralSettings::Initialize() {
	SetNbOfParticles(2000);
}

void GeneralSettings::SetNbOfParticles(int N) {
	// Set
	NB_OF_PARTICLES = N;
	// Update physics shader
	ParticlesSystem::PhysicsShader().bind();
	ParticlesSystem::PhysicsShader().setUniform1i("u_NbOfParticles", NB_OF_PARTICLES);
	ParticlesSystem::PhysicsShader().unbind();
}