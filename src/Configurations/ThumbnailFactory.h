#pragma once

#include "ConfigType.h"

#include "OpenGL/ShaderPipeline.h"
#include "OpenGL/SSBO.h"
#include "OpenGL/DrawOnTextureFramebuffer.h"

class ComputeShader;

class ThumbnailFactory {
public:
	ThumbnailFactory();
	~ThumbnailFactory();

	unsigned int createTexture(ConfigType configType, const std::string& computeShaderCode);

public:
	static unsigned int genTexture();
	void drawOnTexture(unsigned int texID);
	void saveTexture(unsigned int texID, const std::string& filepath);

private:
	ShaderPipeline m_renderPipeline;
	SSBO m_positionsSSBO;
	DrawOnTextureFramebuffer m_framebuffer;
	unsigned int m_vaoID;
	unsigned int m_vboID;
};