#pragma once

class GeneralSettings {
public:
	static void Initialize();
	static void ImGuiWindow();

	static inline int NbOfParticles() { return NB_OF_PARTICLES; }
	static void SetNbOfParticles(int N);

	static void  EnableAlphaTrail();
	static void DisableAlphaTrail();
	static inline bool IsAlphaTrailEnabled() { return ALPHA_TRAIL_ENABLED; }
	static inline float AlphaTrailDecay() { return ALPHA_TRAIL_DECAY; }
	static inline float* BackgroundColor() { return BACKGROUND_COLOR; }

private:
	static int NB_OF_PARTICLES;
	static bool ALPHA_TRAIL_ENABLED;
	static float ALPHA_TRAIL_DECAY;
	static float BACKGROUND_COLOR[3];
};