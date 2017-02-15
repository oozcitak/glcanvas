#pragma once

#include <windows.h>
#include <GL/gl.h>

using namespace System;
using namespace System::Drawing;
using namespace System::ComponentModel;
using namespace System::Windows::Forms;


namespace GLCanvas {

	// Forward class declarations
	ref class GLGraphics2D;
	ref class Canvas2DRenderEventArgs;
	ref class Canvas2DMouseSelectEventArgs;

	/// <summary>
	/// Represents a 2D drawing canvas utilizing OpenGL.
	/// </summary>
	[ToolboxBitmap(GLCanvas::GLCanvas2D::typeid)]
    [DefaultEvent("Render")]
	[Description("Represents a 2D drawing canvas utilizing OpenGL.")]
	[Docking(DockingBehavior::Ask)]
	public ref class GLCanvas2D : public System::Windows::Forms::Control
	{
	private:
		value class GLPointFloat
		{
		    float X;
			float Y;
		};

		value class GLGlyphMetrics
		{
		    float gmfBlackBoxX;
			float gmfBlackBoxY;
			GLPointFloat gmfptGlyphOrigin;
			float gmfCellIncX;
			float gmfCellIncY;
		};

	// Constructor/destructor
	public:
		GLCanvas2D();

	protected:
		~GLCanvas2D();

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
			/// The user can pick a single point.
			/// </summary>
			PointPick,
			/// <summary>
			/// The user selects a line by picking two points.
			/// </summary>
			Line,
			/// <summary>
			/// The user selects a circle by picking the center 
			/// and a point on the circle.
			/// </summary>
			Circle,
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

	// Member variables
	private:
		HDC mhDC;
		HGLRC mhGLRC;
		bool mIsAccelerated;
		Windows::Forms::BorderStyle mBorderStyle;
		PointF mCameraPosition;
	    bool mPanning;
		bool mSelecting;
	    Point mLastMouse;
		float mZoomFactor;
		Drawing::Point mSelPt1, mSelPt2;
		Drawing::RectangleF mLimits;
		bool mShowGrid;
		bool mShowAxes;
		float mGridSpacing;
		Drawing::Color mMinorGridColor;
		Drawing::Color mMajorGridColor;
		Drawing::Color mAxisColor;
		bool mDynamicGrid;
		bool mAntiAlias;
		GLuint base, rasterbase;

	public:
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
		/// Gets or sets the mouse selection mode.
		/// </summary>
		[Category("Behavior"), Browsable(true), DefaultValue(GLCanvas::GLCanvas2D::SelectMode::typeid, "None"), Description("Gets or sets the mouse selection mode.")]
		property SelectMode SelectionMode;
		/// <summary>
		/// Determines whether zooming and panning with the mouse is allowed.
		/// </summary>
		[Category("Behavior"), Browsable(true), DefaultValue(true), Description("Determines whether zooming and panning with the mouse is allowed.")] 
		property bool AllowZoomAndPan;
		/// <summary>
		/// Determines whether grid lines are visible.
		/// </summary>
		[Category("Appearance"), Browsable(true), DefaultValue(true), Description("Determines whether grid lines are visible.")] 
		property bool ShowGrid
		{
			virtual bool get(void) { return mShowGrid; }
			virtual void set(bool value) { mShowGrid = value; Invalidate(); }
		}
		/// <summary>
		/// Determines whether axes are visible.
		/// </summary>
		[Category("Appearance"), Browsable(true), DefaultValue(true), Description("Determines whether axes are visible.")] 
		property bool ShowAxes
		{
			virtual bool get(void) { return mShowAxes; }
			virtual void set(bool value) { mShowAxes = value; Invalidate(); }
		}
		/// <summary>
		/// Determines whether grid spacing is dynamically determined.
		/// </summary>
		[Category("Appearance"), Browsable(true), DefaultValue(true), Description("Determines whether grid spacing is dynamically determined.")] 
		property bool DynamicGrid
		{
			virtual bool get(void) { return mDynamicGrid; }
			virtual void set(bool value) { mDynamicGrid = value; Invalidate(); }
		}
		/// <summary>
		/// Gets or sets the grid spacing.
		/// </summary>
		[Category("Appearance"), Browsable(true), DefaultValue(10.0f), Description("Gets or sets the grid spacing.")] 
		property float GridSpacing
		{
			virtual float get(void) { return mGridSpacing; }
			virtual void set(float value) { mGridSpacing = value; Invalidate(); }
		}
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
		/// Gets or sets the background color of the control.
		/// </summary>
		[Category("Appearance"), Browsable(true), DefaultValue(System::Drawing::Color::typeid, "White"), Description("Gets or sets the background color of the control.")]
		property Drawing::Color BackColor
		{
			virtual Drawing::Color get(void) override { return Control::BackColor; }
			virtual void set(Drawing::Color value) override { Control::BackColor = value; Invalidate(); }
		}
		/// <summary>
		/// Gets or sets the color of minor gridlines.
		/// </summary>
		[Category("Appearance"), Browsable(true), DefaultValue(System::Drawing::Color::typeid, "Beige"), Description("Gets or sets the color of minor gridlines.")]
		property Drawing::Color MinorGridColor
		{
			virtual Drawing::Color get(void) { return mMinorGridColor; }
			virtual void set(Drawing::Color value) { mMinorGridColor = value; Invalidate(); }
		}
		/// <summary>
		/// Gets or sets the color of major gridlines.
		/// </summary>
		[Category("Appearance"), Browsable(true), DefaultValue(System::Drawing::Color::typeid, "Bisque"), Description("Gets or sets the color of major gridlines.")]
		property Drawing::Color MajorGridColor
		{
			virtual Drawing::Color get(void) { return mMajorGridColor; }
			virtual void set(Drawing::Color value) { mMajorGridColor = value; Invalidate(); }
		}
		/// <summary>
		/// Gets or sets the color of axes.
		/// </summary>
		[Category("Appearance"), Browsable(true), DefaultValue(System::Drawing::Color::typeid, "BurlyWood"), Description("Gets or sets the color of axes.")]
		property Drawing::Color AxisColor
		{
			virtual Drawing::Color get(void) { return mAxisColor; }
			virtual void set(Drawing::Color value) { mAxisColor = value; Invalidate(); }
		}
		/// <summary>
		/// Determines whether lines are anti-aliased.
		/// </summary>
		[Category("Appearance"), Browsable(true), DefaultValue(false), Description("Determines whether lines are anti-aliased.")]
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
		/// Gets the limits of all drawing objects on the canvas.
		/// </summary>
		[Category("Behavior"), Browsable(false), Description("Gets the limits of all drawing objects on the canvas.")] 
		property Drawing::RectangleF Limits
		{
			virtual Drawing::RectangleF get(void) { return mLimits; }
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

	// Public methods
	public:
		/// <summary>
		/// Converts the given point from world coordinates to screen coordinates.
		/// </summary>
		/// <param name="x">X coordinate in world coordinates.</param>
		/// <param name="y">Y coordinate in world coordinates.</param>
		/// <returns>A Point in screen coordinates.</returns>
		Drawing::Point WorldToScreen(float x, float y)
		{ 
			return Drawing::Point((int)((x - mCameraPosition.X) / mZoomFactor) + ClientRectangle.Width / 2,
								 -(int)((y - mCameraPosition.Y) / mZoomFactor) + ClientRectangle.Height / 2);
		}
		/// <summary>
		/// Converts the given point from world coordinates to screen coordinates.
		/// </summary>
		/// <param name="pt">Location in world coordinates.</param>
		/// <returns>A Point in screen coordinates.</returns>
		Drawing::Point WorldToScreen(Drawing::PointF pt) { return WorldToScreen(pt.X, pt.Y); }
		/// <summary>
		/// Converts the given vector from world coordinates to screen coordinates.
		/// </summary>
		/// <param name="sz">Size in world coordinates.</param>
		/// <returns>A Size in screen coordinates.</returns>
		Drawing::Size WorldToScreen(Drawing::SizeF sz)
		{
			Drawing::Point pt1 = WorldToScreen(0.0f, 0.0f);
			Drawing::Point pt2 = WorldToScreen(sz.Width, sz.Height);
			return Drawing::Size(pt2.X - pt1.X, pt2.Y - pt1.Y);
		}
		/// <summary>
		/// Converts the given point from screen coordinates to world coordinates.
		/// </summary>
		/// <param name="x">X coordinate in screen coordinates.</param>
		/// <param name="y">Y coordinate in screen coordinates.</param>
		/// <returns>A PointF in world coordinates.</returns>
		Drawing::PointF ScreenToWorld(int x, int y)
		{ 
			return Drawing::PointF((float)(x - ClientRectangle.Width / 2) * mZoomFactor + mCameraPosition.X, 
								  -(float)(y - ClientRectangle.Height / 2) * mZoomFactor + mCameraPosition.Y);
		}
		/// <summary>
		/// Converts the given point from screen coordinates to world coordinates.
		/// </summary>
		/// <param name="pt">Location in screen coordinates.</param>
		/// <returns>A PointF in world coordinates.</returns>
		Drawing::PointF ScreenToWorld(Drawing::Point pt) { return ScreenToWorld(pt.X, pt.Y); }
		/// <summary>
		/// Converts the given vector from screen coordinates to world coordinates.
		/// </summary>
		/// <param name="sz">Size in screen coordinates.</param>
		/// <returns>A SizeF in world coordinates.</returns>
		Drawing::SizeF ScreenToWorld(Drawing::Size sz)
		{
			Drawing::PointF pt1 = ScreenToWorld(0, 0);
			Drawing::PointF pt2 = ScreenToWorld(sz.Width, sz.Height);
			return Drawing::SizeF(pt2.X - pt1.X, pt2.Y - pt1.Y);
		}
		/// <summary>
		/// Returns the coordinates of the viewport in world coordinates.
		/// </summary>
		Drawing::RectangleF GetViewPort()
		{ 
			Drawing::PointF bl = ScreenToWorld(ClientRectangle.Left, ClientRectangle.Bottom);
			Drawing::PointF tr = ScreenToWorld(ClientRectangle.Right, ClientRectangle.Top);
			return Drawing::RectangleF(bl.X, bl.Y, tr.X - bl.X, tr.Y - bl.Y);
		}
		/// <summary>
		/// Sets the viewport to the given model coordinates.
		/// </summary>
		/// <param name="x1">X coordinate of the bottom left corner of the viewport in model coordinates.</param>
		/// <param name="y1">X coordinate of the bottom left corner of the viewport in model coordinates.</param>
		/// <param name="x2">X coordinate of the top right corner of the viewport in model coordinates.</param>
		/// <param name="y2">X coordinate of the top right corner of the viewport in model coordinates.</param>
		System::Void SetView(float x1, float y1, float x2, float y2)
		{ 
			float h = Math::Abs(y1 - y2);
			float w = Math::Abs(x1 - x2);
			mCameraPosition = PointF((x1 + x2) / 2, (y1 + y2) / 2);
			if ((ClientRectangle.Height != 0) && (ClientRectangle.Width != 0))
				mZoomFactor = Math::Max(h / (float)(ClientRectangle.Height), w / (float)(ClientRectangle.Width));
			else
				mZoomFactor = 1;
		}


		/// <summary>
		/// Sets the viewport to the drawing extents.
		/// </summary>
		System::Void SetView()
		{ 
			if (mLimits.IsEmpty)
				SetView(-250, -250, 250, 250);
			else
			{
				Drawing::RectangleF vp = Drawing::RectangleF::Inflate(mLimits, mLimits.Width / 20, mLimits.Height / 20);
				SetView(vp.X, vp.Y, vp.X + vp.Width, vp.Y + vp.Height);
			}
		}
		
		/// <summary>
		/// Sets the viewport to the client area of the control.
		/// </summary>
		System::Void ResetViewport();

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
			System::Windows::Forms::CreateParams ^ get(void) override sealed
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
		virtual void OnPaint(System::Windows::Forms::PaintEventArgs^  e) override sealed;
		virtual void OnPaintBackground(System::Windows::Forms::PaintEventArgs^  e) override sealed;
	
	protected:
		/// <summary>
		/// Raises the GLCanvas2D.Render event.
		/// </summary>
		virtual void OnRender(GLCanvas::Canvas2DRenderEventArgs^ e)
		{
			Render(this, e);
		}
		/// <summary>
		/// Raises the GLCanvas2D.RenderDone event.
		/// </summary>
		virtual void OnRenderDone(System::EventArgs^ e)
		{
			RenderDone(this, e);
		}
		/// <summary>
		/// Raises the GLCanvas2D.MouseSelect event.
		/// </summary>
		virtual void OnMouseSelect(GLCanvas::Canvas2DMouseSelectEventArgs^ e)
		{
			MouseSelect(this, e);
		}

	// Event delegates
	public:
		/// <summary>
		/// Represents the method that will handle the Render event. 
		/// </summary>
		/// <param name="sender">The GLCanvas2D object that is the source of the event.</param>
		/// <param name="e">A Canvas2DRenderEventArgs that contains event data.</param>
		[EditorBrowsable(EditorBrowsableState::Never)]
		delegate void RenderHandler(System::Object^ sender, GLCanvas::Canvas2DRenderEventArgs^ e);
		/// <summary>
		/// Represents the method that will handle the RenderDone event. 
		/// </summary>
		/// <param name="sender">The GLCanvas2D object that is the source of the event.</param>
		/// <param name="e">A System.EventArgs that contains event data.</param>
		[EditorBrowsable(EditorBrowsableState::Never)]
		delegate void RenderDoneHandler(System::Object^ sender, System::EventArgs^ e);
		/// <summary>
		/// Represents the method that will handle the MouseSelect event. 
		/// </summary>
		/// <param name="sender">The GLCanvas2D object that is the source of the event.</param>
		/// <param name="e">A Canvas2DMouseSelectEventArgs that contains event data.</param>
		[EditorBrowsable(EditorBrowsableState::Never)]
		delegate void MouseSelectHandler(System::Object^ sender, GLCanvas::Canvas2DMouseSelectEventArgs^ e);

	// Events
	public:
		/// <summary>
		/// Occurs when the control is redrawn.
		/// </summary>
		[Category("Appearance"), Browsable(true), Description("Occurs when the control is redrawn.")] 
		event RenderHandler^ Render;
		/// <summary>
		/// Occurs after the control is redrawn.
		/// </summary>
		[Category("Appearance"), Browsable(true), Description("Occurs after the control is redrawn.")] 
		event RenderDoneHandler^ RenderDone;
		/// <summary>
		/// Occurs when the user selects a region with the mouse.
		/// </summary>
		[Category("Mouse"), Browsable(true), Description("Occurs when the user selects a region with the mouse.")] 
		event MouseSelectHandler^ MouseSelect;

	private: 
		System::Void InitializeComponent() 
		{
			 this->Name = L"GLCanvas2D";
		}
};

}
