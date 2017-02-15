#include "stdafx.h"

#include <Vcclr.h>
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "GLGraphics3D.h"
#include "GLCanvas3D.h"
#include "UnManaged.h"
#include "Point3D.h"
#include "Utility.h"
#include "GLPickBox.h"

namespace GLCanvas
{
	GLGraphics3D::GLGraphics3D(GLCanvas3D ^ Canvas, Drawing::Graphics ^ GDIGraphics)
	{
		mCanvas = Canvas; 
		mGDIGraphics = GDIGraphics;
		LineWidth = 1.0f;
	}

	Point3D GLGraphics3D::ModelOrigin()
	{
		return Point3D((xmin + xmax) / 2.0f, (ymin + ymax) / 2.0f, (zmin + zmax) / 2.0f);
	}

	float GLGraphics3D::ModelSize()
	{
		return (float)Math::Max(Math::Sqrt((xmin - xmax) * (xmin - xmax) + (ymin - ymax) * (ymin - ymax) + (zmin - zmax) * (zmin - zmax)), 1.0);
	}

	System::Void GLGraphics3D::DrawLine(float x1, float y1, float z1, float x2, float y2, float z2, Drawing::Color color)
	{
		glColor4ub(color.R, color.G, color.B, color.A);
		glBegin(GL_LINES);

		glVertex3f(x1, y1, z1);
		glVertex3f(x2, y2, z2);

		glEnd();

		UpdateLimits(x1, y1, z1);
		UpdateLimits(x2, y2, z2);
	}

