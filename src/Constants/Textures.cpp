#include "Textures.h"

#include <stb_image/stb_image.h>
#include "Debugging/glException.h"

unsigned int Textures::m_playID      = -1;
unsigned int Textures::m_pauseID     = -1;
unsigned int Textures::m_stopID      = -1;
unsigned int Textures::m_recordID    = -1;

void Textures::_LoadAll() {
	m_playID      = LoadTexture("icons/play.png");
	m_pauseID     = LoadTexture("icons/pause.png");
	m_stopID      = LoadTexture("icons/stop.png");
	m_recordID    = LoadTexture("icons/record.png");
}

void Textures::_DestroyAll() {
	DestroyTexture(m_playID);
	DestroyTexture(m_pauseID);
	DestroyTexture(m_stopID);
	DestroyTexture(m_recordID);
}

unsigned int Textures::LoadTexture(const std::string& filepath) {
	// Load image
	stbi_set_flip_vertically_on_load(0);
	int width, height;
	unsigned char* data = stbi_load(filepath.c_str(), &width, &height, nullptr, 4);
	if (!data) {
		spdlog::warn("Couldn't open file '{}'", filepath);
		return -1;
	}
	// Create texture
	unsigned int texID;
	GLCall(glGenTextures(1, &texID));
	GLCall(glBindTexture(GL_TEXTURE_2D, texID));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	// Cleanup
	stbi_image_free(data);
	//
	return texID;
}

void Textures::DestroyTexture(unsigned int texID) {
	GLCall(glDeleteTextures(1, &texID));
}