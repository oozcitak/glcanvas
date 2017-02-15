#pragma once

#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "Point3D.h"
#include "Camera.h"

using namespace System;
using namespace System::Drawing;
using namespace System::ComponentModel;
using namespace System::Windows::Forms;
using namespace System::Collections::Generic;

namespace GLCanvas
{
	// Forward class declarations
	ref class GLGraphics3D;
	ref class Canvas3DRenderEventArgs;
	ref class Canvas3DMouseSelectEventArgs;
	value class GLPickBox;

	/// <summary>
	/// Represents a 3D drawing canvas utilizing OpenGL.
	/// </summary>
	[ToolboxBitmap(GLCanvas::GLCanvas3D::typeid)]
    [DefaultEvent("Render")]
	[Description("Represents a 3D drawing canvas utilizing OpenGL.")]
	[Docking(DockingBehavior::Ask)]
	public ref class GLCanvas3D : public System::Windows::Forms::Control
	{
	// Constructor/destructor
	public:
		GLCanvas3D();

	protected:
		~GLCanvas3D();

	// Enums
	public:
		/// <summary>
		/// Represents the mouse selection mode.
		/// </summary>
		enum class SelectMode
		{
			/// <summary>
			/// Mouse selection is disabled.
			/// </summary>
			None,
			/// <summary>
			/// The user selects a rectange by picking two corner points.
			/// </summary>
			Rectangle,
			/// <summary>
			/// The user selects a shaded rectange by picking two corner points.
			/// </summary>
			ShadedRectangle,
			/// <summary>
			/// The user selects a reversible shaded rectangle by picking two corner points.
			/// </summary>
            ReversableShadedRectangle,
			/// <summary>
			/// The user selects an inverted rectange by picking two corner points.
			/// </summary>
            InvertedShadedRectangle
		};

	// Private classes
	private:
		/// <summary>
		/// Represents a selected object.
		/// </summary>
		value class GLSelectedObject 
		{
		public:
			/// <summary>
			/// The selected object Key.
			/// </summary>			
			GLuint Key;
			/// <summary>
			/// Distance of the object to the camera.
			/// </summary>
			GLuint Z;

			GLSelectedObject(GLuint key, GLuint z)
			{
				Key = key;
				Z = z;
			}
		};

	// Member variables
	private:
		HDC mhDC;
		HGLRC mhGLRC;
		bool mIsAccelerated;
		Windows::Forms::BorderStyle mBorderStyle;
		Camera ^ mCamera;
	    bool mPanning;
		PointF mLastMouse;
		Point3D mOrigin;
		float mSize;
		bool mAntiAlias;
		GLuint base, rasterbase;
		System::Drawing::Color mFloorColor;
		System::Drawing::Color mGridColor;
		bool mLighting;
		bool mSelecting;
		Drawing::Point mSelPt1, mSelPt2;
		GLuint* selectBuffer;
	internal:
		Dictionary<GLuint, GLPickBox> ^ selectBoxes;
		List<float> ^ charWidths;

	public:
		/// <summary>
        /// Gets or sets the projection type.
        /// </summary>
		[Category("Appearance"), Description("Gets or sets the projection type."), DefaultValue(true)]
		property bool Perspective;
		/// <summary>
        /// Gets the camera.
        /// </summary>
		[Category("Appearance"), Description("Gets the camera."), Browsable(false)]
		property GLCanvas::Camera ^ Camera
		{ 
			virtual GLCanvas::Camera ^ get(void) { return mCamera; }
		}
		/// <summary>
        /// Gets the model origin as drawn in the last render.
        /// </summary>
		[Category("Appearance"), Description("Gets the model origin."), Browsable(false)]
		property GLCanvas::Point3D ModelOrigin
		{ 
			virtual GLCanvas::Point3D get(void) { return mOrigin; }
		}
		/// <summary>
        /// Gets the model size as drawn in the last render.
        /// </summary>
		[Category("Appearance"), Description("Gets the model size."), Browsable(false)]
		property float ModelSize
		{ 
			virtual float get(void) { return mSize; } 
		}
		/// <summary>
        /// Gets or sets the border style of the control.
        /// </summary>
		[Category("Appearance"), Description("Gets or sets the border style of the control."), DefaultValue(Windows::Forms::BorderStyle::typeid, "Fixed3D")]
		property Windows::Forms::BorderStyle BorderStyle 
		{ 
			virtual Windows::Forms::BorderStyle get(void) { return mBorderStyle; } 
			virtual void set(Windows::Forms::BorderStyle value) { mBorderStyle = value; UpdateStyles(); } 
		}
		/// <summary>
		/// Determines whether hardware acceleration is enabled.
		/// </summary>
		[Category("Behavior"), Browsable(false), Description("Determines whether hardware acceleration is enabled.")] 
		property bool IsAccelerated
		{
			virtual bool get(void) { return mIsAccelerated; }
		}
		/// <summary>
		/// Determines if the user is currently selecting with the mouse.
		/// </summary>
		[Category("Behavior"), Browsable(false), DefaultValue(false), Description("Determines if the user is currently selecting with the mouse.")]
		property bool Selecting
        {
            virtual bool get(void) { return mSelecting; }
        }
				/// <summary>
		/// Determines if the user is currently panning or rotating with the mouse.
		/// </summary>
		[Category("Behavior"), Browsable(false), DefaultValue(false), Description("Determines if the user is currently panning or rotating with the mouse.")]
		property bool Panning
        {
            virtual bool get(void) { return mPanning; }
        }
        /// <summary>
		/// Gets or sets the mouse selection mode.
		/// </summary>
		[Category("Behavior"), Browsable(true), DefaultValue(GLCanvas::GLCanvas3D::SelectMode::typeid, "None"), Description("Gets or sets the mouse selection mode.")]
		property SelectMode SelectionMode;
		/// <summary>
		/// Gets or sets the color of selection lines.
		/// </summary>
		[Category("Appearance"), Browsable(true), DefaultValue(System::Drawing::Color::typeid, "HighLight"), Description("Gets or sets the color of selection lines.")]
		property Drawing::Color SelectionColor;
		/// <summary>
		/// Gets or sets the color of reverse selection lines.
		/// </summary>
		[Category("Appearance"), Browsable(true), DefaultValue(System::Drawing::Color::typeid, "HighLight"), Description("Gets or sets the color of reverse selection lines.")]
		property Drawing::Color ReverseSelectionColor;
		/// <summary>
		/// Determines the behaviour of the Control key in pan and rotate.  
		/// When set to true, the view is panned while holding down the control key, rotated without the control key;
		/// when false, the behaviour is reversed.
		/// </summary>
		[Category("Behavior"), Browsable(true), DefaultValue(true), Description("Determines the behaviour of the Control key in pan and rotate.")] 
		property bool PanWithControlKey;
		/// <summary>
		/// Determines whether the camera can be zoomed and panned with the mouse.
		/// </summary>
		[Category("Behavior"), Browsable(true), DefaultValue(true), Description("Determines whether the camera can be zoomed and panned with the mouse.")] 
		property bool AllowZoomAndPan;
		/// <summary>
		/// Determines whether the camera can be rotated.
		/// </summary>
		[Category("Behavior"), Browsable(true), DefaultValue(true), Description("Determines whether the camera can be rotated.")] 
		property bool AllowRotate;
		/// <summary>
		/// The cursor that appears when the pointer moves over the control.
		/// </summary>
		[Category("Appearance"), Browsable(true), DefaultValue(Windows::Forms::Cursor::typeid, "Cross"), Description("The cursor that appears when the pointer moves over the control.")]
        property Windows::Forms::Cursor ^ Cursor
		{
			virtual Windows::Forms::Cursor ^ get(void) override { return Control::Cursor; }
			virtual void set(Windows::Forms::Cursor ^ value) override { Control::Cursor = value; }
		}
		/// <summary>
		/// Determines whether the floor is drawn.
		/// </summary>
		[Category("Appearance"), Browsable(true), DefaultValue(true), Description("Determines whether the floor is drawn.")] 
		property bool DrawFloor;
		/// <summary>
		/// Determines whether the axis is drawn.
		/// </summary>
		[Category("Appearance"), Browsable(true), DefaultValue(true), Description("Determines whether the axis is drawn.")] 
		property bool ShowAxis;
		/// <summary>
		/// Gets or sets the color of floor.
		/// </summary>
		[Category("Appearance"), Browsable(true), DefaultValue(System::Drawing::Color::typeid, "Beige"), Description("Gets or sets the color of floor.")]
		property Drawing::Color FloorColor
		{
			virtual Drawing::Color get(void) { return mFloorColor; }
			virtual void set(Drawing::Color value) { mFloorColor = value; Invalidate(); }
		}
		/// <summary>
		/// Gets or sets the color of grid lines.
		/// </summary>
		[Category("Appearance"), Browsable(true), DefaultValue(System::Drawing::Color::typeid, "Wheat"), Description("Gets or sets the color of grid lines.")]
		property Drawing::Color GridColor
		{
			virtual Drawing::Color get(void) { return mGridColor; }
			virtual void set(Drawing::Color value) { mGridColor = value; Invalidate(); }
		}
		/// <summary>
		/// Gets or sets the background color of the control.
		/// </summary>
		[Category("Appearance"), Browsable(true), DefaultValue(System::Drawing::Color::typeid, "White"), Description("Gets or sets the background color of the control.")]
		property Drawing::Color BackColor
		{
			virtual Drawing::Color get(void) override { return Control::BackColor; }
			virtual void set(Drawing::Color value) override { Control::BackColor = value; Invalidate(); }
		}
		/// <summary>
		/// Determines whether lines are anti-aliased.
		/// </summary>
		[Category("Appearance"), Browsable(true), DefaultValue(true), Description("Determines whether lines are anti-aliased.")]
		property bool AntiAlias
		{
			virtual bool get(void) { return mAntiAlias; }
			virtual void set(bool value) 
			{ 
				if (value)
					glEnable(GL_LINE_SMOOTH);
				else
					glDisable(GL_LINE_SMOOTH);
				mAntiAlias = value; 
				Invalidate(); 
			}
		}
		/// <summary>
		/// Determines whether simple lighting model is active.
		/// </summary>
		[Category("Appearance"), Browsable(true), DefaultValue(true), Description("Determines whether simple lighting model is active.")]
		property bool Lighting
		{
			virtual bool get(void) { return mLighting; }
			virtual void set(bool value) 
			{ 
				if (value)
				{
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
				}
				else
				{
					glDisable(GL_LIGHTING);
					glDisable(GL_LIGHT0);
					glDisable(GL_COLOR_MATERIAL);
				}
				mLighting = value; 
			}
		}
		/// <summary>
		/// Returns the display list used to draw vector text.
		/// </summary>
		[Category("Appearance"), Browsable(false), Description("Returns the display list used to draw vector text.")] 
		property GLuint VectorListBase
		{
			virtual GLuint get(void) { return base; }
		}
		/// <summary>
		/// Returns the display list used to draw raster text.
		/// </summary>
		[Category("Appearance"), Browsable(false), Description("Returns the display list used to draw raster text.")] 
		property GLuint RasterListBase
		{
			virtual GLuint get(void) { return rasterbase; }
		}
		/// <summary>
		/// Gets or sets the font used to display text in the control.
		/// </summary>
		[Category("Appearance"), Browsable(true), Description("Gets or sets the font used to display text in the control.")] 
		property System::Drawing::Font ^ Font
		{
			virtual void set(System::Drawing::Font ^ value) override
			{
				Control::Font::set(value);

				if(!this->DesignMode)
				{
					// Save previous context and make our context current
					HDC mhOldDC = wglGetCurrentDC();
					HGLRC mhOldGLRC = wglGetCurrentContext();
					wglMakeCurrent(mhDC, mhGLRC);

					// Delete old display lists
					glDeleteLists(base, 256);
					glDeleteLists(rasterbase, 256);
					
					// Create the font display lists
					SelectObject(mhDC, (HGDIOBJ)value->ToHfont());
					base = glGenLists(256);
					rasterbase = glGenLists(256);
					
					wglUseFontOutlines(mhDC, 0, 256, base, 0.0f, 0.0f, WGL_FONT_POLYGONS, NULL);
					wglUseFontBitmaps(mhDC, 0, 256, rasterbase);
					
					// Restore previous context
					wglMakeCurrent(mhOldDC, mhOldGLRC);

					Invalidate(); 
				}
			}
		}

	// Public methods.
	public:
		/// <summary>
		/// Resets the camera.
		/// </summary>
		System::Void ResetView();
		/// <summary>
		/// Returns the ID of the object hit by the given window coordinates.
		/// </summary>
		/// <param name="x">X coordinate.</param>
		/// <param name="y">Y coordinate.</param>
		/// <param name="pickSize">Size of the pick box.</param>
		/// <returns>Object ID.</returns>
		System::Object ^ HitTest(int x, int y, int pickSize);
		/// <summary>
		/// Returns the ID of the object hit by the given window coordinates.
		/// </summary>
		/// <param name="x">X coordinate.</param>
		/// <param name="y">Y coordinate.</param>
		/// <returns>Object ID.</returns>
		System::Object ^ HitTest(int x, int y)
		{
			return HitTest(x, y, 3);
		}
				/// <summary>
		/// Returns the list of object IDs inside the given selection rectangle.
		/// </summary>
		/// <param name="x">X coordinate.</param>
		/// <param name="y">Y coordinate.</param>
		/// <param name="width">Width of the selection window.</param>
		/// <param name="height">Height of the selection window.</param>
		/// <param name="crossing">When set to true, objects crossing the selection
		/// window will also be returned; when false, only the objects completely
		/// inside the selection window will be returned.</param>
		/// <returns>List of Object IDs.</returns>
		List<System::Object^> ^ HitTest(int x, int y, int width, int height, bool crossing);
		/// <summary>
		/// Returns the list of object IDs inside the given selection rectangle.
		/// </summary>
		/// <param name="x">X coordinate.</param>
		/// <param name="y">Y coordinate.</param>
		/// <param name="width">Width of the selection window.</param>
		/// <param name="height">Height of the selection window.</param>
		/// <returns>List of Object IDs.</returns>
		List<System::Object^> ^ HitTest(int x, int y, int width, int height)
		{
			return HitTest(x, y, width, height, true);
		}
		/// <summary>
		/// Converts the given client coordinates to world coordinates.
		/// </summary>
		/// <param name="x">X coordinate.</param>
		/// <param name="y">Y coordinate.</param>
		Point3D ScreenToWorld(int x, int y)
		{
			return GetOGLPos(x, y);
		}
		/// <summary>
		/// Creates a selectable dummy object.
		/// </summary>
		/// <param name="id">Object ID</param>
		/// <param name="x1">X coordinate of first point</param>
		/// <param name="y1">Y coordinate of first point</param>
		/// <param name="z1">Z coordinate of first point</param>
		/// <param name="x2">X coordinate of second point</param>
		/// <param name="y2">Y coordinate of second point</param>
		/// <param name="z2">Z coordinate of second point</param>
		/// <param name="width">Width of top and bottom faces</param>
		/// <param name="height">Height of top and bottom faces</param>
		System::Void CreatePickBox(System::Object^ id, float x1, float y1, float z1, float x2, float y2, float z2, float width, float height);
		/// <summary>
		/// Removes a pick box.
		/// </summary>
		/// <param name="id">Object ID</param>
		System::Void RemovePickBox(System::Object^ id);
		/// <summary>
		/// Removes all pick boxes.
		/// </summary>
		System::Void ClearPickBoxes();

	private:
		System::Void ControlResize(System::Object^ sender, System::EventArgs^ e);
		System::Void ControlMouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e);
		System::Void ControlMouseMove(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e);
		System::Void ControlMouseUp(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e);
		System::Void ControlMouseWheel(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e);
		System::Void ControlMouseDoubleClick(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e);

