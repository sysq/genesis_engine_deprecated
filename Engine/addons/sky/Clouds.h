#pragma once

#include "core/refcounted.h"
#include "math/float3.h"
#include "math/float2.h"

#include "ColorGradient.h"

namespace Sky
{
	class CloudLayer : public Core::RefCounted
{
	__DeclareClass(CloudLayer);

public:

	struct Options
	{
		/// Cloud layer height
		Math::scalar Height;
		/// Cloud layer scale
		Math::scalar Scale;
		/// Wind direction
		Math::float2 WindDirection;
		/// Time multiplier
		Math::scalar TimeMultiplier;

		/// Distance attenuation
		Math::scalar DistanceAttenuation;
		/// Detail attenuation
		Math::scalar DetailAttenuation;
		/// Normal multiplier
		Math::scalar NormalMultiplier;
		/// Cloud layer height volume(For volumetric effects on the gpu)
		Math::scalar HeightVolume;
		/// Volumetric displacement(For volumetric effects on the gpu)
		Math::scalar VolumetricDisplacement;

		/** Default constructor
		*/
		Options()
			: Height(100)
			, Scale(0.001f)
			, WindDirection(Math::float2(1,1))
			, TimeMultiplier(0.125f)
			, DistanceAttenuation(0.05f)
			, DetailAttenuation(1)
			, NormalMultiplier(2)
			, HeightVolume(0.25f)
			, VolumetricDisplacement(0.01f)
		{
		}

		/** Constructor
		@param _Height Cloud layer height
		@param _Scale Clouds scale
		@param _WindDirection Clouds movement direction
		@param _TimeMultiplier Time multiplier factor
		*/
		Options(const Math::scalar& _Height, 
			const Math::scalar& _Scale, 
			const Math::float2& _WindDirection, 
			const Math::scalar& _TimeMultiplier)
			: Height(_Height)
			, Scale(_Scale)
			, WindDirection(_WindDirection)
			, TimeMultiplier(_TimeMultiplier)
			, DistanceAttenuation(0.05f)
			, DetailAttenuation(1)
			, NormalMultiplier(2)
			, HeightVolume(0.25f)
			, VolumetricDisplacement(0.01f)
		{
		}

		/** Constructor
		@param _Height Cloud layer height
		@param _Scale Clouds scale
		@param _WindDirection Clouds movement direction
		@param _TimeMultiplier Time multiplier factor
		@param _DistanceAttenuation Distance attenuation
		@param _DetailAttenuation Detail attenuation
		@param _NormalMultiplier Normal multiplier coeficient
		@param _HeightVolume Height volume(For volumetric effects on the gpu)
		@param _VolumetricDisplacement Volumetric displacement(For volumetric effects on the gpu)

		*/
		Options(const Math::scalar& _Height, 
			const Math::scalar& _Scale, 
			const Math::float2& _WindDirection, 
			const Math::scalar& _TimeMultiplier,
			const Math::scalar& _DistanceAttenuation,
			const Math::scalar& _DetailAttenuation,
			const Math::scalar& _NormalMultiplier,
			const Math::scalar& _HeightVolume,
			const Math::scalar& _VolumetricDisplacement)
			: Height(_Height)
			, Scale(_Scale)
			, WindDirection(_WindDirection)
			, TimeMultiplier(_TimeMultiplier)
			, DistanceAttenuation(_DistanceAttenuation)
			, DetailAttenuation(_DetailAttenuation)
			, NormalMultiplier(_NormalMultiplier)
			, HeightVolume(_HeightVolume)
			, VolumetricDisplacement(_VolumetricDisplacement)
		{
		}
	};

	CloudLayer();

	CloudLayer(const Options& o);

	~CloudLayer();

	const Options& GetOptions() const;

	void SetAmbientGradient(const ColorGradient& AmbientGradient);

	const ColorGradient& GetAmbientGradient() const;

	void SetSunGradient(const ColorGradient& SunGradient);

	const ColorGradient& GetSunGradient() const;

private:

	Options         m_Options;

	ColorGradient   m_AmbientGradient;
	ColorGradient   m_SunGradient;
};

inline const CloudLayer::Options& CloudLayer::GetOptions() const
{
	return m_Options;
}

inline void CloudLayer::SetAmbientGradient(const ColorGradient& AmbientGradient)
{
	m_AmbientGradient = AmbientGradient;
}

inline const ColorGradient& CloudLayer::GetAmbientGradient() const
{
	return m_AmbientGradient;
}

inline void CloudLayer::SetSunGradient(const ColorGradient& SunGradient)
{
	m_SunGradient = SunGradient;
}

inline const ColorGradient& CloudLayer::GetSunGradient() const
{
	return m_SunGradient;
}

}