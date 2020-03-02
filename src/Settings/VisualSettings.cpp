#include "VisualSettings.h"

#include "Particles/ParticlesSystem.h"

#include <imgui/imgui.h>
#include <SDL2/SDL.h>

//float VisualSettings::PARTICLE_RADIUS_IN_INCHES = 0.87f;
float VisualSettings::PARTICLE_RADIUS_PROP_TO_HEIGHT = 0.0135f;
bool VisualSettings::ALPHA_TRAIL_ENABLED;
float VisualSettings::ALPHA_TRAIL_DECAY = 20.0f;
float VisualSettings::BACKGROUND_COLOR[3] = { 0.0f, 0.0f, 0.0f };

void VisualSettings::Initialize() {
	ALPHA_TRAIL_ENABLED = true;
	EnableAlphaTrail();
}

void VisualSettings::ImGuiWindow(ParticlesSystem& particlesSystem) {
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
	// Background Color
	ImGui::ColorEdit3("Background Color", BACKGROUND_COLOR);
	// Particles Radius
	if (ImGui::SliderFloat("Particles Radius", &PARTICLE_RADIUS_PROP_TO_HEIGHT, 0.0f, 0.1f))
		particlesSystem.recomputeVBO(PARTICLE_RADIUS_PROP_TO_HEIGHT);
	//
	ImGui::End();
}

void VisualSettings::EnableAlphaTrail() {
	SDL_GL_SetSwapInterval(0);
}

void VisualSettings::DisableAlphaTrail() {
	SDL_GL_SetSwapInterval(1);
}