	protected:
		virtual property System::Windows::Forms::CreateParams^ CreateParams
		{
			System::Windows::Forms::CreateParams^ get(void) override sealed
			{ 
				System::Windows::Forms::CreateParams^ cp = Control::CreateParams;

                cp->Style &= ~WS_BORDER;
                cp->ExStyle &= ~WS_EX_CLIENTEDGE;
				if (mBorderStyle == Windows::Forms::BorderStyle::Fixed3D)
                    cp->ExStyle |= WS_EX_CLIENTEDGE;
				else if (mBorderStyle == Windows::Forms::BorderStyle::FixedSingle)
                    cp->Style |= WS_BORDER;

				cp->ClassStyle = cp->ClassStyle | CS_VREDRAW | CS_HREDRAW | CS_OWNDC;

				return cp; 
			}
		}
		virtual void OnPaint(System::Windows::Forms::PaintEventArgs^ e) override sealed;
		virtual void OnPaintBackground(System::Windows::Forms::PaintEventArgs^ e) override sealed;

	protected:
		/// <summary>
		/// Raises the GLCanvas3D.Render event.
		/// </summary>
		virtual void OnRender(GLCanvas::Canvas3DRenderEventArgs^ e)
		{
			Render(this, e);
		}
		/// <summary>
		/// Raises the GLCanvas3D.RenderDone event.
		/// </summary>
		virtual void OnRenderDone(System::EventArgs^ e)
		{
			RenderDone(this, e);
		}
		/// <summary>
		/// Raises the GLCanvas3D.MouseSelect event.
		/// </summary>
		virtual void OnMouseSelect(GLCanvas::Canvas3DMouseSelectEventArgs^ e)
		{
			MouseSelect(this, e);
		}

