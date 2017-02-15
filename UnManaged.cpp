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

#include "stdafx.h"

#include <windows.h>
#include <GL/glu.h>
#include "UnManaged.h"

#pragma unmanaged
void _DrawCylinder(float radius, float length, bool wire, int slices, int stacks)
{
	GLUquadric* q = gluNewQuadric();
	if(wire)
		gluQuadricDrawStyle(q, GLU_LINE); // GLU_SILHOUETTE
	gluCylinder(q, radius, radius, length, slices, stacks);
	gluDeleteQuadric(q);
}

void _DrawSphere(float radius, bool wire, int slices, int stacks)
{
	GLUquadric* q = gluNewQuadric();
	if(wire)
		gluQuadricDrawStyle(q, GLU_LINE); // GLU_SILHOUETTE
	gluSphere(q, radius, slices, stacks);
	gluDeleteQuadric(q);
}
#pragma managed