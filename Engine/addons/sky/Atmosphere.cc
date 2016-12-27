#include "stdneb.h"
#include "Atmosphere.h"

namespace Sky
{
	__ImplementClass( Atmosphere, 'ATMO', Core::RefCounted);

Atmosphere::Atmosphere()
: m_Options(Options()),
  m_LigntingMode(LM_LDR)
{

}

Atmosphere::~Atmosphere()
{

}

const float Atmosphere::_Scale(const float &cos, const float &uScaleDepth) const
{
	float x = 1.0f - cos;
	
	return uScaleDepth * Math::n_exp(-0.00287f + x*(0.459f + x*(3.83f + x*(-6.80f + x*5.25f))));
}

const Math::float3 Atmosphere::GetColorAt(const Math::float3 &Direction) const
{
	if (Direction.y() < 0)
	{
		return Math::float3(0.0, 0.0, 0.0);
	}

	// Parameters
	float	 Scale			= 1.0f / (m_Options.OuterRadius - m_Options.InnerRadius),
		     ScaleDepth		= (m_Options.OuterRadius - m_Options.InnerRadius) / 2.0f,
		ScaleOverScaleDepth = Scale / ScaleDepth,
		Kr4PI  = float(m_Options.RayleighMultiplier * 4.0f * PI),
		KrESun = m_Options.RayleighMultiplier * m_Options.SunIntensity,
		Km4PI  = float(m_Options.MieMultiplier * 4.0f * PI),
		KmESun = m_Options.MieMultiplier * m_Options.SunIntensity;

	// --- Start vertex program simulation ---

	Math::float3 uLightDir		= - GetSunDirection(),
		         v3Pos			= Direction,
				 uCameraPos		= Math::float3(0.0f, m_Options.InnerRadius + (m_Options.OuterRadius - m_Options.InnerRadius) * m_Options.HeightPosition, 0.0f),
				 uInvWaveLength = Math::float3( 1.0f / Math::n_pow(m_Options.WaveLength.x(), 4.0f),
												1.0f / Math::n_pow(m_Options.WaveLength.y(), 4.0f),
												1.0f / Math::n_pow(m_Options.WaveLength.z(), 4.0f)
												);


	// Get the ray from the camera to the vertex, and it's length (far point)
	v3Pos.y() += m_Options.InnerRadius;

	Math::float3 v3Ray = v3Pos - uCameraPos;
	float fFar        = v3Ray.length();
	v3Ray             /= fFar;

	// Calculate the ray's starting position, then calculate its scattering offset
	Math::float3 v3Start = uCameraPos;
	
	float fHeight		= uCameraPos.y(),
		   fStartAngle  = v3Ray.dotProduct(v3Start) / fHeight,
		   fDepth       = Math::n_exp(ScaleOverScaleDepth * (m_Options.InnerRadius - uCameraPos.y())),
		   fStartOffset = fDepth * _Scale(fStartAngle, ScaleDepth);

	// Init loop variables
	float fSampleLength = fFar / static_cast<float> (m_Options.NumberOfSamples),
		   fScaledLength = fSampleLength * Scale,
		   fHeight_, fDepth_, fLightAngle, fCameraAngle, fScatter;

	Math::float3 v3SampleRay   = v3Ray * fSampleLength,
				 v3SamplePoint = v3Start + v3SampleRay * 0.5,
				 color, v3Attenuate;

	// Loop the ray
	for (IndexT i = 0; i<m_Options.NumberOfSamples; ++i)
	{
		fHeight_ = v3SamplePoint.length();
		fDepth_  = Math::n_exp(ScaleOverScaleDepth * (m_Options.InnerRadius - fHeight_));

		fLightAngle  = uLightDir.dotProduct(v3SamplePoint) / fHeight_;
		fCameraAngle = v3Ray.dotProduct(v3SamplePoint) / fHeight_;

		fScatter = (fStartOffset + fDepth * (_Scale(fLightAngle, ScaleDepth) - _Scale(fCameraAngle, ScaleDepth)));

		v3Attenuate = Math::float3(
			Math::n_exp(-fScatter * (uInvWaveLength.x() * Kr4PI + Km4PI)),
			Math::n_exp(-fScatter * (uInvWaveLength.y() * Kr4PI + Km4PI)),
			Math::n_exp(-fScatter * (uInvWaveLength.z() * Kr4PI + Km4PI))
			);

		// Accumulate color
		v3Attenuate *= (fDepth_ * fScaledLength);
		color       += v3Attenuate;

		// Next sample point
		v3SamplePoint += v3SampleRay;
	}

	// Outputs
	Math::float3 oRayleighColor = color * (uInvWaveLength * KrESun),
				 oMieColor      = color * KmESun,
				 oDirection     = uCameraPos - v3Pos;

	// --- End vertex program simulation ---
	// --- Start fragment program simulation ---

	float cos			 = uLightDir.dotProduct(oDirection) / oDirection.length(),
		   cos2			 = cos*cos,
		   rayleighPhase = (float)0.75 * (1.0f + 0.5f*cos2),
		   g2			 = m_Options.G * m_Options.G,
		   miePhase		 = 1.5f * ((1.0f - g2) / (2.0f + g2)) * 
						  (1.0f + cos2) / Math::n_pow(1.0f + g2 - 2.0f * m_Options.G * cos, 1.5f);

	Math::float3 oColor;

	if (m_LigntingMode == LM_LDR)
	{
		oColor = Math::float3(
				1 - Math::n_exp(-m_Options.Exposure * (rayleighPhase * oRayleighColor.x() + miePhase * oMieColor.x())),
				1 - Math::n_exp(-m_Options.Exposure * (rayleighPhase * oRayleighColor.y() + miePhase * oMieColor.y())),
				1 - Math::n_exp(-m_Options.Exposure * (rayleighPhase * oRayleighColor.z() + miePhase * oMieColor.z()))
				             );
	}
	else
	{
		oColor = rayleighPhase * oRayleighColor + miePhase * oMieColor;
	}

	// For night rendering
	oColor += Math::n_clamp((1 - Math::n_max(oColor.x(), Math::n_max(oColor.y(), oColor.z())) * 10), 0.0f, 1.0f)
		   * (Math::float3(0.05f, 0.05f, 0.1f))
		   * (2 - 0.75f * Math::n_clamp(-uLightDir.y(), 0.0f, 1.0f) * Math::n_pow(1 - Direction.y(), 3));

	// --- End fragment program simulation ---

	// Output color
	return oColor;
}

const Math::float3 Atmosphere::GetSunDirection() const
{
	// 24h day: 
	// 0______A(Sunrise)_______B(Sunset)______24
	//      

	float y,
		  X   = m_Options.Time.x(),
		  A   = m_Options.Time.y(),
		  B   = m_Options.Time.z(),
		  AB  = A+24-B,
		  AB_ = B-A,
		  XB  = X+24-B;

	if (X<A || X>B)
	{
		if (X<A)
		{
			y = -XB / AB;
		}
		else
		{
			y = -(X-B) / AB;
		}

		if (y > -0.5f)
		{
			y *= 2;
		}
		else
		{
			y = -(1 + y) * 2.0f;
		}
	}
	else
	{
		y = (X-A)/(B-A);

		if (y < 0.5f)
		{
			y *= 2.0f;
		}
		else
		{
			y = (1 - y) * 2.0f;
		}
	}

	Math::float2 East  = m_Options.EastPosition;
	East			   = Math::float2::normalize(East);

	if (X > A && X < B)
	{
		if (X > (A + AB_/2))
		{
			East = -East;
		}
	}
	else
	{
		if (X<A)
		{
			if (XB < (24-AB_)/2)
			{
				East = -East;
			}
		}
		else
		{
			if ((X-B) < (24-AB_)/2)
			{
				East = -East;
			}
		}
	}

	float ydeg = (float)(PI/2.0f) * y,
		  sn   = Math::n_sin(ydeg),
		  cs   = Math::n_cos(ydeg);

	Math::float3 SPos = Math::float3(East.x() * cs, sn, East.y() * cs);

	return -SPos;
}

}