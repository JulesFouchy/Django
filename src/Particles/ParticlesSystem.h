#include "OpenGL/ComputeShader.h"

#include "OpenGL/SSBO.h"

#include "Settings/PhysicsSettings.h"

#include <glm.hpp>
#include <vector>

class Configuration;

class ParticlesSystem {
public:
	ParticlesSystem(unsigned int nbParticles);
	~ParticlesSystem();

	//inline float ParticleRadiusInInches() { return PARTICLE_RADIUS_IN_INCHES; }
	inline float ParticleRadiusPropToHeight() { return PARTICLE_RADIUS_PROP_TO_HEIGHT; }

	void draw();
	void updatePositions();
	void ImGui_Windows(Configuration& currentConfiguration);

	void setNbParticles(unsigned int newNbParticles);
	inline unsigned int getNbParticles() { return m_nbParticles; }

	inline ShaderPipeline& physicsComputeShader() { return m_physicsShader.get(); }

private:
friend class Config_FillScreen;
friend class Config_Random;
friend class Config_Circle;
friend class App;
	inline size_t size() { return m_restPositions.size(); }
	inline glm::vec2& operator[](size_t index) { return m_restPositions[index]; }

	void sendRestPositionsToGPU();
	void recomputeVBO();

private:
	unsigned int m_nbParticles;
	//float PARTICLE_RADIUS_IN_INCHES;
	float PARTICLE_RADIUS_PROP_TO_HEIGHT = 0.0135f;

	std::vector<glm::vec2> m_restPositions;

	SSBO m_restPositionsSSBO;
	SSBO m_particlesSSBO;
	SSBO m_colorsSSBO;
	PhysicsSettings m_physicsSettings;

	unsigned int m_vaoID;
	unsigned int m_vboID;

	ComputeShader m_physicsShader;
};