#include "OpenGL/ShaderPipeline.h"

#include "OpenGL/SSBO.h"

#include "Settings/PhysicsSettings.h"

#include <glm.hpp>
#include <vector>

class ParticlesSystem {
public:
	static void Initialize();
	ParticlesSystem(unsigned int nbParticles);
	~ParticlesSystem();

	void draw();
	void updatePositions();
	void ImGui_Windows();

	void setNbParticles(unsigned int newNbParticles);
	inline unsigned int getNbParticles() { return m_nbParticles; }

	inline static ShaderPipeline& PhysicsComputeShader() { return s_physicsShader; }

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
	std::vector<glm::vec2> m_restPositions;

	SSBO m_restPositionsSSBO;
	SSBO m_particlesSSBO;
	SSBO m_colorsSSBO;
	PhysicsSettings m_physicsSettings;

	unsigned int m_vaoID;
	unsigned int m_vboID;

	static ShaderPipeline s_physicsShader;
};