	// Event delegates
	public:
		/// <summary>
		/// Represents the method that will handle the Render event. 
		/// </summary>
		/// <param name="sender">The GLCanvas3D object that is the source of the event.</param>
		/// <param name="e">A Canvas3DRenderEventArgs that contains event data.</param>
		[EditorBrowsable(EditorBrowsableState::Never)]		
		delegate void RenderHandler(System::Object^ sender, GLCanvas::Canvas3DRenderEventArgs^ e);
		/// <summary>
		/// Represents the method that will handle the Render event. 
		/// </summary>
		/// <param name="sender">The GLCanvas3D object that is the source of the event.</param>
		/// <param name="e">A System.EventArgs that contains event data.</param>
		[EditorBrowsable(EditorBrowsableState::Never)]	
		delegate void RenderDoneHandler(System::Object^ sender, System::EventArgs^ e);
		/// <summary>
		/// Represents the method that will handle the MouseSelect event. 
		/// </summary>
		/// <param name="sender">The GLCanvas3D object that is the source of the event.</param>
		/// <param name="e">A Canvas3DMouseSelectEventArgs that contains event data.</param>
		[EditorBrowsable(EditorBrowsableState::Never)]
		delegate void MouseSelectHandler(System::Object^ sender, GLCanvas::Canvas3DMouseSelectEventArgs^ e);

