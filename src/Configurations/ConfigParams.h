#pragma once

class StateModifier;

struct ConfigParams {
public:
	bool onKeyPressed(SDL_Scancode scancode, StateModifier& stateModifier);

	void setApplyAndRecord_Wheel     (float value, StateModifier& stateModifier);
	void setApplyAndRecord_CtrlWheel (float value, StateModifier& stateModifier);
	void setApplyAndRecord_ShiftWheel(float value, StateModifier& stateModifier);
	void setApplyAndRecord_AltWheel  (float value, StateModifier& stateModifier);

	inline int   getCount()      const { return count; }
	inline int   getIntLR()      const { return intLR; }
	inline int   getIntUD()      const { return intUD; }
	inline float getWheel()      const { return wheel; }
	inline float getCtrlWheel()  const { return ctrlWheel; }
	inline float getShiftWheel() const { return shiftWheel; }
	inline float getAltWheel()   const { return altWheel; }

private:
	int count = 5;
	int intLR = 6;
	int intUD = 1;
	float wheel = 1.0f;
	float ctrlWheel = 5.0f;
	float shiftWheel = 1.0f;
	float altWheel = 1.0f;
};