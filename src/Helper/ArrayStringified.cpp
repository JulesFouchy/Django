#include "ArrayStringified.h"

void ArrayStringified::push(const std::string& str) {
	if (m_empty) {
		m_str += str;
		m_empty = false;
	}
	else {
		m_str += ", " + str;
	}
}

void ArrayStringified::close() {
	m_str += "}";
}