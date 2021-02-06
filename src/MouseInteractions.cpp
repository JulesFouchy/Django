#include "MouseInteractions.h"

#include "StateModifier.h"
#include "Recording/StateChange.h"
#include "OpenGL/ShaderPipeline.h"
#include "Particles/ParticlesSystem.h"
#include <Cool/App/Input.h>

using namespace Cool;

void MouseInteractions::update(StateModifier& stateModifier, bool bIsExporting) {
	// Force field
	if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && !ImGui::GetIO().WantCaptureMouse && !bIsExporting)
		setForceField(Input::MouseInNormalizedRatioSpace());
	// Burst
	if (ImGui::IsMouseReleased(ImGuiMouseButton_Right) && !bIsExporting) {
		float strength = pow(25.0f * ImGui::GetIO().MouseDownDurationPrev[ImGuiMouseButton_Right], 0.8f);
		setBurst({ Input::MouseInNormalizedRatioSpace(), strength });
	}
	// Apply
	ShaderPipeline& physicsCompute = stateModifier.particleSystem().physicsComputeShader();
	if (m_bForceField) {
		physicsCompute.setUniform1i("u_bForceField", true);
		physicsCompute.setUniform2f("u_mouse", m_forceFieldPos);
		stateModifier.recordChange({ StateChangeType::Mouse_ForceField, m_forceFieldPos }); // record only the position we selected for this frame (current mouse if any, recorded mouse otherwise if any)
		m_bForceField = false; // reset for next frame
	}
	else {
		physicsCompute.setUniform1i("u_bForceField", false);
	}
	if (!m_burstQueue.empty()) {
		physicsCompute.setUniform1f("u_burstStrength", m_burstQueue.front().z);
		physicsCompute.setUniform2f("u_mouse", { m_burstQueue.front().x, m_burstQueue.front().y });
		stateModifier.recordChange({ StateChangeType::Mouse_Burst, m_burstQueue.front() });
		m_burstQueue.pop();
	}
	else {
		physicsCompute.setUniform1f("u_burstStrength", -1.0f);
	}
}

void MouseInteractions::setForceField(const glm::vec2& mousePos) {
	m_bForceField = true;
	m_forceFieldPos = mousePos;
}

void MouseInteractions::setBurst(const glm::vec3& mousePosAndStrength) {
	m_burstQueue.push(mousePosAndStrength);
}