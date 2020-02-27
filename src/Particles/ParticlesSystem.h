#include "OpenGL/ShaderPipeline.h"

#include "OpenGL/SSBO.h"

#include "Settings/PhysicsSettings.h"

#include <glm.hpp>
#include <vector>

class ParticlesSystem {
public:
	ParticlesSystem(unsigned int nbParticles);
	static void Initialize();
	~ParticlesSystem();

	void draw();
	void updatePositions();
	void ImGui_Windows();

	void recomputeVBO();

	inline static ShaderPipeline& PhysicsComputeShader() { return m_physicsShader; }

private:
	friend class Config_FillScreen;
	friend class Config_Random;
	friend class App;
	inline size_t size() { return m_restPositions.size(); }
	inline glm::vec2& operator[](size_t index) { return m_restPositions[index]; }
	void sendRestPositionsToGPU();

private:
	std::vector<glm::vec2> m_restPositions;

	unsigned int m_nbParticles;

	SSBO m_restPositionsSSBO;
	SSBO m_particlesSSBO;
	SSBO m_colorsSSBO;
	PhysicsSettings m_physicsSettings;

	unsigned int m_vaoID;
	unsigned int m_vboID;

	static ShaderPipeline m_physicsShader;
};