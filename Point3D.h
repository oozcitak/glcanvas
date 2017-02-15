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
	/// <summary>
	/// Represents a 3D location.
	/// </summary>
	public value class Point3D
	{
	// Members
	public:
		/// <summary>
		/// X coordinate
		/// </summary>
	    float X;
		/// <summary>
		/// Y coordinate
		/// </summary>
		float Y;
		/// <summary>
		/// Z coordinate
		/// </summary>
		float Z;

	// Properties
	public:
		/// <summary>
		/// Gets the length of the vector represented by this instance.
		/// </summary>
		property float Length
		{
			virtual float get(void) 
			{
				return (float)Math::Sqrt(X * X + Y * Y + Z * Z);
			}
		}

	// Constructor
	public:
		/// <summary>
		/// Initializes a new instance of the Point3D class.
		/// </summary>
		/// <param name="x">X coordinate</param>
		/// <param name="y">Y coordinate</param>
		/// <param name="z">Z coordinate</param>
		Point3D(float x, float y, float z)
		{
			X = x;
			Y = y;
			Z = z;
		}

	// Instance Methods
	public:
		/// <summary>
		/// Returns the string representation of this object.
		/// </summary>
		virtual System::String^ ToString() override
		{
			return String::Format("{0:0.00}, {1:0.00}, {2:0.00}", X, Y, Z);
		}

	//Operators
	public:
		/// <summary>
		/// Returns the vector addition of the given vectors.
		/// </summary>
		static Point3D operator+(Point3D a, Point3D b)
		{
			return Point3D(a.X + b.X, a.Y + b.Y, a.Z + b.Z);
		}
		/// <summary>
		/// Returns the vector difference between the given vectors.
		/// </summary>
		static Point3D operator-(Point3D a, Point3D b)
		{
			return Point3D(a.X - b.X, a.Y - b.Y, a.Z - b.Z);
		}
		/// <summary>
		/// Returns the cross product of the given vectors.
		/// </summary>
		static Point3D operator*(Point3D a, Point3D b)
		{
			return Point3D(a.Y * b.Z - a.Z * b.Y,
				a.Z * b.X - a.X * b.Z,
				a.X * b.Y - a.Y * b.X);
		}
		/// <summary>
		/// Returns the multiplication of the given vector with the given scalar value.
		/// </summary>
		static Point3D operator*(Point3D a, float b)
		{
			return Point3D(a.X * b, a.Y * b, a.Z * b);
		}
		/// <summary>
		/// Returns the multiplication of the given vector with the given scalar value.
		/// </summary>
		static Point3D operator*(float b, Point3D a)
		{
			return Point3D(a.X * b, a.Y * b, a.Z * b);
		}
		/// <summary>
		/// Returns the division of the given vector by the given scalar value.
		/// </summary>
		static Point3D operator/(Point3D a, float b)
		{
			return Point3D(a.X / b, a.Y / b, a.Z / b);
		}
	};
}