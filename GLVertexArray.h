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

using namespace System;

namespace GLView {

	/// <summary>
	/// Represents a vertex array.
	/// </summary>
	private ref class GLVertexArray
	{
	private:
		// Custom vertex class
		value class GLColorVertex {
		public:
			float x, y, z;
			float r, g, b, a;
		};

	// Constructor/destructor
	public:
		GLVertexArray(GLenum Type) 
		{ 
			mVertices = gcnew System::Collections::Generic::List<GLColorVertex>;
			mType = Type;
		}

	// Member variables
	private:
		System::Collections::Generic::List<GLColorVertex>^ mVertices;
		GLenum mType;

	// Implementation
	public:
		/// <summary>
		/// Clears all vertices.
		/// </summary>
		System::Void Clear()
		{
			mVertices->Clear();
		}
		/// <summary>
		/// Adds a new vertex to the array.
		/// </summary>
		/// <param name="x">X coordinate</param>
		/// <param name="y">Y coordinate</param>
		/// <param name="z">Z coordinate</param>
		/// <param name="r">Red color component</param>
		/// <param name="g">Green color component</param>
		/// <param name="b">Blue color component</param>
		/// <param name="a">Alpha color component</param>
		System::Void AddVertex(float x, float y, float z, float r, float g, float b, float a)
		{
			GLColorVertex v;
			v.x = x;
			v.y = y;
			v.z = z;
			v.r = r;
			v.g = g;
			v.b = b;
			v.a = a;

			mVertices->Add(v);
		}
		/// <summary>
		/// Adds a new vertex to the array.
		/// </summary>
		/// <param name="x">X coordinate</param>
		/// <param name="y">Y coordinate</param>
		/// <param name="z">Z coordinate</param>
		/// <param name="color">Vertex color</param>
		System::Void AddVertex(float x, float y, float z, Drawing::Color color)
		{
			AddVertex(x, y, z, (float)color.R / 256.0f, (float)color.G / 256.0f, (float)color.B / 256.0f, (float)color.A / 256.0f);
		}

		/// <summary>
		/// Renders the vertex array.
		/// </summary>
		System::Void Render()
		{
			float * vp = new float[mVertices->Count * 3];
			float * cp = new float[mVertices->Count * 4];

			for (int j = 0; j < mVertices->Count; j++)
			{
				vp[j * 3] = mVertices[j].x;
				vp[j * 3 + 1] = mVertices[j].y;
				vp[j * 3 + 2] = mVertices[j].z;
				cp[j * 4] = mVertices[j].r;
				cp[j * 4 + 1] = mVertices[j].g;
				cp[j * 4 + 2] = mVertices[j].b;
				cp[j * 4 + 3] = mVertices[j].a;
			}

			glLoadIdentity();
			glVertexPointer(3, GL_FLOAT, 3 * sizeof(float), vp);
			glColorPointer(4, GL_FLOAT, 4 * sizeof(float), cp);
			glDrawArrays(mType, 0, mVertices->Count);

			delete[] vp;
			vp = 0;
			delete[] cp;
			cp = 0;
		}

	// Properties
	public:
		/// <summary>
		/// Gets the number of vertices.
		/// </summary>
		property int Count
		{
			virtual int get(void) { return mVertices->Count; }
		}
		/// <summary>
		/// Gets the vertex at the given index.
		/// </summary>
		/// <param name="index">Item index</param>
		property GLColorVertex Vertex[int]
		{
			virtual GLColorVertex get(int index) { return mVertices[index]; }
		}

	};

}
