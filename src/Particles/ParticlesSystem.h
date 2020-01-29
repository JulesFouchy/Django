#include <glm.hpp>

#include <vector>

class ParticlesSystem {
public:
	ParticlesSystem(unsigned int nbParticles);
	~ParticlesSystem();

	void draw();
	void updatePositions();

	void recomputeVBO();

private:
	friend class Config_FillScreen;
	friend class App;
	inline size_t size() { return m_restPositions.size(); }
	inline glm::vec2& operator[](size_t index) { return m_restPositions[index]; }
	void sendRestPositionsToGPU();

private:
	std::vector<glm::vec2> m_restPositions;

	unsigned int m_nbParticles;

	unsigned int m_restPosSSBOid;
	unsigned int m_actualPosSSBOid;

	unsigned int m_vaoID;
	unsigned int m_vboID;
};