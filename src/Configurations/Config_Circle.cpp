#include "Config_Circle.h"

#include "Particles/ParticlesSystem.h"

#include "Debugging/Log.h"

Config_Circle::Config_Circle() {
}

void Config_Circle::embody(ParticlesSystem& partSystem) {
	for (int i = 0; i < partSystem.size(); ++i) {
		float angle = 2.0f * 3.141f * i / (float)partSystem.size();
		float radius = 0.8f;
		partSystem[i].x = radius * cos(angle);
		partSystem[i].y = radius * sin(angle);
	}
}