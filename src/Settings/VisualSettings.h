#pragma once

class ParticlesSystem;

class VisualSettings {
public:
	static void Initialize();
	static void ImGuiWindow(ParticlesSystem& particlesSystem);

	//static inline float ParticleRadiusInInches() { return PARTICLE_RADIUS_IN_INCHES; }
	static inline float ParticleRadiusPropToHeight() { return PARTICLE_RADIUS_PROP_TO_HEIGHT; }

	static void  EnableAlphaTrail();
	static void DisableAlphaTrail();
	static inline bool IsAlphaTrailEnabled() { return ALPHA_TRAIL_ENABLED; }
	static inline float AlphaTrailDecay() { return ALPHA_TRAIL_DECAY; }
	static inline float* BackgroundColor() { return BACKGROUND_COLOR; }

private:
	static int NB_OF_PARTICLES;
	//static float PARTICLE_RADIUS_IN_INCHES;
	static float PARTICLE_RADIUS_PROP_TO_HEIGHT;
	static bool ALPHA_TRAIL_ENABLED;
	static float ALPHA_TRAIL_DECAY;
	static float BACKGROUND_COLOR[3];
};