	System::Void GLGraphics3D::DrawTriangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, Drawing::Color color)
	{
		glColor4ub(color.R, color.G, color.B, color.A);
		glBegin(GL_LINES);

		glVertex3f(x1, y1, z1);
		glVertex3f(x2, y2, z2);

		glVertex3f(x2, y2, z2);
		glVertex3f(x3, y3, z3);

		glVertex3f(x3, y3, z3);
		glVertex3f(x1, y1, z1);

		glEnd();

		UpdateLimits(x1, y1, z1);
		UpdateLimits(x2, y2, z2);
		UpdateLimits(x3, y3, z3);
	}

	System::Void GLGraphics3D::DrawQuad(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, float x4, float y4, float z4, Drawing::Color color)
	{ 
		glColor4ub(color.R, color.G, color.B, color.A);
		glBegin(GL_LINES);

		glVertex3f(x1, y1, z1);
		glVertex3f(x2, y2, z2);

		glVertex3f(x2, y2, z2);
		glVertex3f(x3, y3, z3);

		glVertex3f(x3, y3, z3);
		glVertex3f(x4, y4, z4);

		glVertex3f(x4, y4, z4);
		glVertex3f(x1, y1, z1);

		glEnd();

		UpdateLimits(x1, y1, z1);
		UpdateLimits(x2, y2, z2);
		UpdateLimits(x3, y3, z3);
		UpdateLimits(x4, y4, z4);
	}

	System::Void GLGraphics3D::FillTriangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, Drawing::Color color)
	{
		glColor4ub(color.R, color.G, color.B, color.A);
		glBegin(GL_TRIANGLES);

		float n[3];
		Utility::CrossProduct(x1 - x2, y1 - y2, z1 - z2, x3 - x2, y3 - y2, z3 - z2, n);
		glNormal3f(n[0], n[1], n[2]);

		glVertex3f(x1, y1, z1);
		glVertex3f(x2, y2, z2);
		glVertex3f(x3, y3, z3);

		glEnd();

		UpdateLimits(x1, y1, z1);
		UpdateLimits(x2, y2, z2);
		UpdateLimits(x3, y3, z3);
	}

	System::Void GLGraphics3D::FillQuad(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, float x4, float y4, float z4, Drawing::Color color)
	{
		glColor4ub(color.R, color.G, color.B, color.A);
		glBegin(GL_QUADS);

		float n[3];
		Utility::CrossProduct(x1 - x2, y1 - y2, z1 - z2, x3 - x2, y3 - y2, z3 - z2, n);
		glNormal3f(n[0], n[1], n[2]);

		glVertex3f(x1, y1, z1);
		glVertex3f(x2, y2, z2);
		glVertex3f(x3, y3, z3);
		glVertex3f(x4, y4, z4);

		glEnd();

		UpdateLimits(x1, y1, z1);
		UpdateLimits(x2, y2, z2);
		UpdateLimits(x3, y3, z3);
		UpdateLimits(x4, y4, z4);
	}

	System::Void GLGraphics3D::DrawBox(float x1, float y1, float z1, float x2, float y2, float z2, float width, float height, Drawing::Color color)
	{
		float len = (float)Math::Sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) + (z1 - z2) * (z1 - z2));
		float zrot = (float)(Math::Atan2(y2 - y1, x2 - x1) * 180.0 / Math::PI);
		float yrot = (float)(Math::Atan2(Math::Sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)), z2 - z1) * 180.0 / Math::PI);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glTranslatef(x1, y1, z1);
		glRotatef(zrot, 0, 0, 1);
		glRotatef(yrot, 0, 1, 0);

		glColor4ub(color.R, color.G, color.B, color.A);
		glBegin(GL_LINES);

		glVertex3f(-width / 2, -height / 2, 0);
		glVertex3f(-width / 2, -height / 2, len);
		glVertex3f(width / 2, -height / 2, 0);
		glVertex3f(width / 2, -height / 2, len);
		glVertex3f(-width / 2, height / 2, 0);
		glVertex3f(-width / 2, height / 2, len);
		glVertex3f(width / 2, height / 2, 0);
		glVertex3f(width / 2, height / 2, len);

		glEnd();
		
		// Bottom
		glBegin(GL_LINE_LOOP);
		glVertex3f(-width / 2, -height / 2, 0);
		glVertex3f(-width / 2, height / 2, 0);
		glVertex3f(width / 2, height / 2, 0);
		glVertex3f(width / 2, -height / 2, 0);
		glEnd();

		// Top
		glBegin(GL_LINE_LOOP);
		glVertex3f(-width / 2, -height / 2, len);
		glVertex3f(-width / 2, height / 2, len);
		glVertex3f(width / 2, height / 2, len);
		glVertex3f(width / 2, -height / 2, len);
		glEnd();

		glPopMatrix();

		UpdateLimits(x1, y1, z1);
		UpdateLimits(x2, y2, z2);
	}

	System::Void GLGraphics3D::FillBox(float x1, float y1, float z1, float x2, float y2, float z2, float width, float height, Drawing::Color color)
	{
		float len = (float)Math::Sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) + (z1 - z2) * (z1 - z2));
		float zrot = (float)(Math::Atan2(y2 - y1, x2 - x1) * 180.0 / Math::PI);
		float yrot = (float)(Math::Atan2(Math::Sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)), z2 - z1) * 180.0 / Math::PI);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glTranslatef(x1, y1, z1);
		glRotatef(zrot, 0, 0, 1);
		glRotatef(yrot, 0, 1, 0);

		glColor4ub(color.R, color.G, color.B, color.A);
		glBegin(GL_QUADS);

		// Front
		glNormal3f(0, -1, 0);
		glVertex3f(-width / 2, -height / 2, 0);
		glVertex3f(-width / 2, -height / 2, len);
		glVertex3f(width / 2, -height / 2, len);
		glVertex3f(width / 2, -height / 2, 0);

		// Right
		glNormal3f(1, 0, 0);
		glVertex3f(width / 2, -height / 2, 0);
		glVertex3f(width / 2, -height / 2, len);
		glVertex3f(width / 2, height / 2, len);
		glVertex3f(width / 2, height / 2, 0);

		// Back
		glNormal3f(0, 1, 0);
		glVertex3f(width / 2, height / 2, 0);
		glVertex3f(width / 2, height / 2, len);
		glVertex3f(-width / 2, height / 2, len);
		glVertex3f(-width / 2, height / 2, 0);

		// Left
		glNormal3f(-1, 0, 0);
		glVertex3f(-width / 2, height / 2, 0);
		glVertex3f(-width / 2, height / 2, len);
		glVertex3f(-width / 2, -height / 2, len);
		glVertex3f(-width / 2, -height / 2, 0);

		// Bottom
		glNormal3f(0, 0, -1);
		glVertex3f(-width / 2, -height / 2, 0);
		glVertex3f(-width / 2, height / 2, 0);
		glVertex3f(width / 2, height / 2, 0);
		glVertex3f(width / 2, -height / 2, 0);

		// Top
		glNormal3f(0, 0, 1);
		glVertex3f(-width / 2, -height / 2, len);
		glVertex3f(-width / 2, height / 2, len);
		glVertex3f(width / 2, height / 2, len);
		glVertex3f(width / 2, -height / 2, len);

		glEnd();
		
		glPopMatrix();

		UpdateLimits(x1, y1, z1);
		UpdateLimits(x2, y2, z2);
	}

	System::Void GLGraphics3D::DrawCylinder(float x1, float y1, float z1, float x2, float y2, float z2, float radius, int slices, int stacks, Drawing::Color color)
	{
		float len = (float)Math::Sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) + (z1 - z2) * (z1 - z2));
		float zrot = (float)(Math::Atan2(y2 - y1, x2 - x1) * 180.0 / Math::PI);
		float yrot = (float)(Math::Atan2(Math::Sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)), z2 - z1) * 180.0 / Math::PI);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glTranslatef(x1, y1, z1);
		glRotatef(zrot, 0, 0, 1);
		glRotatef(yrot, 0, 1, 0);

		glColor4ub(color.R, color.G, color.B, color.A);

		_DrawCylinder(radius, len, true, slices, stacks);

		glPopMatrix();

		UpdateLimits(x1, y1, z1);
		UpdateLimits(x2, y2, z2);
	}

	System::Void GLGraphics3D::FillCylinder(float x1, float y1, float z1, float x2, float y2, float z2, float radius, int slices, int stacks, Drawing::Color color)
	{
		float len = (float)Math::Sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) + (z1 - z2) * (z1 - z2));
		float zrot = (float)(Math::Atan2(y2 - y1, x2 - x1) * 180.0 / Math::PI);
		float yrot = (float)(Math::Atan2(Math::Sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)), z2 - z1) * 180.0 / Math::PI);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glTranslatef(x1, y1, z1);
		glRotatef(zrot, 0, 0, 1);
		glRotatef(yrot, 0, 1, 0);

		glColor4ub(color.R, color.G, color.B, color.A);
		_DrawCylinder(radius, len, false, slices, stacks);

		glPopMatrix();

		UpdateLimits(x1, y1, z1);
		UpdateLimits(x2, y2, z2);
	}

	System::Void GLGraphics3D::DrawSphere(float x, float y, float z,float radius, int slices, int stacks, Drawing::Color color)
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glTranslatef(x, y, z);

		glColor4ub(color.R, color.G, color.B, color.A);
		_DrawSphere(radius, true, slices, stacks);

		glPopMatrix();

		UpdateLimits(x - radius, y - radius, z - radius);
		UpdateLimits(x + radius, y + radius, z + radius);
	}

	System::Void GLGraphics3D::FillSphere(float x, float y, float z,float radius, int slices, int stacks, Drawing::Color color)
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glTranslatef(x, y, z);

		glColor4ub(color.R, color.G, color.B, color.A);
		_DrawSphere(radius, false, slices, stacks);

		glPopMatrix();

		UpdateLimits(x - radius, y - radius, z - radius);
		UpdateLimits(x + radius, y + radius, z + radius);
	}

	System::Void GLGraphics3D::DrawRasterText(float x, float y, float z, System::String ^ text, Drawing::Color color, Windows::Forms::HorizontalAlignment alignment)
	{
		glColor4ub(color.R, color.G, color.B, color.A);
		glListBase(mCanvas->RasterListBase);
		// Measure the text
		if(alignment != Windows::Forms::HorizontalAlignment::Left)
		{
			float length = 0.0F;
			for (int i = 0; i < text->Length; i++)
				length += mCanvas->charWidths[text[i]];
			if(alignment == Windows::Forms::HorizontalAlignment::Center)
				length /= 2.0F;
			x -= length;
		}
		glRasterPos3f(x, y, z);
		// Draw the text
		System::IntPtr str = System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(text);
		glCallLists(text->Length, GL_UNSIGNED_BYTE, (GLvoid*)str.ToPointer());
		System::Runtime::InteropServices::Marshal::FreeHGlobal(str);
	}

	System::Void GLGraphics3D::DrawRasterTextWindow(float x, float y, System::String ^ text, Drawing::Color color)
	{
		glColor4ub(color.R, color.G, color.B, color.A);
		glListBase(mCanvas->RasterListBase);
		this->glWindowPos2f(x, y);
		// Draw the text
		System::IntPtr str = System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(text);
		glCallLists(text->Length, GL_UNSIGNED_BYTE, (GLvoid*)str.ToPointer());
		System::Runtime::InteropServices::Marshal::FreeHGlobal(str);
	}

	System::Void GLGraphics3D::DrawVectorText(float x, float y, float z, float height, System::String ^ text, Drawing::Color color)
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glColor4ub(color.R, color.G, color.B, color.A);
		glListBase(mCanvas->VectorListBase);
		glTranslatef(x, y, z);
		glScalef(height, height, height);
		// Draw the text
		System::IntPtr str = System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(text);
		glCallLists(text->Length, GL_UNSIGNED_BYTE, (GLvoid*)str.ToPointer());
		System::Runtime::InteropServices::Marshal::FreeHGlobal(str);
		glPopMatrix();
	}

	System::Void GLGraphics3D::glWindowPos2f(GLfloat x, GLfloat y)
	{
		GLfloat z = 0;
		GLfloat w = 1;
		GLfloat fx, fy;

		// Push current matrix mode and viewport attributes
		glPushAttrib(GL_TRANSFORM_BIT | GL_VIEWPORT_BIT);

		// Setup projection parameters
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		glDepthRange(z, z);
		glViewport((int)x - 1, (int)y - 1, 2, 2);

		// Set the raster (window) position
		fx = x - (int)x;
		fy = y - (int)y;
		glRasterPos4f(fx, fy, 0.0, w);

	    // Restore matrices, viewport and matrix mode
  	    glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
	    glPopAttrib();
	}
}
