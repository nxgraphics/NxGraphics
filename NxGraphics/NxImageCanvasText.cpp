#include "NxGraphicsPreCompiled.h"

#ifndef SK_IGNORE_STDINT_DOT_H
#define SK_IGNORE_STDINT_DOT_H
#endif

#include <core/SkBitmap.h>
#include <core/SkDevice.h>
#include <core/SkPaint.h>
#include <core/SkRect.h>
#include <core/SkPath.h>
#include <effects/SkGradientShader.h>

namespace Nx {


void NxImageCanvasContext::fillText(const std::string& text, float x, float y, float maxWidth)
{
	applyFillStyle();
	mCanvas->drawText(text.c_str(), text.size(), x, y, *mFillStyle);
}

void NxImageCanvasContext::fillText(const std::string& text, const Nx::Vector2& pos, float maxWidth)
{
	fillText(text, pos.x, pos.y, maxWidth);
}

void NxImageCanvasContext::strokeText(const std::string& text, float x, float y, float maxWidth)
{
	applyStrokeStyle();
	mCanvas->drawText(text.c_str(), text.size(), x, y, *mStrokeStyle);
}

void NxImageCanvasContext::strokeText(const std::string& text, const Nx::Vector2& pos, float maxWidth)
{
	strokeText(text, pos.x, pos.y, maxWidth);
}

}