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

#include "StdAfx.h"
#include "GLCanvas3D.h"
#include "GLGraphics3D.h"
#include "GLPickBox.h"
#include "EventArgs.h"
#include "Utility.h"
#include "Camera.h"

#pragma warning(disable:4100)

namespace GLView {

	GLCanvas3D::GLCanvas3D()
	{
		// Event handlers
		if(!this->DesignMode)
		{
			this->Resize += gcnew System::EventHandler(this, &GLCanvas3D::ControlResize);
			this->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &GLCanvas3D::ControlMouseDown);
			this->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &GLCanvas3D::ControlMouseMove);
			this->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &GLCanvas3D::ControlMouseUp);
			this->MouseWheel += gcnew System::Windows::Forms::MouseEventHandler(this, &GLCanvas3D::ControlMouseWheel);
			this->MouseDoubleClick += gcnew System::Windows::Forms::MouseEventHandler(this, &GLCanvas3D::ControlMouseDoubleClick);
		}

		// Set control styles
		this->SuspendLayout();
		this->Name = L"GLCanvas3D";
		this->Size = System::Drawing::Size(300, 300);
		this->SetStyle(ControlStyles::DoubleBuffer, false);
		this->SetStyle(ControlStyles::AllPaintingInWmPaint | ControlStyles::UserPaint | ControlStyles::Opaque |
			ControlStyles::Selectable | ControlStyles::UserMouse, true);
		this->Cursor = Windows::Forms::Cursors::Cross;
		this->ResumeLayout(false);

		// Set property defaults
		Perspective = true;
		PanWithControlKey = true;
		AllowZoomAndPan = true;
		AllowRotate = true;
		BackColor = Drawing::Color::White;
		mBorderStyle = Windows::Forms::BorderStyle::Fixed3D;
		mPanning = false;
		mCamera = gcnew GLView::Camera(Point3D(-5, -5, 5), Point3D(0, 0, 0));
		DrawFloor = true;
		mFloorColor = Drawing::Color::Beige;
		mGridColor = Drawing::Color::Wheat;
		ShowAxis = true;
		mAntiAlias = true;
		SelectionMode = SelectMode::None;
		SelectionColor = Drawing::SystemColors::Highlight;
		ReverseSelectionColor = Drawing::SystemColors::Highlight;
		mSelecting = false;

		mOrigin = Point3D(0, 0, 0);
		mSize = 1.0f;

		if(!this->DesignMode)
		{
			// Get the device context
			mhDC = GetDC((HWND)this->Handle.ToPointer());

			// Choose a pixel format
			PIXELFORMATDESCRIPTOR pfd = {
				sizeof(PIXELFORMATDESCRIPTOR),	// size
				1,								// version
				PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,	// flags
				PFD_TYPE_RGBA,					// pixel type
				32,								// color bits
				0, 0, 0, 0, 0, 0, 0, 0,			// RGBA bits and shifts
				0,								// accumulation buffer bits
				0, 0, 0, 0,						// accumulation buffer RGBA bits
				32,								// depth bits
				24,								// stencil bits
				0,								// aux bits
				PFD_MAIN_PLANE,					// layer type
				0,								// reserved
				0, 0, 0							// layer masks
			};

			// Set the format
			int iPixelFormat = ChoosePixelFormat(mhDC, &pfd);
			SetPixelFormat(mhDC, iPixelFormat, &pfd);
			mIsAccelerated = !(pfd.dwFlags & PFD_GENERIC_FORMAT);

			// Create the render context
			mhGLRC = wglCreateContext(mhDC);
			wglMakeCurrent(mhDC, mhGLRC);

			// Set the viewport
			glViewport(0, 0, 300, 300);

			// Set OpenGL parameters
			glDisable(GL_LIGHTING);
			glShadeModel(GL_SMOOTH);
			glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_DEPTH_TEST);
			glDepthMask(GL_TRUE);
			glDepthFunc(GL_LEQUAL);
			glEnable(GL_POLYGON_OFFSET_FILL);
			glPolygonOffset(0.0f, 0.5f);
			glEnable(GL_LINE_SMOOTH);

			// Enable lighting
			mLighting = true;
			glEnable(GL_LIGHTING);
			glEnable(GL_LIGHT0);
			glEnable(GL_COLOR_MATERIAL);
			GLfloat pos[] = { 1, 1, 1 };
			GLfloat amb[] = { 0, 0, 0, 1 };
			GLfloat dif[] = { 1, 1, 1, 1 };
			glLightfv(GL_LIGHT0, GL_POSITION, pos);
			glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
			glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);
			glLightfv(GL_LIGHT0, GL_SPECULAR, dif);
			glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
			glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, amb);
			GLfloat mspec[] = { 0, 0, 0, 1 };
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mspec);

			// Default line stipple used in selection
			glLineStipple(1, 61680);

			// Selection buffer
			selectBuffer = new GLuint[32768 * 4];
			glSelectBuffer(32768 * 4, selectBuffer);

			// Create the font display lists
			SelectObject(mhDC, (HGDIOBJ)this->Font->ToHfont());
			base = glGenLists(256);
			rasterbase = glGenLists(256);

			GLYPHMETRICSFLOAT gmf[256];
			wglUseFontOutlines(mhDC, 0, 256, base, 0.0f, 0.0f, WGL_FONT_POLYGONS, gmf);
			charWidths = gcnew List<float>();
			for(int i = 0; i <256; i++)
				charWidths->Add(gmf[i].gmfBlackBoxX);
			wglUseFontBitmaps(mhDC, 0, 256, rasterbase);

			// Object IDs for selection mode
			selectBoxes = gcnew Dictionary<GLuint, GLPickBox>();
		}
	}

	GLCanvas3D::~GLCanvas3D()
	{
		if(!this->DesignMode)
		{
			wglMakeCurrent(NULL, NULL);
			wglDeleteContext(mhGLRC);
			ReleaseDC((HWND)this->Handle.ToPointer(), mhDC);

			// Delete font display lists
			glDeleteLists(base, 256);
			glDeleteLists(rasterbase, 256);

			// Delete the selection buffer
			delete[] selectBuffer;
		}
	}

	void GLCanvas3D::OnPaint(System::Windows::Forms::PaintEventArgs^ e) 
	{
		if(this->DesignMode) 
		{
			e->Graphics->Clear(this->BackColor);
			e->Graphics->DrawString("GLCanvas3D", this->Font, gcnew SolidBrush(this->ForeColor), 10, 10);
			return;
		}

		// Save previous context and make our context current
		bool contextDifferent = (wglGetCurrentContext() != mhGLRC);
		HDC mhOldDC = 0;
		HGLRC mhOldGLRC = 0;
		if(contextDifferent)
		{
			mhOldDC = wglGetCurrentDC();
			mhOldGLRC = wglGetCurrentContext();
			wglMakeCurrent(mhDC, mhGLRC);
		}
	
		// Set the view frustrum
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		int cheight = Math::Max(1, this->ClientRectangle.Height);
		float fwidth = 1.0f * (float)this->ClientRectangle.Width / (float)cheight;
		if(Perspective)
		{
			glFrustum(-fwidth, fwidth, -1, 1, 1.0f, 100000.0f);
		}
		else
		{
			float zoom = 1.0f / Math::Max(0.00001f, mCamera->Distance);
			glOrtho(-fwidth / zoom, fwidth / zoom, -1 / zoom, 1 / zoom, 1.0f, 100000.0f);
		}

		// Set the camera
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(mCamera->Position.X, mCamera->Position.Y, mCamera->Position.Z, mCamera->Target.X, mCamera->Target.Y, mCamera->Target.Z, mCamera->Up.X, mCamera->Up.Y, mCamera->Up.Z);

		// Create the GLGraphics object
		GLView::GLGraphics3D ^ graphics = gcnew GLView::GLGraphics3D(this, e->Graphics);

		// Clear screen
		glClearColor(((float)BackColor.R) / 255, ((float)BackColor.G) / 255, ((float)BackColor.B) / 255, ((float)BackColor.A) / 255);
		glClearDepth(1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Raise the custom draw event
		OnRender(gcnew GLView::Canvas3DRenderEventArgs(graphics));
		
		// Get view properties
		mOrigin = graphics->ModelOrigin();
		mSize = graphics->ModelSize();

		// Draw the floor
		if(this->DrawFloor)
		{
			float floorSize = 5.0f * mSize;
			glColor4f((float)mFloorColor.R / 256.0f, (float)mFloorColor.G / 256.0f, (float)mFloorColor.B / 256.0f, (float)mFloorColor.A / 256.0f);
			glBegin(GL_QUADS);
			glNormal3f(0.0f, 0.0f, 1.0f);
			glVertex3f(-floorSize, -floorSize, -0.0002f);
			glVertex3f(-floorSize, floorSize, -0.0002f);
			glVertex3f(floorSize, floorSize, -0.0002f);
			glVertex3f(floorSize, -floorSize, -0.0002f);
			glEnd();

			// Draw the grid
			float spacing = mSize / 10.0f;
			glColor4f((float)mGridColor.R / 256.0f, (float)mGridColor.G / 256.0f, (float)mGridColor.B / 256.0f, (float)mGridColor.A / 256.0f);
			glBegin(GL_LINES);
			glNormal3f(0.0f, 0.0f, 1.0f);
			for(int i = -50; i <= 50; i++)
			{
				glVertex3f(-floorSize, (float)i * spacing, -0.0001f);
				glVertex3f(floorSize, (float)i * spacing, -0.0001f);
				glVertex3f((float)i * spacing, -floorSize, -0.0001f);
				glVertex3f((float)i * spacing, floorSize, -0.0001f);
			}
			glEnd();
		}

		// Draw the axis
		if(ShowAxis)
		{
			float length = Math::Min(1.0f, mSize / 10.0f);
			
			graphics->FillBox(0, 0, 0, length, 0, 0, length / 10.0f, length / 10.0f, Color::Red);
			graphics->FillBox(0, 0, 0, 0, length, 0, length / 10.0f, length / 10.0f, Color::Green);
			graphics->FillBox(0, 0, 0, 0, 0, length, length / 10.0f, length / 10.0f, Color::Blue);
		}

		// Draw selection rectangle if in selection mode
		glLoadIdentity();
		if (mSelecting)
		{
			glMatrixMode (GL_PROJECTION);
			glLoadIdentity();
			gluOrtho2D (0, this->ClientRectangle.Width, 0, this->ClientRectangle.Height);
			glMatrixMode (GL_MODELVIEW);
			glDisable(GL_LIGHTING);
			glDisable(GL_LINE_SMOOTH);

			PointF p1 = PointF((float)mSelPt1.X, (float)this->ClientRectangle.Height - mSelPt1.Y);
			PointF p2 = PointF((float)mSelPt2.X, (float)this->ClientRectangle.Height - mSelPt2.Y);
			
			switch (SelectionMode)
			{
			case SelectMode::Rectangle:
				glEnable(GL_LINE_STIPPLE);
				glBegin(GL_LINE_LOOP);
				glColor4ub(SelectionColor.R, SelectionColor.G, SelectionColor.B, SelectionColor.A);
				glVertex3f(p1.X, p1.Y, 0.0f);
				glVertex3f(p2.X, p1.Y, 0.0f);
				glVertex3f(p2.X, p2.Y, 0.0f);
				glVertex3f(p1.X, p2.Y, 0.0f);
				glEnd();
				glDisable(GL_LINE_STIPPLE);
				break;
			case SelectMode::ShadedRectangle:
				glBegin(GL_QUADS);
				glColor4ub(SelectionColor.R, SelectionColor.G, SelectionColor.B, 32);
				glVertex3f(p1.X, p1.Y, 0.0f);
				glVertex3f(p2.X, p1.Y, 0.0f);
				glVertex3f(p2.X, p2.Y, 0.0f);
				glVertex3f(p1.X, p2.Y, 0.0f);
				glEnd();
				glBegin(GL_LINE_LOOP);
				glColor4ub(SelectionColor.R, SelectionColor.G, SelectionColor.B, SelectionColor.A);
				glVertex3f(p1.X, p1.Y, 0.0f);
				glVertex3f(p2.X, p1.Y, 0.0f);
				glVertex3f(p2.X, p2.Y, 0.0f);
				glVertex3f(p1.X, p2.Y, 0.0f);
				glEnd();
				break;
            case SelectMode::ReversableShadedRectangle:
				glBegin(GL_QUADS);
                if(p1.X < p2.X)
				    glColor4ub(SelectionColor.R, SelectionColor.G, SelectionColor.B, 32);
                else
				    glColor4ub(ReverseSelectionColor.R, ReverseSelectionColor.G, ReverseSelectionColor.B, 32);
				glVertex3f(p1.X, p1.Y, 0.0f);
				glVertex3f(p2.X, p1.Y, 0.0f);
				glVertex3f(p2.X, p2.Y, 0.0f);
				glVertex3f(p1.X, p2.Y, 0.0f);
				glEnd();
                if(p1.X > p2.X)	glEnable(GL_LINE_STIPPLE);
				glBegin(GL_LINE_LOOP);
                if(p1.X < p2.X)
    				glColor4ub(SelectionColor.R, SelectionColor.G, SelectionColor.B, SelectionColor.A);
                else
				    glColor4ub(ReverseSelectionColor.R, ReverseSelectionColor.G, ReverseSelectionColor.B, ReverseSelectionColor.A);
				glVertex3f(p1.X, p1.Y, 0.0f);
				glVertex3f(p2.X, p1.Y, 0.0f);
				glVertex3f(p2.X, p2.Y, 0.0f);
				glVertex3f(p1.X, p2.Y, 0.0f);
				glEnd();
                if(p1.X > p2.X)	glDisable(GL_LINE_STIPPLE);
				break;
            case SelectMode::InvertedShadedRectangle:
				// Get view bounds
				Drawing::RectangleF bounds = GetViewPort();

				glBegin(GL_QUADS);

				glColor4ub(SelectionColor.R, SelectionColor.G, SelectionColor.B, 32);
				glVertex3f(bounds.Left, bounds.Top, 0.0f);
				glVertex3f(bounds.Right, bounds.Top, 0.0f);
                glVertex3f(bounds.Right, Math::Min(p1.Y, p2.Y), 0.0f);
				glVertex3f(bounds.Left, Math::Min(p1.Y, p2.Y), 0.0f);

				glVertex3f(bounds.Left, bounds.Bottom, 0.0f);
				glVertex3f(bounds.Right, bounds.Bottom, 0.0f);
                glVertex3f(bounds.Right, Math::Max(p1.Y, p2.Y), 0.0f);
				glVertex3f(bounds.Left, Math::Max(p1.Y, p2.Y), 0.0f);

				glVertex3f(bounds.Left, Math::Min(p1.Y, p2.Y), 0.0f);
				glVertex3f(Math::Min(p1.X, p2.X), Math::Min(p1.Y, p2.Y), 0.0f);
				glVertex3f(Math::Min(p1.X, p2.X), Math::Max(p1.Y, p2.Y), 0.0f);
				glVertex3f(bounds.Left, Math::Max(p1.Y, p2.Y), 0.0f);

				glVertex3f(Math::Max(p1.X, p2.X), Math::Min(p1.Y, p2.Y), 0.0f);
				glVertex3f(bounds.Right, Math::Min(p1.Y, p2.Y), 0.0f);
				glVertex3f(bounds.Right, Math::Max(p1.Y, p2.Y), 0.0f);
				glVertex3f(Math::Max(p1.X, p2.X), Math::Max(p1.Y, p2.Y), 0.0f);
                glEnd();

				glBegin(GL_LINE_LOOP);
				glColor4ub(SelectionColor.R, SelectionColor.G, SelectionColor.B, SelectionColor.A);
				glVertex3f(p1.X, p1.Y, 0.0f);
				glVertex3f(p2.X, p1.Y, 0.0f);
				glVertex3f(p2.X, p2.Y, 0.0f);
				glVertex3f(p1.X, p2.Y, 0.0f);
				glEnd();
				break;
			}
			glEnable(GL_LIGHTING);
			glEnable(GL_LINE_SMOOTH);
		}

		// Finish
		glFlush();

		// Swap buffers
		SwapBuffers(mhDC);

		// Restore previous context
		if(contextDifferent)
		{
			wglMakeCurrent(mhOldDC, mhOldGLRC);
		}

		// Raise the render done event
		OnRenderDone(gcnew System::EventArgs());
	}

	System::Object ^ GLCanvas3D::HitTest(int x, int y, int pickSize)
	{
		List<GLCanvas3D::GLSelectedObject> list = Select(x, y, pickSize, pickSize);

		if(list.Count > 0)
		{
			GLuint key = list[0].Key;
			GLuint depth = list[0].Z;
			for (int i = 1; i < list.Count; i++)
			{
				// Is this object closer?
				if (list[i].Z < depth)
				{
					key = list[i].Key;
					depth = list[i].Z;
				}
			}
			return selectBoxes[key].ID;
		}

		return nullptr;
	}

	List<System::Object^> ^ GLCanvas3D::HitTest(int x, int y, int width, int height, bool crossing)
	{
		List<GLCanvas3D::GLSelectedObject> ^ list = Select(x + width / 2, y + height / 2, width, height);
		List<System::Object^> ^ result = gcnew List<System::Object^>();
		for (int i = 0; i < list->Count; i++)
		{
			GLuint key = list[i].Key;
			System::Object ^ id = selectBoxes[key].ID;
			result->Add(id);
		}
		
		// Filter out crossing objects
		if(!crossing)
		{
			List<GLCanvas3D::GLSelectedObject> ^ border = nullptr;

			// 1 pixel to left
			border = Select(x - 1, y + height / 2, 2, height);
			for(int i = 0; i < border->Count; i++)
			{
				GLuint key = border[i].Key;
				System::Object ^ id = selectBoxes[key].ID;
				result->Remove(id);
			}

			// 1 pixel to right
			border = Select(x + width + 1, y + height / 2, 2, height);
			for(int i = 0; i < border->Count; i++)
			{
				GLuint key = border[i].Key;
				System::Object ^ id = selectBoxes[key].ID;
				result->Remove(id);
			}

			// 1 pixel to top
			border = Select(x + width / 2, y - 1, width, 2);
			for(int i = 0; i < border->Count; i++)
			{
				GLuint key = border[i].Key;
				System::Object ^ id = selectBoxes[key].ID;
				result->Remove(id);
			}

			// 1 pixel to bottom
			border = Select(x + width / 2, y + height + 1, width, 2);
			for(int i = 0; i < border->Count; i++)
			{
				GLuint key = border[i].Key;
				System::Object ^ id = selectBoxes[key].ID;
				result->Remove(id);
			}
		}

		return result;
	}

	List<GLCanvas3D::GLSelectedObject> ^ GLCanvas3D::Select(int x, int y, int width, int height)
	{
		// Save previous context and make our context current
		bool contextDifferent = (wglGetCurrentContext() != mhGLRC);
		HDC mhOldDC = 0;
		HGLRC mhOldGLRC = 0;
		if(contextDifferent)
		{
			mhOldDC = wglGetCurrentDC();
			mhOldGLRC = wglGetCurrentContext();
			wglMakeCurrent(mhDC, mhGLRC);
		}

		glRenderMode(GL_SELECT);

		glInitNames();
		glPushName(0);

		GLint viewport[4];
		glGetIntegerv(GL_VIEWPORT, viewport);

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluPickMatrix((GLdouble)x, (GLdouble)(viewport[3] - y), (GLdouble)width, (GLdouble)height, viewport);

		// Set the view frustrum
		int cheight = Math::Max(1, this->ClientRectangle.Height);
		float fwidth = 1.0f * (float)this->ClientRectangle.Width / (float)cheight;
		if(Perspective)
		{
			glFrustum(-fwidth, fwidth, -1, 1, 1.0f, 100000.0f);
		}
		else
		{
			float zoom = 1.0f / Math::Max(0.00001f, mCamera->Distance);
			glOrtho(-fwidth / zoom, fwidth / zoom, -1 / zoom, 1 / zoom, 1.0f, 100000.0f);
		}

		// Set the camera
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(mCamera->Position.X, mCamera->Position.Y, mCamera->Position.Z, mCamera->Target.X, mCamera->Target.Y, mCamera->Target.Z, mCamera->Up.X, mCamera->Up.Y, mCamera->Up.Z);

		// Draw the pick boxes
		DrawPickBoxes();

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);

		GLint hits = glRenderMode(GL_RENDER);

		// Restore previous context
		if(contextDifferent)
		{
			wglMakeCurrent(mhOldDC, mhOldGLRC);
		}

		List<GLSelectedObject> ^ list = gcnew List<GLSelectedObject>();
		for (int i = 0; i < hits; i++)
		{
			GLuint key = selectBuffer[i * 4 + 3];
			GLuint depth = selectBuffer[i * 4 + 1];
			list->Add(GLSelectedObject(key, depth));
		}

		return list;
	}

	void GLCanvas3D::DrawPickBoxes()
	{
		// Draw pick boxes in select mode
		for(int i = 0; i < selectBoxes->Count; i++)
		{
			glLoadName((GLuint)i);

			GLPickBox box = selectBoxes[(GLuint)i];
			float x1 = box.X1; float y1 = box.Y1; float z1 = box.Z1; 
			float x2 = box.X2; float y2 = box.Y2; float z2 = box.Z2;
			float width = box.Width; float height = box.Height;

			float len = (float)Math::Sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) + (z1 - z2) * (z1 - z2));
			float zrot = (float)(Math::Atan2(y2 - y1, x2 - x1) * 180.0 / Math::PI);
			float yrot = (float)(Math::Atan2(Math::Sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)), z2 - z1) * 180.0 / Math::PI);

			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			
			glTranslatef(x1, y1, z1);
			glRotatef(zrot, 0, 0, 1);
			glRotatef(yrot, 0, 1, 0);
			
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
		}

		glFlush();
	}

	void GLCanvas3D::OnPaintBackground(System::Windows::Forms::PaintEventArgs^ e) 
	{
	}

	System::Void GLCanvas3D::ControlResize(System::Object^ sender, System::EventArgs^ e)
	{
		ResetViewport();
		Invalidate();
	}

	System::Void GLCanvas3D::ResetViewport()
	{
		// Save previous context and make our context current
		bool contextDifferent = (wglGetCurrentContext() != mhGLRC);
		HDC mhOldDC = 0;
		HGLRC mhOldGLRC = 0;
		if(contextDifferent)
		{
			mhOldDC = wglGetCurrentDC();
			mhOldGLRC = wglGetCurrentContext();
			wglMakeCurrent(mhDC, mhGLRC);
		}

		// Reset the current viewport
		glViewport(0, 0, ClientSize.Width, ClientSize.Height);

		// Restore previous context
		if(contextDifferent)
		{
			wglMakeCurrent(mhOldDC, mhOldGLRC);
		}
	}

	System::Void GLCanvas3D::ControlMouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
	{
		if ((e->Button == Windows::Forms::MouseButtons::Middle) && (AllowZoomAndPan || AllowRotate) && !mPanning)
		{
			mPanning = true;
			mLastMouse = e->Location;
			this->Cursor = Windows::Forms::Cursors::NoMove2D;
		}
		else if ((e->Button == Windows::Forms::MouseButtons::Left) && !mPanning && SelectionMode != SelectMode::None)
		{
            mSelecting = true;
            mSelPt1.X = e->X;
            mSelPt1.Y = e->Y;
            mSelPt2.X = e->X;
            mSelPt2.Y = e->Y;
			this->Cursor = Windows::Forms::Cursors::Arrow;
		}
	}

	System::Void GLCanvas3D::ControlMouseMove(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
	{
		if ((e->Button == Windows::Forms::MouseButtons::Middle) && (mPanning))
		{
			bool controlDown = (Control::ModifierKeys & Keys::Control) == Keys::Control;
			bool rotate = false;
			if(PanWithControlKey)
				rotate = !controlDown;
			else
				rotate = controlDown;

			if(rotate && AllowRotate)
			{
				// Rotate the camera
				float deltax = mLastMouse.X - e->Location.X;
				float maxdeltax = Math::Max(1.0f, (float)this->ClientRectangle.Width / 2.0f);
				float deltaanglex = deltax / maxdeltax * 180.0f / 2.0f;
				mCamera->Yaw += deltaanglex;
				float deltay = mLastMouse.Y - e->Location.Y;
				float maxdeltay = Math::Max(1.0f, (float)this->ClientRectangle.Height / 2.0f);
				float deltaangley = deltay / maxdeltay * 90.0f / 2.0f;
				mCamera->Pitch -= deltaangley;
			}
			else if(!rotate && AllowZoomAndPan)
			{
				float deltax = mLastMouse.X - e->Location.X;
				float deltay = mLastMouse.Y - e->Location.Y;
				float maxdeltax = Math::Max(1.0f, (float)this->ClientRectangle.Width);
				float maxdeltay = Math::Max(1.0f, (float)this->ClientRectangle.Height);
				deltax = -deltax / maxdeltax;
				deltay = -deltay / maxdeltay;
				float len = 2.0f * mCamera->Distance * (float)Math::Tan(Math::PI / 6);
				Point3D dx = mCamera->Up;
				Point3D dy = mCamera->Up * (mCamera->Target - mCamera->Position);
				dy = dy / dy.Length;
				Point3D pan = dy * deltax * len * (float)this->ClientRectangle.Width / (float)this->ClientRectangle.Height +
					dx * deltay * len;
				mCamera->Position += pan;
				mCamera->Target += pan;
			}
			mLastMouse = e->Location;
			Invalidate();
		}
		else if (mSelecting)
		{
			mSelPt2.X = Math::Min(ClientRectangle.Right - 1, Math::Max(1, e->X));
			mSelPt2.Y = Math::Min(ClientRectangle.Bottom - 1, Math::Max(1, e->Y));
            Invalidate();
		}
	}

	System::Void GLCanvas3D::ControlMouseUp(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
	{
		if ((e->Button == Windows::Forms::MouseButtons::Middle) && mPanning)
		{
			mPanning = false;
			this->Cursor = Windows::Forms::Cursors::Cross;
			Invalidate();
		}
		else if (mSelecting)
		{
			mSelPt2.X = Math::Min(ClientRectangle.Right - 1, Math::Max(1, e->X));
			mSelPt2.Y = Math::Min(ClientRectangle.Bottom - 1, Math::Max(1, e->Y));
            mSelecting = false;
            Invalidate();
			
			if (((Math::Abs(mSelPt1.X - mSelPt2.X) < 2) && (Math::Abs(mSelPt1.Y - mSelPt2.Y) < 2)))
				OnMouseSelect(gcnew GLView::Canvas3DMouseSelectEventArgs(mSelPt2, mSelPt2, e->Button));
			else
				OnMouseSelect(gcnew GLView::Canvas3DMouseSelectEventArgs(mSelPt1, mSelPt2, e->Button, (mSelPt2.X < mSelPt1.X)));
		}

		this->Cursor = Windows::Forms::Cursors::Cross;
	}

	System::Void GLCanvas3D::ControlMouseWheel(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
	{
		if (AllowZoomAndPan)
		{
			mCamera->Distance = Math::Max(0.2f * mSize, mCamera->Distance - (float)Math::Sign(e->Delta) * mSize / 10.0f);
			Invalidate();
		}
	}

	System::Void GLCanvas3D::ResetView()
	{
		mCamera->Pan(mCamera->Target, mOrigin);
		mCamera->Distance = Math::Max(4.0f, mSize * 1.0f);
		Invalidate();
	}

	System::Void GLCanvas3D::ControlMouseDoubleClick(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
	{
		if ((e->Button == Windows::Forms::MouseButtons::Middle) && AllowZoomAndPan)
			ResetView();
	}

	System::Void GLCanvas3D::CreatePickBox(System::Object^ id, float x1, float y1, float z1, float x2, float y2, float z2, float width, float height)
	{
		GLuint lastkey = selectBoxes->Count;
		selectBoxes->Add(lastkey, GLPickBox(id, x1, y1, z1, x2, y2, z2, width, height));
	}

	System::Void GLCanvas3D::RemovePickBox(System::Object^ id)
	{
		bool found = false;
		GLuint toRemove = 0;
		for each(KeyValuePair<GLuint, GLPickBox> kv in selectBoxes)
		{
			if(kv.Value == id)
			{
				found = true;
				toRemove = kv.Key;
				break;
			}
		}
		if(found)
			selectBoxes->Remove(toRemove);
	}

	System::Void GLCanvas3D::ClearPickBoxes()
	{
		selectBoxes->Clear();
	}
}