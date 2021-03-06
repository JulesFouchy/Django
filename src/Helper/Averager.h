#pragma once

template<typename T>
class Averager {
public:
	Averager() {
		clear();
	}
	~Averager() = default;

	void push(T value) {
		N++;
		m_average = (m_average * (N-1) +  value) / N;
	}

	T get() {
		return m_average;
	}

	void clear() {
		m_average = 0;
		N = 0;
	}

private:
	T m_average;
	unsigned int N;
};