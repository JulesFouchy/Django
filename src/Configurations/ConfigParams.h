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
	int intLR = 0;
	int intUD = 0;
	float wheel = 0.0f;
	float ctrlWheel = 0.0f;
	float shiftWheel = 0.0f;
	float altWheel = 0.0f;

private:
	//Serialization
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& archive)
	{
		archive(
			CEREAL_NVP(count),
			CEREAL_NVP(intLR),
			CEREAL_NVP(intUD),
			CEREAL_NVP(wheel),
			CEREAL_NVP(ctrlWheel),
			CEREAL_NVP(shiftWheel),
			CEREAL_NVP(altWheel)
		);
	}
};