#pragma once

#include <glad/glad.h>

class SSBO {
public:
	SSBO(unsigned int binding, int componentsCount, GLuint hint); // componentsCount = 2 for vec2 etc.
	~SSBO();

	void uploadData(int nbElements, float* data);

private:
	unsigned int m_binding;
	int m_componentsCount;
	GLuint m_hint;
	unsigned int m_id;
};