#pragma once

class ArrayStrigified {
public:
	ArrayStrigified() = default;
	~ArrayStrigified() = default;

	void push(const std::string& str);
	void end();
	inline const std::string& string() const { return m_str; }

private:
	std::string m_str = "{";
	bool m_empty = true;
};