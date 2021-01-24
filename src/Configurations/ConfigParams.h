#pragma once

class StateModifier;

struct ConfigParams {
public:
	bool onKeyPressed(SDL_Scancode scancode, StateModifier& stateModifier);
	void onWheel(int delta, bool bNoStandardScroll, StateModifier& stateModifier);

	void setApplyAndRecord_Wheel     (int value, StateModifier& stateModifier);
	void setApplyAndRecord_CtrlWheel (int value, StateModifier& stateModifier);
	void setApplyAndRecord_ShiftWheel(int value, StateModifier& stateModifier);
	void setApplyAndRecord_AltWheel  (int value, StateModifier& stateModifier);

	inline int getLR()         const { return LR; }
	inline int getUD()         const { return UD; }
	inline int getWheel()      const { return wheel; }
	inline int getCtrlWheel()  const { return ctrlWheel; }
	inline int getShiftWheel() const { return shiftWheel; }
	inline int getAltWheel()   const { return altWheel; }

private:
	int LR         = 0;
	int UD         = 0;
	int wheel      = 0;
	int ctrlWheel  = 0;
	int shiftWheel = 0;
	int altWheel   = 0;

private:
	//Serialization
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& archive)
	{
		archive(
			CEREAL_NVP(LR),
			CEREAL_NVP(UD),
			CEREAL_NVP(wheel),
			CEREAL_NVP(ctrlWheel),
			CEREAL_NVP(shiftWheel),
			CEREAL_NVP(altWheel)
		);
	}
};