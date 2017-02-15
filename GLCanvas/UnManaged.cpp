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