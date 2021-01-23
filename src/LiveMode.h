#pragma once

class LiveMode {
public:
	[[nodiscard]] static constexpr inline bool IsOff() { return !s_on; }
	static void ImGui();

private:
	static bool s_on;
};