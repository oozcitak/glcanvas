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

using namespace System;

namespace GLView 
{
	// Forward class declarations
	ref class GLCanvas3D;
	value class Point3D;

	/// <summary>
	/// Contains methods for drawing on the canvas.
	/// </summary>
	public ref class GLGraphics3D
	{
	// Constructor/destructor
	internal:
		GLGraphics3D(GLCanvas3D ^ Canvas, Drawing::Graphics ^ GDIGraphics);

	protected:
		~GLGraphics3D() // Dispose
		{ 
		}

	// Member variables
	private:
		float mLineWidth;
		Drawing::RectangleF mView;
		System::Drawing::Graphics ^ mGDIGraphics;
		float xmin, xmax, ymin, ymax, zmin, zmax;
		GLCanvas3D ^ mCanvas;

	// Helper methods
	private:
		/// <summary>
		/// Updates model limits.
		/// </summary>
		/// <param name="x">X coordinate</param>
		/// <param name="y">Y coordinate</param>
		/// <param name="z">Z coordinate</param>
		System::Void UpdateLimits(float x, float y, float z)
		{
			xmin = Math::Min(xmin, x);
			xmax = Math::Max(xmax, x);
			ymin = Math::Min(ymin, y);
			ymax = Math::Max(ymax, y);
			zmin = Math::Min(zmin, z);
			zmax = Math::Max(zmax, z);
		}
		/// <summary>
		/// Sets the raster position to given window coordinates.
		/// </summary>
		/// <param name="x">X coordinate</param>
		/// <param name="y">Y coordinate</param>
		System::Void glWindowPos2f(GLfloat x, GLfloat y);

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

	public:
		/// <summary>
		/// Gets the origion of the model.
		/// </summary>
		/// <returns>Location of the model origin.</returns>
		Point3D ModelOrigin();
		/// <summary>
		/// Gets the size of the model.
		/// </summary>
		/// <returns>Size of the model.</returns>
		float ModelSize();
		/// <summary>
		/// Draws a line connecting the given points.
		/// </summary>
		/// <param name="x1">X coordinate of first point</param>
		/// <param name="y1">Y coordinate of first point</param>
		/// <param name="z1">Z coordinate of first point</param>
		/// <param name="x2">X coordinate of second point</param>
		/// <param name="y2">Y coordinate of second point</param>
		/// <param name="z2">Z coordinate of second point</param>
		/// <param name="color">Drawing color</param>
		System::Void DrawLine(float x1, float y1, float z1, float x2, float y2, float z2, Drawing::Color color);
		/// <summary>
		/// Draws a triangle specified by the given corner points.
		/// </summary>
		/// <param name="x1">X coordinate of first point</param>
		/// <param name="y1">Y coordinate of first point</param>
		/// <param name="z1">Z coordinate of first point</param>
		/// <param name="x2">X coordinate of second point</param>
		/// <param name="y2">Y coordinate of second point</param>
		/// <param name="z2">Z coordinate of second point</param>
		/// <param name="x3">X coordinate of third point</param>
		/// <param name="y3">Y coordinate of third point</param>
		/// <param name="z3">Z coordinate of third point</param>
		/// <param name="color">Drawing color</param>
		System::Void DrawTriangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, Drawing::Color color);
		/// <summary>
		/// Draws a quad specified by the given corner points.
		/// </summary>
		/// <param name="x1">X coordinate of first point</param>
		/// <param name="y1">Y coordinate of first point</param>
		/// <param name="z1">Z coordinate of first point</param>
		/// <param name="x2">X coordinate of second point</param>
		/// <param name="y2">Y coordinate of second point</param>
		/// <param name="z2">Z coordinate of second point</param>
		/// <param name="x3">X coordinate of third point</param>
		/// <param name="y3">Y coordinate of third point</param>
		/// <param name="z3">Z coordinate of third point</param>
		/// <param name="x4">X coordinate of fourth point</param>
		/// <param name="y4">Y coordinate of fourth point</param>
		/// <param name="z4">Z coordinate of fourth point</param>
		/// <param name="color">Drawing color</param>
		System::Void DrawQuad(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, float x4, float y4, float z4, Drawing::Color color);
		/// <summary>
		/// Draws a filled triangle specified by the given corner points.
		/// </summary>
		/// <param name="x1">X coordinate of first point</param>
		/// <param name="y1">Y coordinate of first point</param>
		/// <param name="z1">Z coordinate of first point</param>
		/// <param name="x2">X coordinate of second point</param>
		/// <param name="y2">Y coordinate of second point</param>
		/// <param name="z2">Z coordinate of second point</param>
		/// <param name="x3">X coordinate of third point</param>
		/// <param name="y3">Y coordinate of third point</param>
		/// <param name="z3">Z coordinate of third point</param>
		/// <param name="color">Drawing color</param>
		System::Void FillTriangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, Drawing::Color color);
		/// <summary>
		/// Draws a filled quad specified by the given corner points.
		/// </summary>
		/// <param name="x1">X coordinate of first point</param>
		/// <param name="y1">Y coordinate of first point</param>
		/// <param name="z1">Z coordinate of first point</param>
		/// <param name="x2">X coordinate of second point</param>
		/// <param name="y2">Y coordinate of second point</param>
		/// <param name="z2">Z coordinate of second point</param>
		/// <param name="x3">X coordinate of third point</param>
		/// <param name="y3">Y coordinate of third point</param>
		/// <param name="z3">Z coordinate of third point</param>
		/// <param name="x4">X coordinate of fourth point</param>
		/// <param name="y4">Y coordinate of fourth point</param>
		/// <param name="z4">Z coordinate of fourth point</param>
		/// <param name="color">Drawing color</param>
		System::Void FillQuad(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, float x4, float y4, float z4, Drawing::Color color);
		/// <summary>
		/// Draws a box.
		/// </summary>
		/// <param name="x1">X coordinate of first point</param>
		/// <param name="y1">Y coordinate of first point</param>
		/// <param name="z1">Z coordinate of first point</param>
		/// <param name="x2">X coordinate of second point</param>
		/// <param name="y2">Y coordinate of second point</param>
		/// <param name="z2">Z coordinate of second point</param>
		/// <param name="width">Width of top and bottom faces</param>
		/// <param name="height">Height of top and bottom faces</param>
		/// <param name="color">Drawing color</param>
		System::Void DrawBox(float x1, float y1, float z1, float x2, float y2, float z2, float width, float height, Drawing::Color color);
		/// <summary>
		/// Draws a filled box.
		/// </summary>
		/// <param name="x1">X coordinate of first point</param>
		/// <param name="y1">Y coordinate of first point</param>
		/// <param name="z1">Z coordinate of first point</param>
		/// <param name="x2">X coordinate of second point</param>
		/// <param name="y2">Y coordinate of second point</param>
		/// <param name="z2">Z coordinate of second point</param>
		/// <param name="width">Width of top and bottom faces</param>
		/// <param name="height">Height of top and bottom faces</param>
		/// <param name="color">Drawing color</param>
		System::Void FillBox(float x1, float y1, float z1, float x2, float y2, float z2, float width, float height, Drawing::Color color);
		/// <summary>
		/// Draws a sphere.
		/// </summary>
		/// <param name="x">X coordinate of center</param>
		/// <param name="y">Y coordinate of center</param>
		/// <param name="z">Z coordinate of center</param>
		/// <param name="radius">Radius of sphere</param>
		/// <param name="slices">Number of subdivisions around the z axis.</param>
		/// <param name="stacks">Number of subdivisions along the z axis.</param>
		/// <param name="color">Drawing color</param>
		System::Void DrawSphere(float x, float y, float z, float radius, int slices, int stacks, Drawing::Color color);
		/// <summary>
		/// Draws a sphere.
		/// </summary>
		/// <param name="x">X coordinate of center</param>
		/// <param name="y">Y coordinate of center</param>
		/// <param name="z">Z coordinate of center</param>
		/// <param name="radius">Radius of sphere</param>
		/// <param name="color">Drawing color</param>
		System::Void DrawSphere(float x, float y, float z, float radius, Drawing::Color color)
		{
			DrawSphere(x, y, z, radius, 16, 16, color);
		}
		/// <summary>
		/// Draws a filled sphere.
		/// </summary>
		/// <param name="x">X coordinate of center</param>
		/// <param name="y">Y coordinate of center</param>
		/// <param name="z">Z coordinate of center</param>
		/// <param name="radius">Radius of sphere</param>
		/// <param name="slices">Number of subdivisions around the z axis.</param>
		/// <param name="stacks">Number of subdivisions along the z axis.</param>
		/// <param name="color">Drawing color</param>
		System::Void FillSphere(float x, float y, float z, float radius, int slices, int stacks, Drawing::Color color);
		/// <summary>
		/// Draws a filles sphere.
		/// </summary>
		/// <param name="x">X coordinate of center</param>
		/// <param name="y">Y coordinate of center</param>
		/// <param name="z">Z coordinate of center</param>
		/// <param name="radius">Radius of sphere</param>
		/// <param name="color">Drawing color</param>
		System::Void FillSphere(float x, float y, float z, float radius, Drawing::Color color)
		{
			FillSphere(x, y, z, radius, 16, 16, color);
		}
		/// <summary>
		/// Draws a cylinder.
		/// </summary>
		/// <param name="x1">X coordinate of first point</param>
		/// <param name="y1">Y coordinate of first point</param>
		/// <param name="z1">Z coordinate of first point</param>
		/// <param name="x2">X coordinate of second point</param>
		/// <param name="y2">Y coordinate of second point</param>
		/// <param name="z2">Z coordinate of second point</param>
		/// <param name="radius">Raidus of top and bottom faces</param>
		/// <param name="slices">Number of subdivisions around the z axis.</param>
		/// <param name="stacks">Number of subdivisions along the z axis.</param>
		/// <param name="color">Drawing color</param>
		System::Void DrawCylinder(float x1, float y1, float z1, float x2, float y2, float z2, float radius, int slices, int stacks, Drawing::Color color);
		/// <summary>
		/// Draws a cylinder.
		/// </summary>
		/// <param name="x1">X coordinate of first point</param>
		/// <param name="y1">Y coordinate of first point</param>
		/// <param name="z1">Z coordinate of first point</param>
		/// <param name="x2">X coordinate of second point</param>
		/// <param name="y2">Y coordinate of second point</param>
		/// <param name="z2">Z coordinate of second point</param>
		/// <param name="radius">Raidus of top and bottom faces</param>
		/// <param name="color">Drawing color</param>
		System::Void DrawCylinder(float x1, float y1, float z1, float x2, float y2, float z2, float radius, Drawing::Color color)
		{
			DrawCylinder(x1, y1, z1, x2, y2, z2, radius, 16, 16, color);
		}
		/// <summary>
		/// Draws a filled cylinder.
		/// </summary>
		/// <param name="x1">X coordinate of first point</param>
		/// <param name="y1">Y coordinate of first point</param>
		/// <param name="z1">Z coordinate of first point</param>
		/// <param name="x2">X coordinate of second point</param>
		/// <param name="y2">Y coordinate of second point</param>
		/// <param name="z2">Z coordinate of second point</param>
		/// <param name="radius">Raidus of top and bottom faces</param>
		/// <param name="slices">Number of subdivisions around the z axis.</param>
		/// <param name="stacks">Number of subdivisions along the z axis.</param>
		/// <param name="color">Drawing color</param>
		System::Void FillCylinder(float x1, float y1, float z1, float x2, float y2, float z2, float radius, int slices, int stacks, Drawing::Color color);
		/// <summary>
		/// Draws a filles cylinder.
		/// </summary>
		/// <param name="x1">X coordinate of first point</param>
		/// <param name="y1">Y coordinate of first point</param>
		/// <param name="z1">Z coordinate of first point</param>
		/// <param name="x2">X coordinate of second point</param>
		/// <param name="y2">Y coordinate of second point</param>
		/// <param name="z2">Z coordinate of second point</param>
		/// <param name="radius">Raidus of top and bottom faces</param>
		/// <param name="color">Drawing color</param>
		System::Void FillCylinder(float x1, float y1, float z1, float x2, float y2, float z2, float radius, Drawing::Color color)
		{
			FillCylinder(x1, y1, z1, x2, y2, z2, radius, 16, 16, color);
		}
		/// <summary>
		/// Draws raster text at the given coordinates.
		/// </summary>
		/// <param name="x">X coordinate</param>
		/// <param name="y">Y coordinate</param>
		/// <param name="z">Z coordinate</param>
		/// <param name="text">Text to draw</param>
		/// <param name="color">Drawing color</param>
		/// <param name="alignment">Horizontal text alignment</param>
		System::Void DrawRasterText(float x, float y, float z, System::String ^ text, Drawing::Color color, Windows::Forms::HorizontalAlignment alignment);
		/// <summary>
		/// Draws raster text at the given coordinates.
		/// </summary>
		/// <param name="x">X coordinate</param>
		/// <param name="y">Y coordinate</param>
		/// <param name="z">Z coordinate</param>
		/// <param name="text">Text to draw</param>
		/// <param name="color">Drawing color</param>
		System::Void DrawRasterText(float x, float y, float z, System::String ^ text, Drawing::Color color)
		{
			DrawRasterText(x, y, z, text, color, Windows::Forms::HorizontalAlignment::Left);
		}
		/// <summary>
		/// Draws text at the given window coordinates.
		/// </summary>
		/// <param name="x">X coordinate</param>
		/// <param name="y">Y coordinate</param>
		/// <param name="text">Text to draw</param>
		/// <param name="color">Drawing color</param>
		System::Void DrawRasterTextWindow(float x, float y, System::String ^ text, Drawing::Color color);
		/// <summary>
		/// Draws vector text at the given coordinates.
		/// </summary>
		/// <param name="x">X coordinate</param>
		/// <param name="y">Y coordinate</param>
		/// <param name="z">Z coordinate</param>
		/// <param name="text">Text to draw</param>
		/// <param name="color">Drawing color</param>
		System::Void DrawVectorText(float x, float y, float z, float height, System::String ^ text, Drawing::Color color);
	};

}
