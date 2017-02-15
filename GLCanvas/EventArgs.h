#pragma once

using namespace System;

namespace GLCanvas {
	// Forward class declarations
	ref class GLGraphics2D;
	ref class GLGraphics3D;

	/// <summary>
	/// Provides data for the GLCanvas2D.Render event.
	/// </summary>
	[System::Runtime::InteropServices::ComVisibleAttribute(true)]
	public ref class Canvas2DRenderEventArgs : public System::EventArgs
	{
	// Member variables
	private:
		GLCanvas::GLGraphics2D^ mGraphics;

	// Properties
	public:
		/// <summary>
		/// Gets the graphics used to paint.
		/// </summary>
		property GLCanvas::GLGraphics2D^ Graphics
		{
			virtual GLCanvas::GLGraphics2D^ get(void) { return mGraphics; }
		}

	// Constructor/destructor
	public:
		/// <summary>
		/// Initializes a new instance of the Canvas2DRenderEventArgs class.
		/// </summary>
		/// <param name="graphics">The graphics used to paint.</param>
		Canvas2DRenderEventArgs(GLCanvas::GLGraphics2D^ graphics)
		{
			mGraphics = graphics;
		}
	};

	/// <summary>
	/// Provides data for the GLCanvas2D.MouseSelect event.
	/// </summary>
	[System::Runtime::InteropServices::ComVisibleAttribute(true)]
	public ref class Canvas2DMouseSelectEventArgs : public System::EventArgs
	{
	// Member variables
	private:
		Drawing::Point mPoint1, mPoint2;
		System::Windows::Forms::MouseButtons mButton;

	// Properties
	public:
		/// <summary>
		/// Gets the first point picked by the user.
		/// </summary>
		property Drawing::Point Point1
		{
			virtual Drawing::Point get(void) { return mPoint1; }
		}
		/// <summary>
		/// Gets the second point picked by the user.
		/// </summary>
		property Drawing::Point Point2
		{
			virtual Drawing::Point get(void) { return mPoint2; }
		}
		/// <summary>
		/// Gets the state of mouse buttons.
		/// </summary>
		property System::Windows::Forms::MouseButtons Button
		{
			virtual System::Windows::Forms::MouseButtons get(void) { return mButton; }
		}

	// Constructor/destructor
	public:
		/// <summary>
		/// Initializes a new instance of the Canvas2DMouseSelectEventArgs class.
		/// </summary>
		/// <param name="point1">First point picked by the user.</param>
		/// <param name="point2">Seond point picked by the user.</param>
		/// <param name="button">State of mouse buttons.</param>
		Canvas2DMouseSelectEventArgs(Drawing::Point point1, Drawing::Point point2, System::Windows::Forms::MouseButtons button)
		{
			mPoint1 = point1;
			mPoint2 = point2;
			mButton = button;
		}
	};

	/// <summary>
	/// Provides data for the GLCanvas3D.Render event.
	/// </summary>
	[System::Runtime::InteropServices::ComVisibleAttribute(true)]
	public ref class Canvas3DRenderEventArgs : public System::EventArgs
	{
	// Member variables
	private:
		GLCanvas::GLGraphics3D^ mGraphics;

	// Properties
	public:
		/// <summary>
		/// Gets the graphics used to paint.
		/// </summary>
		property GLCanvas::GLGraphics3D^ Graphics
		{
			virtual GLCanvas::GLGraphics3D^ get(void) { return mGraphics; }
		}

	// Constructor/destructor
	public:
		/// <summary>
		/// Initializes a new instance of the Canvas3DRenderEventArgs class.
		/// </summary>
		/// <param name="graphics">The graphics used to paint.</param>
		Canvas3DRenderEventArgs(GLCanvas::GLGraphics3D^ graphics)
		{
			mGraphics = graphics;
		}
	};

	/// <summary>
	/// Provides data for the GLCanvas3D.MouseSelect event.
	/// </summary>
	[System::Runtime::InteropServices::ComVisibleAttribute(true)]
	public ref class Canvas3DMouseSelectEventArgs : public System::EventArgs
	{
	// Member variables
	private:
		bool mReverse;
		Drawing::Point mPoint1, mPoint2;
		System::Windows::Forms::MouseButtons mButton;

	// Properties
	public:
		/// <summary>
		/// Gets the first point picked by the user.
		/// </summary>
		property Drawing::Point Point1
		{
			virtual Drawing::Point get(void) { return mPoint1; }
		}
		/// <summary>
		/// Gets the second point picked by the user.
		/// </summary>
		property Drawing::Point Point2
		{
			virtual Drawing::Point get(void) { return mPoint2; }
		}
		/// <summary>
		/// Gets the state of mouse buttons.
		/// </summary>
		property System::Windows::Forms::MouseButtons Button
		{
			virtual System::Windows::Forms::MouseButtons get(void) { return mButton; }
		}
		/// <summary>
		/// Gets whether the selection was made right to left.
		/// </summary>
		property bool Reverse
		{
			virtual bool get(void) { return mReverse; }
		}

	// Constructor/destructor
	public:
		/// <summary>
		/// Initializes a new instance of the Canvas3DMouseSelectEventArgs class.
		/// </summary>
		/// <param name="point1">First point picked by the user.</param>
		/// <param name="point2">Seond point picked by the user.</param>
		/// <param name="button">State of mouse buttons.</param>
		/// <param name="reverse">Whether the selection was made right to left.</param>
		Canvas3DMouseSelectEventArgs(Drawing::Point point1, Drawing::Point point2, System::Windows::Forms::MouseButtons button, bool reverse)
		{
			mPoint1 = point1;
			mPoint2 = point2;
			mButton = button;
			mReverse = reverse;
		}

		/// <summary>
		/// Initializes a new instance of the Canvas3DMouseSelectEventArgs class.
		/// </summary>
		/// <param name="point1">First point picked by the user.</param>
		/// <param name="point2">Seond point picked by the user.</param>
		/// <param name="button">State of mouse buttons.</param>
		Canvas3DMouseSelectEventArgs(Drawing::Point point1, Drawing::Point point2, System::Windows::Forms::MouseButtons button)
		{
			mPoint1 = point1;
			mPoint2 = point2;
			mButton = button;
			mReverse = false;
		}
	};
}