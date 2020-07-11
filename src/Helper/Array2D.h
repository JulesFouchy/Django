#pragma once

template <typename T>
class Array2D {
public:
	Array2D() = default;
	~Array2D() = default;

	void setSize(size_t X, size_t Y) {
		m_width  = X;
		m_height = Y;
		m_vector.resize(X * Y);
	}

	inline size_t getWidth()  { return m_width; }
	inline size_t getHeight() { return m_height; }

	inline T& operator()(size_t x, size_t y) {
		return m_vector[x + y * m_width];
	}
private:
	std::vector<T> m_vector;
	size_t m_width;
	size_t m_height;
};