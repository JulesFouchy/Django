#include "AlphaTrailSettings.h"

AlphaTrailSettings::AlphaTrailSettings()
	: m_presets("djgTrail")
{
	m_clearScreenPipeline.addShader(ShaderType::Vertex, "res/shaders/clearScreen.vert");
	m_clearScreenPipeline.addShader(ShaderType::Fragment, "res/shaders/clearScreen.frag");
	m_clearScreenPipeline.createProgram();
}

void AlphaTrailSettings::apply() {

}

void AlphaTrailSettings::ImGui() {
	bool b = false;
	if (ImGui::Checkbox("Enabled", &m_values.bAlphaTrail))
		b = true;
	if (ImGui::SliderFloat("Trail Decay", &m_values.alphaTrailDecay, 0.0f, 60.0f))
		b = true;
	if (m_presets.ImGui(&m_values)) {
		apply();
	}
	if (b)
		m_presets.setToPlaceholderSetting();
}

void AlphaTrailSettings::clearScreen(float dt, const glm::vec3& bgColor) {
	if (m_values.bAlphaTrail) {
		//
		m_renderBuffer.bind();
		//
		m_clearScreenPipeline.bind();
		m_clearScreenPipeline.setUniform1f("dt", dt);
		m_clearScreenPipeline.setUniform1f("decay", m_values.alphaTrailDecay);
		m_clearScreenPipeline.setUniform3f("backgroundColor", bgColor.x, bgColor.y, bgColor.z);
		m_fullScreenVAO.bind();
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 6));
		glClear(GL_DEPTH_BUFFER_BIT);
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