#pragma once

#include "Action.h"

#include "OpenGL/ShaderPipeline.h"
#include "OpenGL/SSBO.h"
#include "OpenGL/DrawOnTextureFramebuffer.h"

class ThumbnailFactory {
public:
	ThumbnailFactory();
	~ThumbnailFactory();

	unsigned int createTexture(ActionType actionType, const std::string& computeShaderCode);

public:
	static unsigned int genTexture();
	void drawOnTexture(unsigned int texID);
	void saveTexture(unsigned int texID, const std::string& filepath);
	void createAndApplyComputeShader(const std::string& shaderSrc);

private:
	ShaderPipeline m_renderPipeline;
	SSBO m_positionsSSBO;
	DrawOnTextureFramebuffer m_framebuffer;
	unsigned int m_vaoID;
	unsigned int m_vboID;

	std::string m_shapeTemplateSrc;
};