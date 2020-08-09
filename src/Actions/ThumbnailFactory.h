#pragma once

#include "Action.h"

#include "OpenGL/ShaderPipeline.h"
#include "OpenGL/SSBO.h"
#include "OpenGL/DrawOnTextureFramebuffer.h"
#include "OpenGL/ComputeShader.h"

class ThumbnailFactory {
public:
	ThumbnailFactory();
	~ThumbnailFactory();

	unsigned int createTexture(ActionType actionType, const std::string& computeShaderCode);
	unsigned int createTextureForSVG(unsigned int nbCurves, unsigned int offsetInSsbo);

private:
	static unsigned int genTexture();
	void drawOnTexture(unsigned int texID);
	void saveTexture(unsigned int texID, const std::string& filepath);
	void createAndApplyComputeShader(const std::string& shaderSrc);

private:
	ShaderPipeline m_renderPipeline;
	SSBO m_positionsSSBO;
	ComputeShader m_svgComputeShader;
	DrawOnTextureFramebuffer m_framebuffer;
	unsigned int m_vaoID;
	unsigned int m_vboID;

	std::string m_shapeTemplateSrc;
	std::string m_layoutBeforeTemplateSrc;
	std::string m_layoutAfterTemplateSrc;
	std::string m_standaloneTemplateSrc;
};