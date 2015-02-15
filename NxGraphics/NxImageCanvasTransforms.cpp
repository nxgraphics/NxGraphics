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

//Transformation
void NxImageCanvasContext::scale(float x, float y)
{
	mCanvas->scale(x, y);
}

void NxImageCanvasContext::scale(const Nx::Vector2& scaling)
{
	scale(scaling.x, scaling.y);
}

void NxImageCanvasContext::rotate(float radian)
{
	mCanvas->rotate((SkScalar)(radian*180.0/Nx::Math::PI));
}

void NxImageCanvasContext::rotate(const Nx::Radian& angle)
{
	rotate(angle.valueRadians());
}

void NxImageCanvasContext::translate(float x, float y)
{
	mCanvas->translate(x, y);
}

void NxImageCanvasContext::translate(const Nx::Vector2& translation)
{
	translate(translation.x, translation.y);
}

void NxImageCanvasContext::transform(float m11, float m12, float m21, float m22, float dx, float dy)
{
	SkMatrix matrix;
	matrix.set(0, m11); //scaleX
	matrix.set(1, m21); //skewX
	matrix.set(2, dx);  //translateX
	matrix.set(3, m12); //skewY
	matrix.set(4, m22); //scaleY
	matrix.set(5, dy);  //translateY
	matrix.set(6, 0);   //perspX
	matrix.set(7, 0);   //perspY
	matrix.set(8, 1);
	mCanvas->concat(matrix);
}

void NxImageCanvasContext::transform(const Nx::Matrix3& transform)
{
	/*
	transform(transform[0][0], transform[0][0], 
		      transform[0][0], transform[0][0], 
			  transform[0][0], transform[0][0]);
	*/
}

void NxImageCanvasContext::setTransform(float m11, float m12, float m21, float m22, float dx, float dy)
{
	SkMatrix matrix;
	matrix.set(0, m11); //scaleX
	matrix.set(1, m21); //skewX
	matrix.set(2, dx);  //translateX
	matrix.set(3, m12); //skewY
	matrix.set(4, m22); //scaleY
	matrix.set(5, dy);  //translateY
	matrix.set(6, 0);   //perspX
	matrix.set(7, 0);   //perspY
	matrix.set(8, 1);
	mCanvas->setMatrix(matrix);
}

void NxImageCanvasContext::setTransform(const Nx::Matrix3& transform)
{
	/*
	setTransform(transform[0][0], transform[0][0], 
		         transform[0][0], transform[0][0], 
			     transform[0][0], transform[0][0]);
	*/
}


}