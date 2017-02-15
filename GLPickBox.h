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
namespace GLView 
{
	/// <summary>
	/// Represents a box shaped selection object.
	/// </summary>
	private value class GLPickBox
	{
	// Member variables
	public:
		/// <summary>
		/// Object ID.
		/// </summary>
		System::Object^ ID;

		/// <summary>
		/// X coordinate of the start point of the box.
		/// </summary>
		float X1;
		/// <summary>
		/// Y coordinate of the start point of the box.
		/// </summary>
		float Y1;
		/// <summary>
		/// Z coordinate of the start point of the box.
		/// </summary>
		float Z1;
		
		/// <summary>
		/// X coordinate of the end point of the box.
		/// </summary>
		float X2;
		/// <summary>
		/// Y coordinate of the end point of the box.
		/// </summary>
		float Y2;
		/// <summary>
		/// Z coordinate of the end point of the box.
		/// </summary>
		float Z2;

		/// <summary>
		/// Width of the box.
		/// </summary>
		float Width;
		/// <summary>
		/// Height of the box.
		/// </summary>
		float Height;

	// Constructor/destructor
	public:
		/// <summary>
		/// Initializes a new instance of the Point3D class.
		/// </summary>
		/// <param name="id">Object ID</param>
		/// <param name="x1">X coordinate of the start point of the box.</param>
		/// <param name="y1">Z coordinate of the start point of the box.</param>
		/// <param name="z1">Z coordinate of the start point of the box.</param>
		/// <param name="x2">X coordinate of the end point of the box.</param>
		/// <param name="y2">Z coordinate of the end point of the box.</param>
		/// <param name="z2">Z coordinate of the end point of the box.</param>
		/// <param name="width">Width of the box.</param>
		/// <param name="height">Height of the box.</param>
		GLPickBox(System::Object^ id, float x1, float y1, float z1, float x2, float y2, float z2, float width, float height)
		{
			ID = id;

			X1 = x1; Y1 = y1; Z1 = z1;
			X2 = x2; Y2 = y2; Z2 = z2;

			Width = width;
			Height = height;
		}
	};
}