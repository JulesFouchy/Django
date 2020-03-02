#pragma once

class ParticlesSystem;

class VisualSettings {
public:
	static void Initialize();
	static void ImGuiWindow();

	static void  EnableAlphaTrail();
	static void DisableAlphaTrail();
	static inline bool IsAlphaTrailEnabled() { return ALPHA_TRAIL_ENABLED; }
	static inline float AlphaTrailDecay() { return ALPHA_TRAIL_DECAY; }
	static inline float* BackgroundColor() { return BACKGROUND_COLOR; }

private:
	static bool ALPHA_TRAIL_ENABLED;
	static float ALPHA_TRAIL_DECAY;
	static float BACKGROUND_COLOR[3];
};