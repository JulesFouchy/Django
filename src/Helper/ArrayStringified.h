#pragma once

class ArrayStringified {
public:
	ArrayStringified() = default;
	~ArrayStringified() = default;

	void push(const std::string& str);
	void close();
	inline const std::string& string() const { return m_str; }

private:
	std::string m_str = "{";
	bool m_empty = true;
};