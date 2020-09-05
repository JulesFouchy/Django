#pragma once

template <typename T, size_t N>
class CircularBuffer {
public:
	CircularBuffer() = default;
	~CircularBuffer() = default;

	void push(T element) {
		m_array[m_nextIndex] = element;
		m_nextIndex++;
		if (m_nextIndex == N) {
			m_nextIndex = 0;
			m_bIsFull = true;
		}
	}

	T computeAverage() { //!\ it won't quite work if T is an integer type
		size_t size = m_bIsFull ? N : m_nextIndex;
		T sum = 0;
		for (size_t i = 0; i < size; i++)
			sum += m_array[i];
		return sum / size;
	}

	void clear() {
		m_nextIndex = 0;
		m_bIsFull = false;
	}

private:
	std::array<T, N> m_array;
	size_t m_nextIndex = 0;
	bool m_bIsFull = false;
};