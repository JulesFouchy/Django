#include "OpenGL/ComputeShader.h"
#include "OpenGL/SSBO.h"

struct ColorSettingsValues;

class ParticlesSystem {
public:
	ParticlesSystem();
	~ParticlesSystem();

	void draw();
	void updatePositions();

	void setNbParticles(unsigned int newNbParticles, const ColorSettingsValues& colorSettings);
	void setParticlesColors(const ColorSettingsValues& colorSettings);

	inline ShaderPipeline& physicsComputeShader() { return m_physicsShader.get(); }

	void recomputeVBO(float partRadiusRelToHeight);

private:
friend class Config_FillScreen;
friend class Config_RandomCPU;
friend class Config_Circle;
friend class App;
	inline size_t size() { return m_restPositions.size(); }
	inline glm::vec2& operator[](size_t index) { return m_restPositions[index]; }

	void sendRestPositionsToGPU();

private:
	unsigned int m_nbParticles;

	std::vector<glm::vec2> m_restPositions;

	SSBO m_restPositionsSSBO;
	SSBO m_particlesSSBO;
	SSBO m_colorsSSBO;

	unsigned int m_vaoID;
	unsigned int m_vboID;

	ComputeShader m_physicsShader;
};