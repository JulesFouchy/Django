#include "MouseInteractions.h"

#include "StateModifier.h"
#include "Recording/StateChange.h"
#include "OpenGL/ShaderPipeline.h"
#include "Particles/ParticlesSystem.h"
#include "Helper/Input.h"

void MouseInteractions::update(StateModifier& stateModifier) {
	// Force field
	if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && !ImGui::GetIO().WantCaptureMouse)
		setApplyAndRecord_ForceField(Input::GetMouseInNormalizedRatioSpace(), stateModifier);
	// Burst
	if (ImGui::IsMouseReleased(ImGuiMouseButton_Right)) {
		float strength = pow(25.0f * ImGui::GetIO().MouseDownDurationPrev[ImGuiMouseButton_Right], 0.8f);
		setApplyAndRecord_Burst({ Input::GetMouseInNormalizedRatioSpace(), strength }, stateModifier);
	}
	// Apply
	ShaderPipeline& physicsCompute = stateModifier.particleSystem().physicsComputeShader();
	if (!m_forceFieldQueue.empty()) {
		physicsCompute.setUniform1i("u_bForceField", true);
		physicsCompute.setUniform2f("u_mouse", m_forceFieldQueue.front());
		m_forceFieldQueue.pop();
	}
	else {
		physicsCompute.setUniform1i("u_bForceField", false);
	}
	if (!m_burstQueue.empty()) {
		physicsCompute.setUniform1f("u_burstStrength", m_burstQueue.front().z);
		physicsCompute.setUniform2f("u_mouse", { m_burstQueue.front().x, m_burstQueue.front().y });
		m_burstQueue.pop();
	}
	else {
		physicsCompute.setUniform1f("u_burstStrength", -1.0f);
	}
}

void MouseInteractions::setApplyAndRecord_ForceField(const glm::vec2& mousePos, StateModifier& stateModifier) {
	m_forceFieldQueue.push(mousePos);
	stateModifier.recordChange({ StateChangeType::Mouse_ForceField, mousePos });
}

void MouseInteractions::setApplyAndRecord_Burst(const glm::vec3& mousePosAndStrength, StateModifier& stateModifier) {
	m_burstQueue.push(mousePosAndStrength);
	stateModifier.recordChange({ StateChangeType::Mouse_Burst, mousePosAndStrength });
}