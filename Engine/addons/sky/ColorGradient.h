#pragma once

#include "util/array.h"
#include "util/dictionary.h"
#include "math/float3.h"
#include "math/float2.h"


namespace Sky
{
	class ColorGradient
{

public:
	typedef Util::KeyValuePair<Math::float3, Math::scalar> ColorFrame;

	ColorGradient();

	~ColorGradient();

	/** Add color frame
	@param CFrame Color frame
	*/
	void AddCFrame(const ColorFrame& CFrame);

	/** Clear color gradient
	*/
	void Clear();

	/** Get color value
	@param p The gradient point in [0,1] range
	@return Color at the given gradient position
	*/
	const Math::float3 GetColor(const Math::scalar& gp) const;

private:
	/** Check bounds
	@return false if the Color gradient is mal-formed
	*/
	const bool CheckBounds() const;

	bool  m_bMalFormed;

	Util::Array<ColorFrame> m_CFrameVector;
};

inline void ColorGradient::AddCFrame(const ColorFrame& CFrame)
{
	m_CFrameVector.Append(CFrame);

	m_bMalFormed = !CheckBounds();
}

inline void ColorGradient::Clear()
{
	m_CFrameVector.Clear();
}

}