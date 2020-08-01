#pragma once

class SSBO {
public:
	SSBO(unsigned int binding, GLuint hint);
	~SSBO();

	void uploadData(int nbFloats, float* data);

private:
	unsigned int m_binding;
	GLuint m_hint;
	unsigned int m_id;
};