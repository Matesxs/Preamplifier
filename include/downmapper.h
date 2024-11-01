#pragma once
#include <cassert>

namespace Downmapper
{
	template<typename T>
	class LatchingDownmapper
	{
	public:
		LatchingDownmapper(const T inMin, const T inMax, const T outMin, const T outMax, const double latchingCoef = 0.5, const T initValue = 0) :
			m_inMin(inMin), m_inMax(inMax),
			m_outMin(outMin), m_outMax(outMax),
			m_inSpan(inMax - inMin), m_outSpan(outMax - outMin),
			m_value(initValue)
		{
			assert(m_inSpan != 0);
			assert(m_outSpan != 0);
			assert(m_inSpan != m_outSpan);
			assert(m_inSpan > m_outSpan);
			assert(latchingCoef < 2.0 && latchingCoef >= 0.0);

			m_distance = m_inSpan / m_outSpan;
			m_latchingDistance = static_cast<T>(m_distance * latchingCoef);
		}

		T Map(const T value)
		{
			const T mapped = (value - m_inMin) / m_distance + m_outMin;

			if (mapped != m_value)
			{
				const T prevUnmapped = (m_value - m_outMin) * m_distance + m_inMin;

				T unmappedDistance = value - prevUnmapped;
				if (unmappedDistance < 0)
					unmappedDistance = -unmappedDistance;

				if (unmappedDistance >= m_latchingDistance)
					m_value = mapped;
			}

			return m_value;
		}

	private:
		T m_value;
		const T m_inMin, m_inMax, m_outMin, m_outMax;
		T m_distance;
		T m_latchingDistance;
		const T m_inSpan, m_outSpan;
	};
}
