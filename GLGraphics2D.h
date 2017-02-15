// GLView - OpenGL controls for .NET
// Copyright (C) 2009 Ozgur Ozcitak
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// Ozgur Ozcitak (ozcitak@yahoo.com)

#pragma once

#include <windows.h>
#include <GL/gl.h>
#include "GLVertexArray.h"

using namespace System;

namespace GLView {

	// Forward class declarations
	ref class GLCanvas2D;

	/// <summary>
	/// Contains methods for drawing on the canvas.
	/// </summary>
	public ref class GLGraphics2D
	{
	// Constructor/destructor
	internal:
		GLGraphics2D(GLCanvas2D ^ Canvas, Drawing::Graphics ^ GDIGraphics);

	protected:
		~GLGraphics2D() // Dispose
		{ 
			// Clear arrays
			mTriangles->Clear();
			mLines->Clear();
			mTexts->Clear();
		}

	// Privat classes
	private:
		value class GLTextParam
		{
		public:
			float x, y, height;
			System::String ^ text;
			Drawing::Color color;
			bool vectortext;

			GLTextParam(float X, float Y, float Height, System::String ^ Text, Drawing::Color TextColor, bool VectorText)
			{
				x = X; y = Y;
				height = Height;
				text = Text;
				color = TextColor;
				vectortext = VectorText;	
			}
		};

    // Member variables
	private:
		bool mInit;
		float mLineWidth;
		float mZ;
		Drawing::RectangleF mView;
		System::Drawing::Graphics^ mGDIGraphics;
		GLCanvas2D^ mCanvas;
		GLVertexArray^ mTriangles;
		GLVertexArray^ mLines;
		System::Collections::Generic::List<GLTextParam> ^ mTexts;
		Drawing::PointF mBL, mTR;

	// Helper methods
	private:
		/// <summary>
		/// Returns the number of lines required to properly approximate a curve with the given feature size.
		/// </summary>
		/// <param name="featureSize">The size of the feature (circle perimeter, 
		/// arc length etc.) in model coordinates.</param>
		/// <returns>Number of lines required to approximate the given model size.</returns>
		int GetCirclePrecision(float featureSize);
		/// <summary>
		/// Updates the Z coordinate of the next drawing object, so that new objects
		/// are drawn on top of old ones.
		/// </summary>
		System::Void UpdateDepth() 
		{ 
			mZ += 0.000001f; 
		};
		/// <summary>
		/// Updates drawing limits to enclose the given coordinates.
		/// </summary>
		/// <param name="x">X coordinate</param>
		/// <param name="y">Y coordinate</param>
		System::Void UpdateLimits(float x, float y)
		{
			if (mInit)
			{
				mBL.X = Math::Min(mBL.X, x);
				mBL.Y = Math::Min(mBL.Y, y);
				mTR.X = Math::Max(mTR.X, x);
				mTR.Y = Math::Max(mTR.Y, y);
			}
			else
			{
				mBL = Drawing::PointF(x, y);
				mTR = Drawing::PointF(x, y);
				mInit = true;
			}
		}

	// Properties
	public:
		/// <summary>
		/// Gets or sets the current line width.
		/// </summary>
		property float LineWidth
		{
			virtual float get(void) 
			{ 
				return mLineWidth; 
			}
			virtual void set(float value) 
			{ 
				mLineWidth = value;
				glLineWidth(value);
			}
		}
		
	internal:
		/// <summary>
		/// The GLGraphics class collects drawing objects in arrays. No drawing is actually 
		/// performed until Render() is called. Render() is automatically called by the containing 
		/// canvas class. Do not call Render() manually from your code.
		/// </summary>
		Drawing::RectangleF Render();

	public:
		/// <summary>
		/// Draws raster text at the given coordinates.
		/// </summary>
		/// <param name="x">X coordinate</param>
		/// <param name="y">Y coordinate</param>
		/// <param name="text">Text to draw</param>
		/// <param name="color">Drawing color</param>
		System::Void DrawRasterText(float x, float y, System::String ^ text, Drawing::Color color);
		/// <summary>
		/// Draws raster text at the given coordinates.
		/// </summary>
		/// <param name="ptf">Location</param>
		/// <param name="text">Text to draw</param>
		/// <param name="color">Drawing color</param>
		System::Void DrawRasterText(Drawing::PointF ptf, System::String ^ text, Drawing::Color color)
		{
			DrawRasterText(ptf.X, ptf.Y, text, color);
		}
		/// <summary>
		/// Draws vector text at the given coordinates.
		/// </summary>
		/// <param name="x">X coordinate</param>
		/// <param name="y">Y coordinate</param>
		/// <param name="height">Text height</param>
		/// <param name="text">Text to draw</param>
		/// <param name="color">Drawing color</param>
		System::Void DrawVectorText(float x, float y, float height, System::String ^ text, Drawing::Color color);
		/// <summary>
		/// Draws vector text at the given coordinates.
		/// </summary>
		/// <param name="ptf">Location</param>
		/// <param name="height">Text height</param>
		/// <param name="text">Text to draw</param>
		/// <param name="color">Drawing color</param>
		System::Void DrawVectorText(Drawing::PointF ptf, float height, System::String ^ text, Drawing::Color color)
		{
			DrawVectorText(ptf.X, ptf.Y, height, text, color);
		}
		/// <summary>
		/// Draws a line connecting the given points.
		/// </summary>
		/// <param name="x1">X coordinate of first point</param>
		/// <param name="y1">Y coordinate of first point</param>
		/// <param name="x2">X coordinate of second point</param>
		/// <param name="y2">Y coordinate of second point</param>
		/// <param name="color">Drawing color</param>
		System::Void DrawLine(float x1, float y1, float x2, float y2, Drawing::Color color);
		/// <summary>
		/// Draws a line connecting the given points.
		/// </summary>
		/// <param name="pt1">Location of first point</param>
		/// <param name="pt2">Location of second point</param>
		/// <param name="color">Drawing color</param>
		System::Void DrawLine(Drawing::PointF pt1, Drawing::PointF pt2, Drawing::Color color)
		{
			DrawLine(pt1.X, pt1.Y, pt2.X, pt2.Y, color);
		}
		/// <summary>
		/// Draws a line with the given thickness in world units.
		/// </summary>
		/// <param name="x1">X coordinate of first point</param>
		/// <param name="y1">Y coordinate of first point</param>
		/// <param name="x2">X coordinate of second point</param>
		/// <param name="y2">Y coordinate of second point</param>
		/// <param name="thickness">Line thickness</param>
		/// <param name="color">Drawing color</param>
		System::Void DrawLine(float x1, float y1, float x2, float y2, float thickness, Drawing::Color color);
		/// <summary>
		/// Draws a line with the given thickness in world units.
		/// </summary>
		/// <param name="pt1">Location of first point</param>
		/// <param name="pt2">Location of second point</param>
		/// <param name="thickness">Line thickness</param>
		/// <param name="color">Drawing color</param>
		System::Void DrawLine(Drawing::PointF pt1, Drawing::PointF pt2, float thickness, Drawing::Color color)
		{
			DrawLine(pt1.X, pt1.Y, pt2.X, pt2.Y, thickness, color);
		}
		/// <summary>
		/// Draws a line with the given start and end thicknesses in world units.
		/// </summary>
		/// <param name="x1">X coordinate of first point</param>
		/// <param name="y1">Y coordinate of first point</param>
		/// <param name="x2">X coordinate of second point</param>
		/// <param name="y2">Y coordinate of second point</param>
		/// <param name="startthickness">Starting line thickness</param>
		/// <param name="endthickness">Ending line thickness</param>
		/// <param name="color">Drawing color</param>
		System::Void DrawLine(float x1, float y1, float x2, float y2, float startthickness, float endthickness, Drawing::Color color);
		/// <summary>
		/// Draws a line with the given start and end thicknesses in world units.
		/// </summary>
		/// <param name="pt1">Location of first point</param>
		/// <param name="pt2">Location of second point</param>
		/// <param name="startthickness">Starting line thickness</param>
		/// <param name="endthickness">Ending line thickness</param>
		/// <param name="color">Drawing color</param>
		System::Void DrawLine(Drawing::PointF pt1, Drawing::PointF pt2, float startthickness, float endthickness, Drawing::Color color)
		{
			DrawLine(pt1.X, pt1.Y, pt2.X, pt2.Y, startthickness, endthickness, color);
		}
		/// <summary>
		/// Draws an elliptic arc specified by center coordinates, a width, and a height.
		/// </summary>
		/// <param name="x">X coordinate of the center of the arc</param>
		/// <param name="y">Y coordinate of the center of the arc</param>
		/// <param name="width">Width of the ellipse</param>
		/// <param name="height">Height of the ellipse</param>
		/// <param name="startAngle">Start angle in radians measured counter-clockwise from the x-axis</param>
		/// <param name="sweepAngle">Sweep angle in radians measured counter-clockwise</param>
		/// <param name="color">Drawing color</param>
		System::Void DrawArc(float x, float y, float width, float height, float startAngle, float sweepAngle, Drawing::Color color);
		/// <summary>
		/// Draws an elliptic arc specified by center coordinates, and size.
		/// </summary>
		/// <param name="pt">Location of the center of the arc</param>
		/// <param name="sz">Size of the ellipse</param>
		/// <param name="startAngle">Start angle in radians measured counter-clockwise from the x-axis</param>
		/// <param name="sweepAngle">Sweep angle in radians measured counter-clockwise</param>
		/// <param name="color">Drawing color</param>
		System::Void DrawArc(Drawing::PointF pt, Drawing::SizeF sz, float startAngle, float sweepAngle, Drawing::Color color)
		{
			DrawArc(pt.X, pt.Y, sz.Width, sz.Height, startAngle, sweepAngle, color);
		}
		/// <summary>
		/// Draws a triangle specified by the given corner points.
		/// </summary>
		/// <param name="x1">X coordinate of first corner point</param>
		/// <param name="y1">Y coordinate of first corner point</param>
		/// <param name="x2">X coordinate of second corner point</param>
		/// <param name="y2">Y coordinate of second corner point</param>
		/// <param name="x3">X coordinate of third corner point</param>
		/// <param name="y3">Y coordinate of third corner point</param>
		/// <param name="color">Drawing color</param>
		System::Void DrawTriangle(float x1, float y1, float x2, float y2,float x3,float y3, Drawing::Color color);
		/// <summary>
		/// Draws a triangle specified by the given corner points.
		/// </summary>
		/// <param name="pt1">Location of first corner point</param>
		/// <param name="pt2">Location of second corner point</param>
		/// <param name="pt3">Location of third corner point</param>
		/// <param name="color">Drawing color</param>
		System::Void DrawTriangle(Drawing::PointF pt1, Drawing::PointF pt2, Drawing::PointF pt3, Drawing::Color color)
		{
			DrawTriangle(pt1.X, pt1.Y, pt2.X, pt2.Y, pt3.X, pt3.Y, color);
		}
		/// <summary>
		/// Draws a rectangle specified by the given corner points.
		/// </summary>
		/// <param name="x1">X coordinate of first corner point</param>
		/// <param name="y1">Y coordinate of first corner point</param>
		/// <param name="x2">X coordinate of second corner point</param>
		/// <param name="y2">Y coordinate of second corner point</param>
		/// <param name="color">Drawing color</param>
		System::Void DrawRectangle(float x1, float y1, float x2, float y2, Drawing::Color color);
		/// <summary>
		/// Draws a rectangle.
		/// </summary>
		/// <param name="rec">Rectangle to draw</param>
		/// <param name="color">Drawing color</param>
		System::Void DrawRectangle(Drawing::RectangleF rec, Drawing::Color color)
		{
			DrawRectangle(rec.Left, rec.Bottom, rec.Right, rec.Top, color);
		}
		/// <summary>
		/// Draws a rounded rectangle specified by the given corner points and corner radii.
		/// </summary>
		/// <param name="x1">X coordinate of first corner point</param>
		/// <param name="y1">Y coordinate of first corner point</param>
		/// <param name="x2">X coordinate of second corner point</param>
		/// <param name="y2">Y coordinate of second corner point</param>
		/// <param name="rx">X fillet radius</param>
		/// <param name="ry">Y fillet radius</param>
		/// <param name="color">Drawing color</param>
		System::Void DrawRoundedRectangle(float x1, float y1, float x2, float y2, float rx, float ry, Drawing::Color color);
		/// <summary>
		/// Draws a rounded rectangle specified by the given corner points and corner radius.
		/// </summary>
		/// <param name="x1">X coordinate of first corner point</param>
		/// <param name="y1">Y coordinate of first corner point</param>
		/// <param name="x2">X coordinate of second corner point</param>
		/// <param name="y2">Y coordinate of second corner point</param>
		/// <param name="r">Fillet radius</param>
		/// <param name="color">Drawing color</param>
		System::Void DrawRoundedRectangle(float x1, float y1, float x2, float y2, float r, Drawing::Color color)
		{
			DrawRoundedRectangle(x1, y1, x2, y2, r, r, color);
		}
		/// <summary>
		/// Draws a rounded rectangle.
		/// </summary>
		/// <param name="rec">Rectangle to draw</param>
		/// <param name="rx">X fillet radius</param>
		/// <param name="ry">Y fillet radius</param>
		/// <param name="color">Drawing color</param>
		System::Void DrawRoundedRectangle(Drawing::RectangleF rec, float rx, float ry, Drawing::Color color)
		{
			DrawRoundedRectangle(rec.Left, rec.Bottom, rec.Right, rec.Top, rx, ry, color);
		}
		/// <summary>
		/// Draws a rounded rectangle.
		/// </summary>
		/// <param name="rec">Rectangle to draw</param>
		/// <param name="r">Fillet radius</param>
		/// <param name="color">Drawing color</param>
		System::Void DrawRoundedRectangle(Drawing::RectangleF rec, float r, Drawing::Color color)
		{
			DrawRoundedRectangle(rec, r, r, color);
		}
		/// <summary>
		/// Draws an ellipse specified by center coordinates, a width, and a height.
		/// </summary>
		/// <param name="x">X coordinate of the center of the arc</param>
		/// <param name="y">Y coordinate of the center of the arc</param>
		/// <param name="width">Width of the ellipse</param>
		/// <param name="height">Height of the ellipse</param>
		/// <param name="color">Drawing color</param>
		System::Void DrawEllipse(float x, float y, float width, float height, Drawing::Color color);
		/// <summary>
		/// Draws an ellipse specified by center coordinates, a width, and a height.
		/// </summary>
		/// <param name="pt">Location of the center of the ellipse</param>
		/// <param name="sz">Size of the ellipse</param>
		/// <param name="color">Drawing color</param>
		System::Void DrawEllipse(Drawing::PointF pt, Drawing::SizeF sz, Drawing::Color color)
		{
			DrawEllipse(pt.X, pt.Y, sz.Width, sz.Height, color);
		}
		/// <summary>
		/// Draws a polygon specified by the given point coordinates.
		/// </summary>
		/// <param name="points">An array of corner points</param>
		/// <param name="color">Drawing color</param>
		System::Void DrawPolygon(array<Drawing::PointF, 1>^ points, Drawing::Color color);
		/// <summary>
		/// Fills an elliptic pie specified by center coordinates, a width, and a height.
		/// </summary>
		/// <param name="x">X coordinate of the center of the pie</param>
		/// <param name="y">Y coordinate of the center of the pie</param>
		/// <param name="width">Width of the ellipse</param>
		/// <param name="height">Height of the ellipse</param>
		/// <param name="startAngle">Start angle in radians measured counter-clockwise from the x-axis</param>
		/// <param name="sweepAngle">Sweep angle in radians measured counter-clockwise</param>
		/// <param name="color">Drawing color</param>
		System::Void FillPie(float x, float y, float width, float height, float startAngle, float sweepAngle, Drawing::Color color);
		/// <summary>
		/// Fills an elliptic pie specified by center coordinates and size.
		/// </summary>
		/// <param name="pt">Location of the center of the pie</param>
		/// <param name="sz">Size of the ellipse</param>
		/// <param name="startAngle">Start angle in radians measured counter-clockwise from the x-axis</param>
		/// <param name="sweepAngle">Sweep angle in radians measured counter-clockwise</param>
		/// <param name="color">Drawing color</param>
		System::Void FillPie(Drawing::PointF pt, Drawing::SizeF sz, float startAngle, float sweepAngle, Drawing::Color color)
		{
			FillPie(pt.X, pt.Y, sz.Width, sz.Height, startAngle, sweepAngle, color);
		}
		/// <summary>
		/// Fills a triangle specified by the given corner points.
		/// </summary>
		/// <param name="x1">X coordinate of first corner point</param>
		/// <param name="y1">Y coordinate of first corner point</param>
		/// <param name="x2">X coordinate of second corner point</param>
		/// <param name="y2">Y coordinate of second corner point</param>
		/// <param name="x3">X coordinate of third corner point</param>
		/// <param name="y3">Y coordinate of third corner point</param>
		/// <param name="color">Drawing color</param>
		System::Void FillTriangle(float x1, float y1, float x2, float y2,float x3,float y3, Drawing::Color color);
		/// <summary>
		/// Fills a triangle specified by the given corner points.
		/// </summary>
		/// <param name="pt1">Location of first corner point</param>
		/// <param name="pt2">Location of second corner point</param>
		/// <param name="pt3">Location of third corner point</param>
		/// <param name="color">Drawing color</param>
		System::Void FillTriangle(Drawing::PointF pt1, Drawing::PointF pt2, Drawing::PointF pt3, Drawing::Color color)
		{
			FillTriangle(pt1.X, pt1.Y, pt2.X, pt2.Y, pt3.X, pt3.Y, color);
		}
		/// <summary>
		/// Fills a rectangle specified by the given corner points.
		/// </summary>
		/// <param name="x1">X coordinate of first corner point</param>
		/// <param name="y1">Y coordinate of first corner point</param>
		/// <param name="x2">X coordinate of second corner point</param>
		/// <param name="y2">Y coordinate of second corner point</param>
		/// <param name="color">Drawing color</param>
		System::Void FillRectangle(float x1, float y1, float x2, float y2, Drawing::Color color);
		/// <summary>
		/// Fills a rectangle specified by the given corner points.
		/// </summary>
		/// <param name="rec">Rectangle to draw</param>
		/// <param name="color">Drawing color</param>
		System::Void FillRectangle(Drawing::RectangleF rec, Drawing::Color color)
		{
			FillRectangle(rec.Left, rec.Bottom, rec.Right, rec.Top, color);
		}
		/// <summary>
		/// Fills a rounded rectangle specified by the given corner points and corner radii.
		/// </summary>
		/// <param name="x1">X coordinate of first corner point</param>
		/// <param name="y1">Y coordinate of first corner point</param>
		/// <param name="x2">X coordinate of second corner point</param>
		/// <param name="y2">Y coordinate of second corner point</param>
		/// <param name="rx">X fillet radius</param>
		/// <param name="ry">Y fillet radius</param>
		/// <param name="color">Drawing color</param>
		System::Void FillRoundedRectangle(float x1, float y1, float x2, float y2, float rx, float ry, Drawing::Color color);
		/// <summary>
		/// Fills a rounded rectangle specified by the given corner points and corner radius.
		/// </summary>
		/// <param name="x1">X coordinate of first corner point</param>
		/// <param name="y1">Y coordinate of first corner point</param>
		/// <param name="x2">X coordinate of second corner point</param>
		/// <param name="y2">Y coordinate of second corner point</param>
		/// <param name="r">Fillet radius</param>
		/// <param name="color">Drawing color</param>
		System::Void FillRoundedRectangle(float x1, float y1, float x2, float y2, float r, Drawing::Color color)
		{
			FillRoundedRectangle(x1, y1, x2, y2, r, r, color);
		}
		/// <summary>
		/// Fills a rounded rectangle.
		/// </summary>
		/// <param name="rec">Rectangle to draw</param>
		/// <param name="rx">X fillet radius</param>
		/// <param name="ry">Y fillet radius</param>
		/// <param name="color">Drawing color</param>
		System::Void FillRoundedRectangle(Drawing::RectangleF rec, float rx, float ry, Drawing::Color color)
		{
			FillRoundedRectangle(rec.Left, rec.Bottom, rec.Right, rec.Top, rx, ry, color);
		}
		/// <summary>
		/// Fills a rounded rectangle.
		/// </summary>
		/// <param name="rec">Rectangle to draw</param>
		/// <param name="r">Fillet radius</param>
		/// <param name="color">Drawing color</param>
		System::Void FillRoundedRectangle(Drawing::RectangleF rec, float r, Drawing::Color color)
		{
			FillRoundedRectangle(rec, r, r, color);
		}
		/// <summary>
		/// Fills an ellipse specified by center coordinates, a width, and a height.
		/// </summary>
		/// <param name="x">X coordinate of the center of the arc</param>
		/// <param name="y">Y coordinate of the center of the arc</param>
		/// <param name="width">Width of the ellipse</param>
		/// <param name="height">Height of the ellipse</param>
		/// <param name="color">Drawing color</param>
		System::Void FillEllipse(float x, float y, float width, float height, Drawing::Color color);
		/// <summary>
		/// Fills an ellipse specified by center coordinates, a width, and a height.
		/// </summary>
		/// <param name="pt">Location of the center of the ellipse</param>
		/// <param name="sz">Size of the ellipse</param>
		/// <param name="color">Drawing color</param>
		System::Void FillEllipse(Drawing::PointF pt, Drawing::SizeF sz, Drawing::Color color)
		{
			FillEllipse(pt.X, pt.Y, sz.Width, sz.Height, color);
		}
		/// <summary>
		/// Fills a polygon specified by the given point coordinates.
		/// </summary>
		/// <param name="points">An array of corner points</param>
		/// <param name="color">Drawing color</param>
		System::Void FillPolygon(array<Drawing::PointF, 1> ^ points, Drawing::Color color);
		/// <summary>
		/// Measures the given string.
		/// </summary>
		/// <param name="text">The text to measure</param>
		/// <returns>Size of text bounds</returns>
		Drawing::SizeF MeasureString(System::String ^ text);
	};

}