#pragma once

#include <stdio.h>
#include <math.h>
#include <algorithm>
#include <tuple>

template<class T, size_t current_size, size_t target_size>
class Interpolator
{
public:
	Interpolator()
	{
		memset(m_sourceData, 0, sizeof(T) * current_size);
		memset(m_finalData, 0, sizeof(T) * target_size);
	};

	Interpolator(T(&source_data)[current_size])
	{
		SetData(source_data);
		memset(m_finalData, 0, sizeof(T) * target_size);
	}

	Interpolator(const T(&source_data)[current_size])
	{
		SetData(source_data);
		memset(m_finalData, 0, sizeof(T) * target_size);
	}

	void SetData(T(&source_data)[current_size])
	{
		memcpy(m_sourceData, source_data, current_size * sizeof(T));
		m_interpolated = false;
	}

	void SetData(const T(&source_data)[current_size])
	{
		memcpy(m_sourceData, source_data, current_size * sizeof(T));
		m_interpolated = false;
	}

	void Interpolate()
	{
		for (size_t tIdx = 0; tIdx < target_size; tIdx++)
			m_finalData[tIdx] = calculateValue(tIdx);
		m_interpolated = true;
	}

	T& operator[](size_t idx) 
	{ 
		if (!m_interpolated)
			m_finalData[idx] = calculateValue(idx);
		return m_finalData[idx];
	}

	T* begin() 
	{ 
		if (!m_interpolated)
			Interpolate();
		return m_finalData; 
	}

	T* end() 
	{ 
		if (!m_interpolated)
			Interpolate();
		return &(m_finalData[target_size]); 
	}

private:
	T lerp(T a, T b, float p)
	{
		return static_cast<T>(a + p * (b - a));
	}

	T calculateValue(size_t index)
	{
		float rawIndex = m_stepSize * index;
		size_t leftSrcIndex = static_cast<size_t>(rawIndex);
		size_t rightSrcIndex = static_cast<size_t>(std::min(leftSrcIndex + 1, current_size - 1));
		float idxDiff = rawIndex - static_cast<float>(leftSrcIndex);
		return lerp(m_sourceData[leftSrcIndex], m_sourceData[rightSrcIndex], idxDiff);
	}

private:
	bool m_interpolated = false;
	float m_stepSize = static_cast<float>(current_size - 1) / static_cast<float>(target_size - 1);
	T m_sourceData[current_size];
	T m_finalData[target_size];
};


template<class IT, class VT, size_t size>
class PointInterpolator
{
public:
	PointInterpolator()
	{
		memset(m_sourceData, 0, sizeof(std::tuple<IT, VT>) * size);
	};

	PointInterpolator(std::tuple<IT, VT>(&source_data)[size])
	{
		Set(source_data);
	}

	PointInterpolator(const std::tuple<IT, VT>(&source_data)[size])
	{
		Set(source_data);
	}

	void Set(std::tuple<IT, VT>(&source_data)[size])
	{
		memcpy(m_sourceData, source_data, sizeof(std::tuple<IT, VT>) * size);
		std::sort(m_sourceData, m_sourceData + size, customLess);
	}

	void Set(const std::tuple<IT, VT>(&source_data)[size])
	{
		memcpy(m_sourceData, source_data, sizeof(std::tuple<IT, VT>) * size);
		std::sort(m_sourceData, m_sourceData + size, customLess);
	}

	VT Get(IT idx)
	{
		if (idx <= std::get<0>(m_sourceData[0]))
			return std::get<1>(m_sourceData[0]);
		else if (idx >= std::get<0>(m_sourceData[size - 1]))
			return std::get<1>(m_sourceData[size - 1]);

		size_t targetIndex;
		for (size_t i = 0; i < (size - 1); i++)
		{
			std::tuple<IT, VT> current = m_sourceData[i];
			std::tuple<IT, VT> next = m_sourceData[i + 1];

			if (std::get<0>(current) == idx)
				return std::get<1>(current);
			else if (std::get<0>(current) < idx && std::get<0>(next) > idx)
			{
				targetIndex = i;
				break;
			}
		}

		return _map(idx, std::get<0>(m_sourceData[targetIndex]), std::get<0>(m_sourceData[targetIndex + 1]), std::get<1>(m_sourceData[targetIndex]), std::get<1>(m_sourceData[targetIndex + 1]));
	}

private:
	struct
	{
		bool operator()(std::tuple<IT, VT> it1, std::tuple<IT, VT> it2) const { return std::get<0>(it1) < std::get<0>(it2); }
	}
	customLess;

	VT _map(IT x, IT in_min, IT in_max, VT out_min, VT out_max)
	{
		IT progress = (x - in_min) / (in_max - in_min);
		return (out_max - out_min) * static_cast<VT>(progress) + out_min;
	}

private:
	std::tuple<IT, VT>m_sourceData[size];
};
