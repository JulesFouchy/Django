#include "GeneralSettings.h"

#include "Particles/ParticlesSystem.h"

#include <imgui/imgui.h>
#include <SDL2/SDL.h>

int GeneralSettings::NB_OF_PARTICLES;
bool GeneralSettings::ALPHA_TRAIL_ENABLED;
float GeneralSettings::ALPHA_TRAIL_DECAY;

void GeneralSettings::Initialize() {
	SetNbOfParticles(2000);
	ALPHA_TRAIL_ENABLED = true;
	EnableAlphaTrail();
	ALPHA_TRAIL_DECAY = 20.0f;
}

void GeneralSettings::ImGuiWindow() {
	ImGui::Begin("Rendering Settings");
	// Alpha trail
		// toggle
	if (ImGui::Checkbox("Alpha Trail enabled", &ALPHA_TRAIL_ENABLED)) {
		if (ALPHA_TRAIL_ENABLED)
			EnableAlphaTrail();
		else
			DisableAlphaTrail();
	}
		// strength
	ImGui::SliderFloat("Trail Decay", &ALPHA_TRAIL_DECAY, 0.0f, 60.0f);
	ImGui::End();
}

void GeneralSettings::SetNbOfParticles(int N) {
	// Set
	NB_OF_PARTICLES = N;
	// Update physics shader
	ParticlesSystem::PhysicsComputeShader().bind();
	ParticlesSystem::PhysicsComputeShader().setUniform1i("u_NbOfParticles", NB_OF_PARTICLES);
	ParticlesSystem::PhysicsComputeShader().unbind();
}

void GeneralSettings::EnableAlphaTrail() {
	SDL_GL_SetSwapInterval(0);
}

void GeneralSettings::DisableAlphaTrail() {
	SDL_GL_SetSwapInterval(1);
}