#include "NxGraphicsPreCompiled.h"
 
namespace Nx {
 
	const NxColourValue NxColourValue::ZERO = NxColourValue(0.0,0.0,0.0,0.0);
	const NxColourValue NxColourValue::Black = NxColourValue(0.0,0.0,0.0);
	const NxColourValue NxColourValue::White = NxColourValue(1.0,1.0,1.0);
	const NxColourValue NxColourValue::Red = NxColourValue(1.0,0.0,0.0);
	const NxColourValue NxColourValue::Green = NxColourValue(0.0,1.0,0.0);
	const NxColourValue NxColourValue::Blue = NxColourValue(0.0,0.0,1.0);

	//---------------------------------------------------------------------
#if NXGRAPHICS_ENDIAN == OGRE_ENDIAN_BIG
	ABGR NxColourValue::getAsABGR(void) const
#else
	RGBA NxColourValue::getAsRGBA(void) const
#endif
	{
		uint8 val8;
		uint32 val32 = 0;

		// Convert to 32bit pattern
		// (RGBA = 8888)

		// Red
		val8 = static_cast<uint8>(r * 255);
		val32 = val8 << 24;

		// Green
		val8 = static_cast<uint8>(g * 255);
		val32 += val8 << 16;

		// Blue
		val8 = static_cast<uint8>(b * 255);
		val32 += val8 << 8;

		// Alpha
		val8 = static_cast<uint8>(a * 255);
		val32 += val8;

		return val32;
	}
	//---------------------------------------------------------------------
#if NXGRAPHICS_ENDIAN == OGRE_ENDIAN_BIG
	BGRA NxColourValue::getAsBGRA(void) const
#else
	ARGB NxColourValue::getAsARGB(void) const
#endif
	{
		uint8 val8;
		uint32 val32 = 0;

		// Convert to 32bit pattern
		// (ARGB = 8888)

		// Alpha
		val8 = static_cast<uint8>(a * 255);
		val32 = val8 << 24;

		// Red
		val8 = static_cast<uint8>(r * 255);
		val32 += val8 << 16;

		// Green
		val8 = static_cast<uint8>(g * 255);
		val32 += val8 << 8;

		// Blue
		val8 = static_cast<uint8>(b * 255);
		val32 += val8;


		return val32;
	}
	//---------------------------------------------------------------------
#if NXGRAPHICS_ENDIAN == OGRE_ENDIAN_BIG
	ARGB NxColourValue::getAsARGB(void) const
#else
	BGRA NxColourValue::getAsBGRA(void) const
#endif
	{
		uint8 val8;
		uint32 val32 = 0;

		// Convert to 32bit pattern
		// (ARGB = 8888)

		// Blue
		val8 = static_cast<uint8>(b * 255);
		val32 = val8 << 24;

		// Green
		val8 = static_cast<uint8>(g * 255);
		val32 += val8 << 16;

		// Red
		val8 = static_cast<uint8>(r * 255);
		val32 += val8 << 8;

		// Alpha
		val8 = static_cast<uint8>(a * 255);
		val32 += val8;


		return val32;
	}
	//---------------------------------------------------------------------
#if NXGRAPHICS_ENDIAN == OGRE_ENDIAN_BIG
	RGBA NxColourValue::getAsRGBA(void) const
#else
	ABGR NxColourValue::getAsABGR(void) const
#endif
	{
		uint8 val8;
		uint32 val32 = 0;

		// Convert to 32bit pattern
		// (ABRG = 8888)

		// Alpha
		val8 = static_cast<uint8>(a * 255);
		val32 = val8 << 24;

		// Blue
		val8 = static_cast<uint8>(b * 255);
		val32 += val8 << 16;

		// Green
		val8 = static_cast<uint8>(g * 255);
		val32 += val8 << 8;

		// Red
		val8 = static_cast<uint8>(r * 255);
		val32 += val8;


		return val32;
	}
	//---------------------------------------------------------------------
#if NXGRAPHICS_ENDIAN == OGRE_ENDIAN_BIG
	void NxColourValue::setAsABGR(const ABGR val)
#else
	void NxColourValue::setAsRGBA(const RGBA val)
#endif
	{
		uint32 val32 = val;

		// Convert from 32bit pattern
		// (RGBA = 8888)

		// Red
		r = ((val32 >> 24) & 0xFF) / 255.0f;

		// Green
		g = ((val32 >> 16) & 0xFF) / 255.0f;

		// Blue
		b = ((val32 >> 8) & 0xFF) / 255.0f;

		// Alpha
		a = (val32 & 0xFF) / 255.0f;
	}
	//---------------------------------------------------------------------
#if NXGRAPHICS_ENDIAN == OGRE_ENDIAN_BIG
	void NxColourValue::setAsBGRA(const BGRA val)
#else
	void NxColourValue::setAsARGB(const ARGB val)
#endif
	{
		uint32 val32 = val;

		// Convert from 32bit pattern
		// (ARGB = 8888)

		// Alpha
		a = ((val32 >> 24) & 0xFF) / 255.0f;

		// Red
		r = ((val32 >> 16) & 0xFF) / 255.0f;

		// Green
		g = ((val32 >> 8) & 0xFF) / 255.0f;

		// Blue
		b = (val32 & 0xFF) / 255.0f;
	}
	//---------------------------------------------------------------------
#if NXGRAPHICS_ENDIAN == OGRE_ENDIAN_BIG
	void NxColourValue::setAsARGB(const ARGB val)
#else
	void NxColourValue::setAsBGRA(const BGRA val)
#endif
	{
		uint32 val32 = val;

		// Convert from 32bit pattern
		// (ARGB = 8888)

		// Blue
		b = ((val32 >> 24) & 0xFF) / 255.0f;

		// Green
		g = ((val32 >> 16) & 0xFF) / 255.0f;

		// Red
		r = ((val32 >> 8) & 0xFF) / 255.0f;

		// Alpha
		a = (val32 & 0xFF) / 255.0f;
	}
	//---------------------------------------------------------------------
#if NXGRAPHICS_ENDIAN == OGRE_ENDIAN_BIG
	void NxColourValue::setAsRGBA(const RGBA val)
#else
	void NxColourValue::setAsABGR(const ABGR val)
#endif
	{
		uint32 val32 = val;

		// Convert from 32bit pattern
		// (ABGR = 8888)

		// Alpha
		a = ((val32 >> 24) & 0xFF) / 255.0f;

		// Blue
		b = ((val32 >> 16) & 0xFF) / 255.0f;

		// Green
		g = ((val32 >> 8) & 0xFF) / 255.0f;

		// Red
		r = (val32 & 0xFF) / 255.0f;
	}
	//---------------------------------------------------------------------
	bool NxColourValue::operator==(const NxColourValue& rhs) const
	{
		return (r == rhs.r &&
			g == rhs.g &&
			b == rhs.b &&
			a == rhs.a);
	}
	//---------------------------------------------------------------------
	bool NxColourValue::operator!=(const NxColourValue& rhs) const
	{
		return !(*this == rhs);
	}
	//---------------------------------------------------------------------
	void NxColourValue::setHSB(float hue, float saturation, float brightness)
	{
		// wrap hue
		if (hue > 1.0f)
		{
			hue -= (int)hue;
		}
		else if (hue < 0.0f)
		{
			hue += (int)hue + 1;
		}
		// clamp saturation / brightness
		saturation = std::min(saturation, (Nx::Real)1.0);
		saturation = std::max(saturation, (Nx::Real)0.0);
		brightness = std::min(brightness, (Nx::Real)1.0);
		brightness = std::max(brightness, (Nx::Real)0.0);

		if (brightness == 0.0f)
		{   
			// early exit, this has to be black
			r = g = b = 0.0f;
			return;
		}

		if (saturation == 0.0f)
		{   
			// early exit, this has to be grey

			r = g = b = brightness;
			return;
		}


		Nx::Real hueDomain  = hue * 6.0f;
		if (hueDomain >= 6.0f)
		{
			// wrap around, and allow mathematical errors
			hueDomain = 0.0f;
		}
		unsigned short domain = (unsigned short)hueDomain;
		Nx::Real f1 = brightness * (1 - saturation);
		Nx::Real f2 = brightness * (1 - saturation * (hueDomain - domain));
		Nx::Real f3 = brightness * (1 - saturation * (1 - (hueDomain - domain)));

		switch (domain)
		{
		case 0:
			// red domain; green ascends
			r = brightness;
			g = f3;
			b = f1;
			break;
		case 1:
			// yellow domain; red descends
			r = f2;
			g = brightness;
			b = f1;
			break;
		case 2:
			// green domain; blue ascends
			r = f1;
			g = brightness;
			b = f3;
			break;
		case 3:
			// cyan domain; green descends
			r = f1;
			g = f2;
			b = brightness;
			break;
		case 4:
			// blue domain; red ascends
			r = f3;
			g = f1;
			b = brightness;
			break;
		case 5:
			// magenta domain; blue descends
			r = brightness;
			g = f1;
			b = f2;
			break;
		}


	}
	//---------------------------------------------------------------------
	void NxColourValue::getHSB(float* hue, float* saturation, float* brightness) const
	{

		Nx::Real vMin = std::min(r, std::min(g, b));
		Nx::Real vMax = std::max(r, std::max(g, b));
		Nx::Real delta = vMax - vMin;

		*brightness = vMax;

		if (Nx::Math::RealEqual(delta, 0.0f, 1e-6))
		{
			// grey
			*hue = 0;
			*saturation = 0;
		}
		else                                    
		{
			// a colour
			*saturation = delta / vMax;

			Nx::Real deltaR = (((vMax - r) / 6.0f) + (delta / 2.0f)) / delta;
			Nx::Real deltaG = (((vMax - g) / 6.0f) + (delta / 2.0f)) / delta;
			Nx::Real deltaB = (((vMax - b) / 6.0f) + (delta / 2.0f)) / delta;

			if (Nx::Math::RealEqual(r, vMax))
				*hue = deltaB - deltaG;
			else if (Nx::Math::RealEqual(g, vMax))
				*hue = 0.3333333f + deltaR - deltaB;
			else if (Nx::Math::RealEqual(b, vMax)) 
				*hue = 0.6666667f + deltaG - deltaR;

			if (*hue < 0.0f) 
				*hue += 1.0f;
			if (*hue > 1.0f)
				*hue -= 1.0f;
		}


	}

}

