#include "OpenGL/ComputeShader.h"
#include "OpenGL/SSBO.h"

struct ColorSettingsValues;
class StateModifier;

class ParticleSystem {
public:
	ParticleSystem();
	~ParticleSystem();

	void draw();
	void updatePositions();

	inline unsigned int getNbParticles() { return m_nbParticles; }
	void applyNbParticles(unsigned int newNbParticles, const ColorSettingsValues& colorSettings);
	void applyParticleColors(const ColorSettingsValues& colorSettings);
	void applyAndRecord_SetAllRestPositions(const glm::vec2& position, StateModifier& stateModifier);

	inline ShaderPipeline& physicsComputeShader() { return m_physicsShader.get(); }

	void recomputeVBO(float partRadiusRelToHeight);

private:
	inline size_t size() { return m_restPositions.size(); }
	inline glm::vec2& operator[](size_t index) { return m_restPositions[index]; }

	void sendRestPositionsToGPU();

private:
	unsigned int m_nbParticles;

	std::vector<glm::vec2> m_restPositions;

	SSBO<float> m_restPositionsSSBO;
	SSBO<float> m_particlesSSBO;
	SSBO<float> m_colorsSSBO;

	unsigned int m_vaoID;
	unsigned int m_vboID;

	ComputeShader m_physicsShader;
	ComputeShader m_colorGradientComputeShader;
	ComputeShader m_hueGradientComputeShader;
	ComputeShader m_setAllRestPositionsComputeShader;
};