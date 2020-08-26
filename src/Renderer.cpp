#include "Renderer.h"

#include "Settings/AlphaTrailSettings.h"
#include "Helper/DisplayInfos.h"

Renderer::Renderer()
	: m_fullScreenVAOWithUVs(true)
{
	m_clearScreenPipeline.addShader(ShaderType::Vertex,   "internal-shaders/passVertPos.vert");
	m_clearScreenPipeline.addShader(ShaderType::Fragment, "internal-shaders/clearScreen.frag");
	m_clearScreenPipeline.createProgram();
	//
	m_clearScreenNoResidualsPipeline.addShader(ShaderType::Vertex,   "internal-shaders/passVertPosAndUVs.vert");
	m_clearScreenNoResidualsPipeline.addShader(ShaderType::Fragment, "internal-shaders/clearScreenNoResiduals.frag");
	m_clearScreenNoResidualsPipeline.createProgram();
}

RenderBuffer& Renderer::renderBuffer() {
	return m_targetRenderBuffer ? *m_targetRenderBuffer : m_screenSizeRenderBuffer;
}

void Renderer::onRenderBegin(float dt, const glm::vec3& bgColor, const AlphaTrailSettingsValues& alphaTrail) {
	if (alphaTrail.bEnabled) {
		if (!alphaTrail.bFixResiduals) {
			renderBuffer().bind();
			m_clearScreenPipeline.bind();
			m_clearScreenPipeline.setUniform1f("alpha", dt * alphaTrail.decay);
			m_clearScreenPipeline.setUniform3f("backgroundColor", bgColor);
			drawFullScreen();
			glClear(GL_DEPTH_BUFFER_BIT);
		}
		else {
			renderBuffer().blitTo(m_textureFrameBuffer);
			renderBuffer().bind();
			m_clearScreenNoResidualsPipeline.bind();
			m_clearScreenNoResidualsPipeline.setUniform1f("alpha", dt * alphaTrail.decay);
			m_clearScreenNoResidualsPipeline.setUniform3f("backgroundColor", bgColor);
			m_clearScreenNoResidualsPipeline.setUniform1f("threshold", alphaTrail.threshold);
			m_clearScreenNoResidualsPipeline.setUniform1f("minAlpha", alphaTrail.minAlpha);
			m_textureFrameBuffer.attachTextureToSlot(1);
			m_clearScreenNoResidualsPipeline.setUniform1i("prevFrame", 1);
			drawFullScreenWithUVs();
			glClear(GL_DEPTH_BUFFER_BIT);
		}
	}
	else {
		if (m_targetRenderBuffer)
			m_targetRenderBuffer->bind();
		glClearColor(bgColor.x, bgColor.y, bgColor.z, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
}

void Renderer::onRenderEnd(const AlphaTrailSettingsValues& alphaTrail) {
	if (alphaTrail.bEnabled || m_targetRenderBuffer) {
		renderBuffer().blitToScreen();
	}
}

void Renderer::onWindowResize(unsigned int width, unsigned int height) {
	m_screenSizeRenderBuffer.setSize(width, height);
	if (!hasRenderBufferAttached())
		m_textureFrameBuffer.setSize(width, height);
	DisplayInfos::SetRenderTargetAspectRatio(aspectRatio());
}

void Renderer::attachRenderbuffer(RenderBuffer& renderBuffer, const glm::vec3& bgColor) {
	assert(!hasRenderBufferAttached());
	m_targetRenderBuffer = &renderBuffer;
	m_textureFrameBuffer.setSize(renderBuffer.width(), renderBuffer.height());
	clearRenderBuffer(bgColor);
}

void Renderer::detachRenderBuffer() {
	assert(hasRenderBufferAttached());
	m_targetRenderBuffer = nullptr;
	m_textureFrameBuffer.setSize(m_screenSizeRenderBuffer.width(), m_screenSizeRenderBuffer.height());
}

void Renderer::clearRenderBuffer(const glm::vec3& clearColor) {
	renderBuffer().bind();
	glClearColor(clearColor.r, clearColor.g, clearColor.b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	renderBuffer().unbind();
}

void Renderer::drawFullScreen() {
	m_fullScreenVAO.bind();
	GLCall(glDrawArrays(GL_TRIANGLES, 0, 6));
}

void Renderer::drawFullScreenWithUVs() {
	m_fullScreenVAOWithUVs.bind();
	GLCall(glDrawArrays(GL_TRIANGLES, 0, 6));
}

float Renderer::aspectRatio() const {
	return m_targetRenderBuffer ? m_targetRenderBuffer->aspectRatio() : m_screenSizeRenderBuffer.aspectRatio();
}