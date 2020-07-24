#include "ArrayStrigified.h"

void ArrayStrigified::push(const std::string& str) {
	if (m_empty) {
		m_str += str;
		m_empty = false;
	}
	else {
		m_str += " ," + str;
	}
}

void ArrayStrigified::end() {
	m_str += "}";
}