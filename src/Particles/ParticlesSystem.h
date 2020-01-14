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
	std::vector<glm::vec2> m_restPositions;

	unsigned int m_nbParticles;

	unsigned int m_restPosSSBOid;
	unsigned int m_actualPosSSBOid;

	unsigned int m_vaoID;
	unsigned int m_vboID;
};