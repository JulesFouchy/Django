#pragma once

class LiveMode {
public:
	[[nodiscard]] static inline bool IsOff() { return !s_on; }
	[[nodiscard]] static inline bool ShowHelpMarkers() { return !s_hideHelpMarkers; }
	static void ImGui();

private:
	static bool s_on;
	static bool s_hideHelpMarkers;
};