	// Events
	public:
		/// <summary>
		/// Occurs when the control is redrawn.
		/// </summary>
		[Category("Appearance"), Browsable(true), Description("Occurs when the control is redrawn.")] 
		event RenderHandler ^ Render;
		/// <summary>
		/// Occurs after the control is redrawn.
		/// </summary>
		[Category("Appearance"), Browsable(true), Description("Occurs after the control is redrawn.")] 
		event RenderDoneHandler ^ RenderDone;
		/// <summary>
		/// Occurs when the user selects a region with the mouse.
		/// </summary>
		[Category("Mouse"), Browsable(true), Description("Occurs when the user selects a region with the mouse.")] 
		event MouseSelectHandler^ MouseSelect;

	private: 
		System::Void InitializeComponent()
		{
			this->Name = L"GLCanvas3D";
		}
		System::Void DrawPickBoxes();
		System::Void ResetViewport();
		/// <summary>
		/// Returns the coordinates of the viewport in world coordinates.
		/// </summary>
		Drawing::RectangleF GetViewPort()
		{ 
			Point3D bl = GetOGLPos(ClientRectangle.Left, ClientRectangle.Bottom);
			Point3D tr = GetOGLPos(ClientRectangle.Right, ClientRectangle.Top);
			return Drawing::RectangleF(bl.X, bl.Y, tr.X - bl.X, tr.Y - bl.Y);
		}
		/// <summary>
		/// Converts the given screen coordinates to model coordinates.
		/// </summary>
		/// <param name="x">X coordinate.</param>
		/// <param name="y">Y coordinate.</param>
		/// <returns>A Point3D in model coordinates.</returns>
		Point3D GetOGLPos(int x, int y)
		{
			GLfloat winX, winY, winZ;
			GLdouble posX, posY, posZ;

			GLint viewport[4];
			glGetIntegerv(GL_VIEWPORT, viewport);
			GLdouble modelview[16];
			glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
			GLdouble projection[16];
			glGetDoublev(GL_PROJECTION_MATRIX, projection);

			winX = (float)x;
			winY = (float)viewport[3] - (float)y;
			glReadPixels(x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

			gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

			return Point3D((float)posX, (float)posY, (float)posZ);
		}
		/// <summary>
		/// Returns the list of object IDs inside the given selection rectangle.
		/// </summary>
		/// <param name="x">X coordinate of the center of the selection window.</param>
		/// <param name="y">Y coordinate of the center of the selection window.</param>
		/// <param name="width">Width of the selection window.</param>
		/// <param name="height">Height of the selection window.</param>
		/// <returns>List of selected objects.</returns>
		List<GLSelectedObject> ^ Select(int x, int y, int width, int height);
	};
}
