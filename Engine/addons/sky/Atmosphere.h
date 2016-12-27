#pragma once

#include "core/refcounted.h"
#include "math/float3.h"
#include "math/float2.h"

namespace Sky
{
	class Atmosphere : public Core::RefCounted
{
	__DeclareClass(Atmosphere);

public:

	/** Atmosphere options 
	*/

	struct  Options
	{
		/// Time information: x = time in [0, 24]h range, y = sunrise hour in [0, 24]h range, z = sunset hour in [0, 24] range
		Math::float3 Time;

		/// East position
		Math::float2 EastPosition;

		/// Inner atmosphere radius
		Math::scalar InnerRadius;
		/// Outer atmosphere radius
		Math::scalar OuterRadius;
		/// Height position, in [0, 1] range, 0=InnerRadius, 1=OuterRadius
		Math::scalar HeightPosition;

		/// Rayleigh multiplier
		Math::scalar RayleighMultiplier;
		/// Mie multiplier
		Math::scalar MieMultiplier;
		/// Sun intensity
		Math::scalar SunIntensity;

		/// WaveLength for RGB channels
		Math::float3 WaveLength;

		/// Phase function
		Math::scalar G;

		/// Exposure coeficient
		Math::scalar Exposure;

		/// Number of samples
		int NumberOfSamples;

		/** Default constructor
		*/
		Options()
			: Time(Math::float3(7.0f, 7.50f, 20.50f))
			, EastPosition(Math::float2(1.0f,0.0f))
			, InnerRadius(9.77501f)
			, OuterRadius(10.2963f)
			, HeightPosition(0.01f)
			, RayleighMultiplier(0.0022f)
			, MieMultiplier(0.000675f)
			, SunIntensity(30.0f)
			, WaveLength(Math::float3(0.57f, 0.54f, 0.44f))
			, G(-0.991f)
			, Exposure(2.0f)
			, NumberOfSamples(4)
		{
		}

		/** Simple constructor
		@param _Time Time information: x = time in [0, 24]h range, y = sunrise hour in [0, 24]h range, z = sunset hour in [0, 24] range
		*/
		Options(const Math::float3& _Time)
			: Time(_Time)
			, EastPosition(Math::float2(1.0f,0.0f))
			, InnerRadius(9.77501f)
			, OuterRadius(10.2963f)
			, HeightPosition(0.01f)
			, RayleighMultiplier(0.0022f)
			, MieMultiplier(0.000675f)
			, SunIntensity(30)
			, WaveLength(Math::float3(0.57f, 0.54f, 0.44f))
			, G(-0.991f)
			, Exposure(2.0f)
			, NumberOfSamples(4)
		{
		}

		/** Constructor
		@param _Time Time information: x = time in [0, 24]h range, y = sunrise hour in [0, 24]h range, z = sunset hour in [0, 24] range
		@param _EastPosition East position
		@param _InnerRadius Inner atmosphere radius
		@param _OuterRadius Outer atmosphere radius
		@param _HeightPosition Height position, in [0, 1] range, 0=InnerRadius, 1=OuterRadius
		@param _RayleighMultiplier Rayleigh multiplier
		@param _MieMultiplier Mie multiplier
		@param _SunIntensity Sun intensity
		@param _WaveLength Wave length for RGB channels
		@param _G Phase function
		@param _Exposure Exposure
		@param _NumerOfSamples Number of samples
		*/
		Options(const Math::float3& _Time,
			const Math::float2& _EastPosition,
			const Math::scalar&	 _InnerRadius,
			const Math::scalar&	 _OuterRadius,
			const Math::scalar&	 _HeightPosition,
			const Math::scalar&	 _RayleighMultiplier,
			const Math::scalar&	 _MieMultiplier,
			const Math::scalar&  _SunIntensity,
			const Math::float3&  _WaveLength,
			const Math::scalar&  _G,
			const Math::scalar&  _Exposure,
			const int&			 _NumerOfSamples)
			: Time(_Time)
			, EastPosition(_EastPosition)
			, InnerRadius(_InnerRadius)
			, OuterRadius(_OuterRadius)
			, HeightPosition(_HeightPosition)
			, RayleighMultiplier(_RayleighMultiplier)
			, MieMultiplier(_MieMultiplier)
			, SunIntensity(_SunIntensity)
			, WaveLength(_WaveLength)
			, G(_G)
			, Exposure(_Exposure)
			, NumberOfSamples(_NumerOfSamples)
		{
		}
	};

	enum LightMode
	{
		LM_LDR = 0,
		LM_HDR
	};

	Atmosphere();

	~Atmosphere();

	/** Set options
	@param _Options New options
	*/
	void SetOptions(const Options& _Options);

	/** Get current options 
	@return Current options
	*/
	Options& GetOptions();
		
	/** Get current atmosphere color at the given direction
	@param Direction *Normalised* direction
	@return Atmosphere color at the especified direction
	*/
	const Math::float3 GetColorAt(const Math::float3& Direction) const;

	/** Get sun direction
	@return Sun direction
	*/
	const Math::float3 GetSunDirection() const;


	void  SetLightingMode(int mode = LM_LDR);

	const Math::float3&  GetTime();

	const Math::float2&  GetEastPosition();

	float GetInnerRadius();

	float GetOuterRadius();

	float GetHeightPosition();

	float GetRayleighMultiplier();

	float GetMieMultiplier();

	float GetSunIntensity();

	Math::float3 GetWaveLength();

	float GetG();

	float GetExposure();

	int   GetNumberOfSamples();

private:
	/** Shader scale funtion 
	@param cos Cos
	@param uScaleDepth Scale Depth
	@return Scale
	*/
	const float _Scale(const float& cos, const float& uScaleDepth) const;

	Options	 m_Options;

	int      m_LigntingMode;
};

inline void Atmosphere::SetOptions(const Options& _Options)
{
	m_Options = _Options;
}

inline Atmosphere::Options& Atmosphere::GetOptions()
{
	return m_Options;
}

inline void Atmosphere::SetLightingMode(int mode)
{
	m_LigntingMode = mode;
}

inline const Math::float3& Atmosphere::GetTime()
{
	return m_Options.Time;
}

inline const Math::float2& Atmosphere::GetEastPosition()
{
	return m_Options.EastPosition;
}

inline float Atmosphere::GetInnerRadius()
{
	return m_Options.InnerRadius;
}

inline float Atmosphere::GetOuterRadius()
{
	return m_Options.OuterRadius;
}

inline float Atmosphere::GetHeightPosition()
{
	return m_Options.HeightPosition;
}

inline float Atmosphere::GetMieMultiplier()
{
	return m_Options.MieMultiplier;
}

inline float Atmosphere::GetSunIntensity()
{
	return m_Options.SunIntensity;
}

inline Math::float3 Atmosphere::GetWaveLength()
{
	return m_Options.WaveLength;
}

inline float Atmosphere::GetG()
{
	return m_Options.G;
}

inline float Atmosphere::GetExposure()
{
	return m_Options.Exposure;
}

inline int Atmosphere::GetNumberOfSamples()
{
	return m_Options.NumberOfSamples;
}

}
