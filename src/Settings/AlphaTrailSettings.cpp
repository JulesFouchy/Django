#include "AlphaTrailSettings.h"

#include "Helper/MyImGui.h"

AlphaTrailSettings::AlphaTrailSettings()
	: m_presets("djgTrail"), m_fullScreenVAOWithUVs(true)
{
	m_clearScreenPipeline.addShader(ShaderType::Vertex, "res/shaders/passVertPos.vert");
	m_clearScreenPipeline.addShader(ShaderType::Fragment, "res/shaders/clearScreen.frag");
	m_clearScreenPipeline.createProgram();
	//
	m_clearScreenNoResidualsPipeline.addShader(ShaderType::Vertex, "res/shaders/passVertPosAndUVs.vert");
	m_clearScreenNoResidualsPipeline.addShader(ShaderType::Fragment, "res/shaders/clearScreenNoResiduals.frag");
	m_clearScreenNoResidualsPipeline.createProgram();
}

void AlphaTrailSettings::apply(const glm::vec3& bgColor) {
	if (m_values.bAlphaTrail) {
		if (!m_values.bFixResiduals)
			glEnable(GL_BLEND);
		else
			glDisable(GL_BLEND);
	}
	else {
		glDisable(GL_BLEND);
	}
}

void AlphaTrailSettings::ImGui(const glm::vec3& bgColor) {
	bool b = false;
	if (ImGui::Checkbox("Enabled", &m_values.bAlphaTrail)) {
		b = true;
		if (m_values.bAlphaTrail)
			clearRenderBuffer(bgColor);
		apply(bgColor);
	}
	if (m_values.bAlphaTrail) {
		if (ImGui::SliderFloat("Trail Decay", &m_values.alphaTrailDecay, 0.0f, 30.0f))
			b = true;
		if (ImGui::Checkbox("Fix Residuals", &m_values.bFixResiduals)) {
			b = true;
			apply(bgColor);
		}
		ImGui::SameLine();
		MyImGui::HelpMarker("For very small values of Trail Decay, some artifacts appear.\nCheck this only if you see them, since the fix comes at a small performance cost.");
		if (m_values.bFixResiduals) {
			if (ImGui::SliderFloat("Threshold", &m_values.threshold, 0.0f, 0.5f))
				b = true;
			if (ImGui::SliderFloat("Min Alpha", &m_values.minAlpha, 0.0f, 0.2f))
				b = true;
		}
	}
	bool prevBTrail = m_values.bAlphaTrail;
	if (m_presets.ImGui(&m_values)) {
		if (m_values.bAlphaTrail && !prevBTrail)
			clearRenderBuffer(bgColor);
		apply(bgColor);
	}
	if (b)
		m_presets.setToPlaceholderSetting();
}

void AlphaTrailSettings::clearScreen(float dt, const glm::vec3& bgColor) {
	if (m_values.bAlphaTrail) {
		if (!m_values.bFixResiduals) {
			m_renderBuffer.bind();
			m_clearScreenPipeline.bind();
			m_clearScreenPipeline.setUniform1f("alpha", dt * m_values.alphaTrailDecay);
			m_clearScreenPipeline.setUniform3f("backgroundColor", bgColor.x, bgColor.y, bgColor.z);
			m_fullScreenVAO.bind();
			GLCall(glDrawArrays(GL_TRIANGLES, 0, 6));
			glClear(GL_DEPTH_BUFFER_BIT);
		}
		else {
			m_renderBuffer.blitTo(m_textureFrameBuffer);
			m_renderBuffer.bind();
			m_clearScreenNoResidualsPipeline.bind();
			m_clearScreenNoResidualsPipeline.setUniform1f("alpha", dt * m_values.alphaTrailDecay);
			m_clearScreenNoResidualsPipeline.setUniform3f("backgroundColor", bgColor.x, bgColor.y, bgColor.z);
			m_clearScreenNoResidualsPipeline.setUniform1f("threshold", m_values.threshold);
			m_clearScreenNoResidualsPipeline.setUniform1f("minAlpha", m_values.minAlpha);
			m_textureFrameBuffer.attachTextureToSlot(1);
			m_clearScreenNoResidualsPipeline.setUniform1i("prevFrame", 1);
			m_fullScreenVAOWithUVs.bind();
			GLCall(glDrawArrays(GL_TRIANGLES, 0, 6));
			glClear(GL_DEPTH_BUFFER_BIT);
		}
	}
	else {
		glClearColor(bgColor.x, bgColor.y, bgColor.z, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
}

void AlphaTrailSettings::finishRendering() {
	if (m_values.bAlphaTrail) {
		m_renderBuffer.unbind();
		m_renderBuffer.blitToScreen();
	}
}

void AlphaTrailSettings::clearRenderBuffer(const glm::vec3& bgColor) {
	m_renderBuffer.bind();
	glClearColor(bgColor.x, bgColor.y, bgColor.z, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	m_renderBuffer.unbind();
}