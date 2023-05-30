#pragma once

#include <stdio.h>
#include <math.h>
#include <algorithm>

template<class T, size_t current_size, size_t target_size>
class Interpolator
{
public:
	Interpolator() {};

	Interpolator(T(&source_data)[current_size])
	{
		SetData(source_data);
	}

	void SetData(T(&source_data)[current_size])
	{
		memcpy(m_sourceData, source_data, current_size * sizeof(T));
	}

	T lerp(T a, T b, float p)
	{
		return static_cast<T>(a + p * (b - a));
	}

	void Interpolate()
	{
		for (size_t tIdx = 0; tIdx < target_size; tIdx++)
		{
			float rawIndex = m_stepSize * tIdx;
			size_t leftSrcIndex = static_cast<size_t>(rawIndex);
			size_t rightSrcIndex = static_cast<size_t>(std::min(leftSrcIndex + 1, current_size - 1));
			float idxDiff = rawIndex - static_cast<float>(leftSrcIndex);
			m_finalData[tIdx] = lerp(m_sourceData[leftSrcIndex], m_sourceData[rightSrcIndex], idxDiff);
		}
	}

	T& operator[](std::size_t idx) { return m_finalData[idx]; }

	T* begin() { return m_finalData; }
	T* end() { return &(m_finalData[target_size]); }

private:
	float m_stepSize = static_cast<float>(current_size - 1) / static_cast<float>(target_size - 1);
	T m_sourceData[current_size];
	T m_finalData[target_size];
};
