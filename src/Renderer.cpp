#include "Renderer.h"

#include "Settings/AlphaTrailSettings.h"
#include "Viewports/Viewports.h"

Renderer::Renderer(std::function<void()> renderTargetChangeCallback)
	: m_fullScreenVAOWithUVs(true), m_renderTargetChangeCallback(renderTargetChangeCallback)
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
	return m_targetRenderBuffer ? *m_targetRenderBuffer : m_renderAreaRenderBuffer;
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
		renderBuffer().bind();
		glClearColor(bgColor.x, bgColor.y, bgColor.z, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
}

void Renderer::onRenderEnd(const AlphaTrailSettingsValues& alphaTrail) {
	renderBuffer().blitToScreen(Viewports::SwapYConvention(Viewports::RenderArea.botLeft()), Viewports::SwapYConvention(Viewports::RenderArea.topRight()));
}

void Renderer::onRenderAreaResized(int width, int height) {
	m_renderAreaRenderBuffer.setSize(width, height);
	if (!hasRenderBufferAttached())
		m_textureFrameBuffer.setSize(width, height);
}

void Renderer::attachRenderbuffer(RenderBuffer& renderBuffer, const glm::vec3& bgColor) {
	assert(!hasRenderBufferAttached());
	m_targetRenderBuffer = &renderBuffer;
	m_textureFrameBuffer.setSize(renderBuffer.width(), renderBuffer.height());
	clearRenderBuffer(bgColor);
	Viewports::RenderArea.constrainRatio(aspectRatio());
	m_renderTargetChangeCallback();
}

void Renderer::detachRenderBuffer() {
	assert(hasRenderBufferAttached());
	m_targetRenderBuffer = nullptr;
	m_textureFrameBuffer.setSize(m_renderAreaRenderBuffer.width(), m_renderAreaRenderBuffer.height());
	applyRatioConstraints();
	// m_renderTargetChangeCallback(); // already called by applyRatioConstraints()
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
	return m_targetRenderBuffer ? m_targetRenderBuffer->aspectRatio() : m_renderAreaRenderBuffer.aspectRatio();
}

void Renderer::ImGui() {
	if (ImGui::Checkbox("Free ratio", &m_bFreeRatio)) {
		applyRatioConstraints();
	}
	if (!m_bFreeRatio) {
		if (ImGui::SliderFloat("Aspect Ratio", &m_desiredRatio, 0.5f, 2.0f)) {
			applyRatioConstraints();
		}
	}
}

void Renderer::applyRatioConstraints() {
	if (m_bFreeRatio)
		Viewports::RenderArea.unconstrainRatio();
	else
		Viewports::RenderArea.constrainRatio(m_desiredRatio);
	m_renderTargetChangeCallback();
}