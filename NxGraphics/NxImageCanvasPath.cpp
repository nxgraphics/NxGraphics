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

//Paths
void NxImageCanvasContext::beginPath()
{
	mPath->reset();
}

void NxImageCanvasContext::closePath()
{
	mPath->close();
}

void NxImageCanvasContext::fill()
{	
	applyFillStyle();	
	mCanvas->drawPath(*mPath, *mFillStyle);
}

void NxImageCanvasContext::stroke()
{
	applyStrokeStyle();
	mCanvas->drawPath(*mPath, *mStrokeStyle);
	//beginPath(); //is it needed ?
}

void NxImageCanvasContext::clip()
{
	mCanvas->clipPath(*mPath);
}

void NxImageCanvasContext::moveTo(float x, float y)
{
	mPath->moveTo(x, y);
}

void NxImageCanvasContext::moveTo(const Nx::Vector2& pos)
{
	moveTo(pos.x, pos.y);
}

void NxImageCanvasContext::lineTo(float x, float y)
{
	if (!pathHasCurrentPoint(*mPath))
		mPath->moveTo(x, y);
	else
		mPath->lineTo(x, y);
}

void NxImageCanvasContext::lineTo(const Nx::Vector2& pos)
{
	lineTo(pos.x, pos.y);
}

void NxImageCanvasContext::Circle( float x, float y, float Radius )
{
	mPath->addCircle( x, y, Radius, SkPath::kCW_Direction );
}

void NxImageCanvasContext::quadraticCurveTo( float cpx, float cpy, float x, float y )
{
	if (!pathHasCurrentPoint(*mPath))
		mPath->moveTo(x, y);
	else
		mPath->quadTo(cpx, cpy, x, y);
}

void NxImageCanvasContext:: quadraticCurveTo(const Nx::Vector2& controlPoint, const Nx::Vector2& pos)
{
	quadraticCurveTo(controlPoint.x, controlPoint.y, pos.x, pos.y);
}

void NxImageCanvasContext::bezierCurveTo(float cp1x, float cp1y, float cp2x, float cp2y, float x, float y)
{
	if (!pathHasCurrentPoint(*mPath))
		mPath->moveTo(x, y);
	else	
		mPath->cubicTo(cp1x, cp1y, cp2x, cp2y, x, y);
}

void NxImageCanvasContext:: bezierCurveTo(const Nx::Vector2& controlPoint1, const Nx::Vector2& controlPoint2, const Nx::Vector2& pos)
{
	bezierCurveTo(controlPoint1.x, controlPoint1.y, controlPoint2.x, controlPoint2.y, pos.x, pos.y);
}

void NxImageCanvasContext:: arcTo(float x1, float y1, float x2, float y2, float radius)
{
	mPath->arcTo(x1, y1, x2, y2, radius);
}

void NxImageCanvasContext:: arcTo(const Nx::Vector2& pos1, const Nx::Vector2& pos2, float radius)
{
	arcTo(pos1.x, pos1.y, pos2.x, pos2.y, radius);
}

void NxImageCanvasContext::arc(float x, float y, float r, float startRadian, float endRadian, bool anticlockwise)
{
    SkScalar cx = x;
    SkScalar cy = y;
    SkScalar radius = r;
	SkScalar sa = startRadian;
	SkScalar ea = endRadian;	

    SkRect oval;
    oval.set(cx - radius, cy - radius, cx + radius, cy + radius);

    float sweep = ea - sa;
    // check for a circle
	if (sweep >= 2 * Nx::Math::PI || sweep <= -2 * Nx::Math::PI)
        mPath->addOval(oval);
    else {
        SkScalar startDegrees = (sa * 180 / Nx::Math::PI);
        SkScalar sweepDegrees = (sweep * 180 / Nx::Math::PI);

        // Counterclockwise arcs should be drawn with negative sweeps, while
        // clockwise arcs should be drawn with positive sweeps. Check to see
        // if the situation is reversed and correct it by adding or subtracting
        // a full circle
        if (anticlockwise && sweepDegrees > 0) {
            sweepDegrees -= SkIntToScalar(360);
        } else if (!anticlockwise && sweepDegrees < 0) {
            sweepDegrees += SkIntToScalar(360);
        }

        mPath->arcTo(oval, startDegrees, sweepDegrees, false);
    }
}

void NxImageCanvasContext::arc(const Nx::Vector2& pos, float radius, Nx::Radian start, Nx::Radian end, bool anticlockwise)
{
	arc(pos.x, pos.y, radius, start.valueRadians(), end.valueRadians(), anticlockwise);
}

void NxImageCanvasContext::rect(float x, float y, float w, float h)
{
	mPath->addRect(x, y, w+x, h+y);
}

void NxImageCanvasContext::rect(const Nx::Vector2& pos, const Nx::Vector2& dim)
{
	rect(pos.x, pos.y, dim.x, dim.y);
}

bool NxImageCanvasContext::isPointInPath(float x, float y)
{
	return false;
}

bool NxImageCanvasContext::isPointInPath(const Nx::Vector2& pos)
{
	return isPointInPath(pos.x, pos.y);
}




}