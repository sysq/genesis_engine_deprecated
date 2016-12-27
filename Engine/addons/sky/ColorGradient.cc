#include "stdneb.h"
#include "ColorGradient.h"

namespace Sky
{

ColorGradient::ColorGradient()
: m_bMalFormed(true)
{

}

ColorGradient::~ColorGradient()
{

}

const Math::float3 ColorGradient::GetColor(const Math::scalar &gp) const
{
	if (m_bMalFormed)
	{
		return Math::float3(0.0, 0.0, 0.0);
	}

	if (m_CFrameVector.Size() == 0)
	{
		return Math::float3(0.0, 0.0, 0.0);
	}
	else if (m_CFrameVector.Size() == 1)
	{
		return m_CFrameVector.Front().Key();
	}

	Util::KeyValuePair<int, Math::scalar> minBound, maxBound;

	// Min value
	minBound.Key()   = 0;
	minBound.Value() = -1;

	for (IndexT k = 0; k < m_CFrameVector.Size(); ++k)
	{
		if (m_CFrameVector[k].Value() < gp && m_CFrameVector[k].Value() > minBound.Value())
		{
			minBound.Key()   = k;
			minBound.Value() = m_CFrameVector[k].Value();
		}
	}

	// Max value
	maxBound.Key()   = 0;
	maxBound.Value() = 2;

	for (IndexT k = 0; k < m_CFrameVector.Size(); ++k)
	{
		if (m_CFrameVector[k].Value() > gp && m_CFrameVector[k].Value() < maxBound.Value())
		{
			maxBound.Key()   = k;
			maxBound.Value() = m_CFrameVector[k].Value();
		}
	}

	float range      = maxBound.Value() - minBound.Value();
	float rangepoint = (gp - minBound.Value()) / range;

	return m_CFrameVector[minBound.Key()].Key() * (1 - rangepoint) + 
		   m_CFrameVector[maxBound.Key()].Key() * rangepoint;

}

const bool ColorGradient::CheckBounds() const
{
	Util::KeyValuePair<bool, bool> existbounds;

	existbounds.Key()   = false;
	existbounds.Value() = false;

	for (IndexT k = 0; k < m_CFrameVector.Size(); ++k)
	{
		if (m_CFrameVector[k].Value() == 0)
		{
			// More than one min bound
			if (existbounds.Key())
			{
				return false;
			}

			existbounds.Key() = true;
		}

		if (m_CFrameVector[k].Value() < 0 || m_CFrameVector[k].Value() > 1)
		{
			return false;
		}
	}

	for (IndexT k = 0; k < m_CFrameVector.Size(); ++k)
	{
		if (m_CFrameVector[k].Value() == 1)
		{
			// More than one min bound
			if (existbounds.Value())
			{
				return false;
			}

			existbounds.Value() = true;
		}	
	}

	if (! existbounds.Key() || ! existbounds.Value())
	{
		return false;
	}

	return true;
}

}