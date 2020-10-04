#include "Renderer.h"

#include "Settings/AlphaTrailSettings.h"
#include "Viewports/Viewports.h"

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

void Renderer::onRenderBegin(float dt, const glm::vec3& bgColor, const AlphaTrailSettingsValues& alphaTrail) {
	if (!alphaTrail.bEnabled) {
		m_renderBuffer.bind();
		glClearColor(bgColor.x, bgColor.y, bgColor.z, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	else {
		if (!alphaTrail.bFixResiduals) {
			m_renderBuffer.bind();
			m_clearScreenPipeline.bind();
			m_clearScreenPipeline.setUniform1f("alpha", dt * alphaTrail.decay);
			m_clearScreenPipeline.setUniform3f("backgroundColor", bgColor);
			drawFullScreen();
			glClear(GL_DEPTH_BUFFER_BIT);
		}
		else {
			m_renderBuffer.blitTo(m_prevFrameTexture);
			m_renderBuffer.bind();
			m_clearScreenNoResidualsPipeline.bind();
			m_clearScreenNoResidualsPipeline.setUniform1f("alpha", dt * alphaTrail.decay);
			m_clearScreenNoResidualsPipeline.setUniform3f("backgroundColor", bgColor);
			m_clearScreenNoResidualsPipeline.setUniform1f("threshold", alphaTrail.threshold);
			m_clearScreenNoResidualsPipeline.setUniform1f("minAlpha", alphaTrail.minAlpha);
			m_prevFrameTexture.attachTextureToSlot(1);
			m_clearScreenNoResidualsPipeline.setUniform1i("prevFrame", 1);
			drawFullScreenWithUVs();
			glClear(GL_DEPTH_BUFFER_BIT);
		}
	}
}

void Renderer::onRenderSizeChanged(const glm::vec3& bgColor) {
	const glm::ivec2& size = Viewports::RenderSize().size();
	m_renderBuffer    .setSize(size.x, size.y);
	m_prevFrameTexture.setSize(size.x, size.y);
	clearRenderBuffer(bgColor);
}

void Renderer::clearRenderBuffer(const glm::vec3& clearColor) {
	m_renderBuffer.bind();
	glClearColor(clearColor.r, clearColor.g, clearColor.b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	m_renderBuffer.unbind();
}

void Renderer::drawFullScreen() {
	m_fullScreenVAO.bind();
	GLCall(glDrawArrays(GL_TRIANGLES, 0, 6));
}

void Renderer::drawFullScreenWithUVs() {
	m_fullScreenVAOWithUVs.bind();
	GLCall(glDrawArrays(GL_TRIANGLES, 0, 6));
}