#include "stdafx.h"
#include "GLGraphics2D.h"
#include "GLCanvas2D.h"
#include <Vcclr.h>

namespace GLCanvas
{
	GLGraphics2D::GLGraphics2D(GLCanvas2D ^ Canvas, Drawing::Graphics ^ GDIGraphics)
	{
		mCanvas = Canvas; 
		mGDIGraphics = GDIGraphics;
		LineWidth = 1.0f;
		mZ = -0.9f;
		mInit = false;
		mTriangles = gcnew GLVertexArray(GL_TRIANGLES);
		mLines = gcnew GLVertexArray(GL_LINES);
		mTexts = gcnew System::Collections::Generic::List<GLTextParam>;
		mView = Canvas->GetViewPort();
	}

	Drawing::RectangleF GLGraphics2D::Render()
	{		
		// Render drawing objects
		mTriangles->Render();
		mLines->Render();

		// Draw text objects
		for each (GLTextParam tp in mTexts)
		{
			// Position the text
			glLoadIdentity();
			glColor4ub(tp.color.R, tp.color.G, tp.color.B, tp.color.A);
			if (tp.vectortext)
			{
				glListBase(mCanvas->VectorListBase);
				glTranslatef(tp.x, tp.y, mZ);
				glScalef(tp.height, tp.height, tp.height);
			}
			else
			{
				glListBase(mCanvas->RasterListBase);
				glRasterPos2f(tp.x, tp.y);
			}
			// Draw the text
			System::IntPtr str = System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(tp.text);
			glCallLists(tp.text->Length, GL_UNSIGNED_BYTE, 
				(GLvoid*)str.ToPointer());
			System::Runtime::InteropServices::Marshal::FreeHGlobal(str);
			UpdateDepth();
		}
		glLoadIdentity();

		// Clear arrays
		mTriangles->Clear();
		mLines->Clear();
		mTexts->Clear();

		// Set depth
		mZ = -0.9f;

		return Drawing::RectangleF(mBL.X, mBL.Y, mTR.X - mBL.X, mTR.Y - mBL.Y);
	}

	int GLGraphics2D::GetCirclePrecision(float featureSize)
	{
		// Measure the pixel size of the feature
		int pts = mCanvas->WorldToScreen(Drawing::SizeF(featureSize, 0.0f)).Width;				
		// Try to represent curved features by at most 4 pixels.
		return (int)((float)Math::Sqrt(pts) * 3.0f) + 4;
	}

	System::Void GLGraphics2D::DrawRasterText(float x, float y, System::String ^ text, Drawing::Color color)
	{
		mTexts->Add(GLTextParam(x, y, 0.0f, text, color, false));
	}

	System::Void GLGraphics2D::DrawVectorText(float x, float y, float height, System::String ^ text, Drawing::Color color)
	{
		mTexts->Add(GLTextParam(x, y, height, text, color, true));
	}

	System::Void GLGraphics2D::DrawLine(float x1, float y1, float x2, float y2, Drawing::Color color)
	{
		// Check intersections
		Drawing::RectangleF lRect(Math::Min(x1, x2), Math::Min(y1, y2), Math::Abs(x1 - x2), Math::Abs(y1 - y2));
		if (mView.IntersectsWith(lRect))
		{
			mLines->AddVertex(x1, y1, mZ, color);
			mLines->AddVertex(x2, y2, mZ, color);
			UpdateDepth();
		}
		UpdateLimits(x1, y1);
		UpdateLimits(x2, y2);
	}

	System::Void GLGraphics2D::DrawLine(float x1, float y1, float x2, float y2, float thickness, Drawing::Color color)
	{
		// Check intersections
		Drawing::RectangleF lRect(Math::Min(x1, x2), Math::Min(y1, y2), Math::Abs(x1 - x2), Math::Abs(y1 - y2));
		if (mView.IntersectsWith(lRect))
		{
			float angle = (float)Math::Atan2(y2 - y1, x2 - x1);
			float t2sina = thickness / 2 * (float)Math::Sin(angle);
			float t2cosa = thickness / 2 * (float)Math::Cos(angle);
			mTriangles->AddVertex(x1 + t2sina, y1 - t2cosa, mZ, color);
			mTriangles->AddVertex(x2 + t2sina, y2 - t2cosa, mZ, color);
			mTriangles->AddVertex(x2 - t2sina, y2 + t2cosa, mZ, color);
			mTriangles->AddVertex(x2 - t2sina, y2 + t2cosa, mZ, color);
			mTriangles->AddVertex(x1 - t2sina, y1 + t2cosa, mZ, color);
			mTriangles->AddVertex(x1 + t2sina, y1 - t2cosa, mZ, color);
			UpdateDepth();
		}
		UpdateLimits(x1, y1);
		UpdateLimits(x2, y2);
	}

	System::Void GLGraphics2D::DrawLine(float x1, float y1, float x2, float y2, float startthickness, float endthickness, Drawing::Color color)
	{
		// Check intersections
		Drawing::RectangleF lRect(Math::Min(x1, x2), Math::Min(y1, y2), Math::Abs(x1 - x2), Math::Abs(y1 - y2));
		if (mView.IntersectsWith(lRect))
		{
			float angle = (float)Math::Atan2(y2 - y1, x2 - x1);
			float t2sina1 = startthickness / 2 * (float)Math::Sin(angle);
			float t2cosa1 = startthickness / 2 * (float)Math::Cos(angle);
			float t2sina2 = endthickness / 2 * (float)Math::Sin(angle);
			float t2cosa2 = endthickness / 2 * (float)Math::Cos(angle);
			mTriangles->AddVertex(x1 + t2sina1, y1 - t2cosa1, mZ, color);
			mTriangles->AddVertex(x2 + t2sina2, y2 - t2cosa2, mZ, color);
			mTriangles->AddVertex(x2 - t2sina2, y2 + t2cosa2, mZ, color);
			mTriangles->AddVertex(x2 - t2sina2, y2 + t2cosa2, mZ, color);
			mTriangles->AddVertex(x1 - t2sina1, y1 + t2cosa1, mZ, color);
			mTriangles->AddVertex(x1 + t2sina1, y1 - t2cosa1, mZ, color);
			UpdateDepth();
		}
		UpdateLimits(x1, y1);
		UpdateLimits(x2, y2);
	}

	System::Void GLGraphics2D::DrawArc(float x, float y, float width, float height, float startAngle, float sweepAngle, Drawing::Color color) 
	{
		bool check = mView.IntersectsWith(Drawing::RectangleF(x - width / 2, y - height / 2, width, height));
		float da = sweepAngle / (float)GetCirclePrecision(Math::Max(width, height));
		for (float a = startAngle; a < startAngle + sweepAngle; a += da)
		{
			float xv = x + width / 2 * (float)Math::Cos(a);
			float yv = y + height / 2 * (float)Math::Sin(a);
			float xend = x + width / 2 * (float)Math::Cos(a + da);
			float yend = y + height / 2 * (float)Math::Sin(a + da);
			UpdateLimits(xv, yv);
			UpdateLimits(xend, yend);

			if (check)
			{
				mLines->AddVertex(xv, yv, mZ, color);
				mLines->AddVertex(xend, yend, mZ, color);
			}
		}

		UpdateDepth();
	}

	System::Void GLGraphics2D::FillPie(float x, float y, float width, float height, float startAngle, float sweepAngle, Drawing::Color color) 
	{ 
		bool check = mView.IntersectsWith(Drawing::RectangleF(x - width / 2, y - height / 2, width, height));
		float da = sweepAngle / (float)GetCirclePrecision(Math::Max(width, height));
		for (float a = startAngle; a < startAngle + sweepAngle; a += da)
		{
			float xv = x + width / 2 * (float)Math::Cos(a);
			float yv = y + height / 2 * (float)Math::Sin(a);
			float xend = x + width / 2 * (float)Math::Cos(a + da);
			float yend = y + height / 2 * (float)Math::Sin(a + da);
			UpdateLimits(xv, yv);
			UpdateLimits(xend, yend);

			if (check)
			{
				mTriangles->AddVertex(x, y, mZ, color);
				mTriangles->AddVertex(xv, yv, mZ, color);
				mTriangles->AddVertex(xend, yend, mZ, color);
			}
		}

		UpdateDepth();
	}

	System::Void GLGraphics2D::DrawTriangle(float x1, float y1, float x2, float y2,float x3,float y3, Drawing::Color color)
	{
		float xmin = Math::Min(Math::Min(x1, x2), x3);
		float ymin = Math::Min(Math::Min(y1, y2), y3);
		float xmax = Math::Max(Math::Max(x1, x2), x3);
		float ymax = Math::Max(Math::Max(y1, y2), y3);
		Drawing::RectangleF lRect(xmin, ymin, xmax - xmin, ymax - ymin);
		if (mView.IntersectsWith(lRect))
		{
			mLines->AddVertex(x1, y1, mZ, color);
			mLines->AddVertex(x2, y2, mZ, color);

			mLines->AddVertex(x2, y2, mZ, color);
			mLines->AddVertex(x3, y3, mZ, color);

			mLines->AddVertex(x3, y3, mZ, color);
			mLines->AddVertex(x1, y1, mZ, color);

			UpdateDepth();
		}

		UpdateLimits(x1, y1);
		UpdateLimits(x2, y2);
		UpdateLimits(x3, y3);
	}

	System::Void GLGraphics2D::DrawRectangle(float x1, float y1, float x2, float y2, Drawing::Color color) 
	{ 
		bool check = mView.IntersectsWith(Drawing::RectangleF(Math::Min(x1, x2), Math::Min(y1, y2), Math::Abs(x1 - x2), Math::Abs(y1 - y2)));

		if (check)
		{
			mLines->AddVertex(x1, y1, mZ, color);
			mLines->AddVertex(x2, y1, mZ, color);

			mLines->AddVertex(x2, y1, mZ, color);
			mLines->AddVertex(x2, y2, mZ, color);

			mLines->AddVertex(x2, y2, mZ, color);
			mLines->AddVertex(x1, y2, mZ, color);

			mLines->AddVertex(x1, y2, mZ, color);
			mLines->AddVertex(x1, y1, mZ, color);

			UpdateDepth();
		}

		UpdateLimits(x1, y1);
		UpdateLimits(x2, y2);
	}

	System::Void GLGraphics2D::DrawRoundedRectangle(float x1, float y1, float x2, float y2, float rx, float ry, Drawing::Color color)
	{
		DrawLine(x1 + rx, y1, x2 - rx, y1, color);
		DrawLine(x1 + rx, y2, x2 - rx, y2, color);
		DrawLine(x1, y1 + ry, x1, y2 - ry, color);
		DrawLine(x2, y1 + ry, x2, y2 - ry, color);

	    int prec = GetCirclePrecision(Math::Max(rx, ry) * 2.0f);
		// make precision divisable by 4, this is neccessary for the
		// loop variable to detect the offset change
		prec = (prec | 3) + 1;
		float da = 2.0f * (float)Math::PI / (float)prec;	

		float x = x2 - rx;	// offset ellipse to first corner (upper right)
		float y = y2 - ry;
		for(int loop = 0; loop < prec; loop++)
		{
			float a = da * (float)loop;
			float xv = x + rx * (float)Math::Cos(a);
			float yv = y + ry * (float)Math::Sin(a);
			float xend = x + rx * (float)Math::Cos(a + da);
			float yend = y + ry * (float)Math::Sin(a + da);

			mLines->AddVertex(xv, yv, mZ, color);
			mLines->AddVertex(xend, yend, mZ, color);

			if (loop == (prec / 4) * 1 - 1) x = x1 + rx;		// offset ellipse to second corner (upper left)
			if (loop == (prec / 4) * 2 - 1) y = y1 + ry;		// offset ellipse to third corner (lower left)
			if (loop == (prec / 4) * 3 - 1) x = x2 - rx;		// offset ellipse to fourth corner (lower right)
		}
		UpdateDepth();

		UpdateLimits(x1, y1);
		UpdateLimits(x2, y2);
	}

	System::Void GLGraphics2D::FillRoundedRectangle(float x1, float y1, float x2, float y2, float rx, float ry, Drawing::Color color)
	{
		FillRectangle(x1, y1 + ry, x2, y2 - ry, color);			// center
		FillRectangle(x1 + rx, y2 - ry, x2 - rx, y2, color);	// top
		FillRectangle(x1 + rx, y1, x2 - rx, y1 + ry, color);	// bottom

	    int prec = GetCirclePrecision(Math::Max(rx, ry) * 2.0f);
		// make precision divisable by 4, this is neccessary for the
		// loop variable to detect the offset change
		prec = (prec | 3) + 1;
		float da = 2.0f * (float)Math::PI / (float)prec;	

		float x = x2 - rx;	// offset ellipse to first corner (upper right)
		float y = y2 - ry;
		for(int loop = 0; loop < prec; loop++)
		{
			float a = da * (float)loop;
			float xv = x + rx * (float)Math::Cos(a);
			float yv = y + ry * (float)Math::Sin(a);
			float xend = x + rx * (float)Math::Cos(a + da);
			float yend = y + ry * (float)Math::Sin(a + da);

			mTriangles->AddVertex(x, y, mZ, color);
			mTriangles->AddVertex(xv, yv, mZ, color);
			mTriangles->AddVertex(xend, yend, mZ, color);

			if (loop == (prec / 4) * 1 - 1) x = x1 + rx;		// offset ellipse to second corner (upper left)
			if (loop == (prec / 4) * 2 - 1) y = y1 + ry;		// offset ellipse to third corner (lower left)
			if (loop == (prec / 4) * 3 - 1) x = x2 - rx;		// offset ellipse to fourth corner (lower right)
		}
		UpdateDepth();

		UpdateLimits(x1, y1);
		UpdateLimits(x2, y2);
	}

	System::Void GLGraphics2D::FillTriangle(float x1, float y1, float x2, float y2,float x3,float y3, Drawing::Color color)
	{
		float xmin = Math::Min(Math::Min(x1, x2), x3);
		float ymin = Math::Min(Math::Min(y1, y2), y3);
		float xmax = Math::Max(Math::Max(x1, x2), x3);
		float ymax = Math::Max(Math::Max(y1, y2), y3);
		Drawing::RectangleF lRect(xmin, ymin, xmax - xmin, ymax - ymin);
		if (mView.IntersectsWith(lRect))
		{
			mTriangles->AddVertex(x1, y1, mZ, color);
			mTriangles->AddVertex(x2, y2, mZ, color);
			mTriangles->AddVertex(x3, y3, mZ, color);

			UpdateDepth();
		}

		UpdateLimits(x1, y1);
		UpdateLimits(x2, y2);
		UpdateLimits(x3, y3);
	}

	System::Void GLGraphics2D::FillRectangle(float x1, float y1, float x2, float y2, Drawing::Color color) 
	{ 
		bool check = mView.IntersectsWith(Drawing::RectangleF(Math::Min(x1, x2), Math::Min(y1, y2), Math::Abs(x1 - x2), Math::Abs(y1 - y2)));

		if (check)
		{
			mTriangles->AddVertex(x1, y1, mZ, color);
			mTriangles->AddVertex(x2, y1, mZ, color);
			mTriangles->AddVertex(x2, y2, mZ, color);

			mTriangles->AddVertex(x2, y2, mZ, color);
			mTriangles->AddVertex(x1, y2, mZ, color);
			mTriangles->AddVertex(x1, y1, mZ, color);

			UpdateDepth();
		}

		UpdateLimits(x1, y1);
		UpdateLimits(x2, y2);
	}

	System::Void GLGraphics2D::DrawEllipse(float x, float y, float width, float height, Drawing::Color color) 
	{ 
		bool check = mView.IntersectsWith(Drawing::RectangleF(x - width / 2, y - height / 2, width, height));
		if (check)
		{
			float da = 2.0f * (float)Math::PI / (float)GetCirclePrecision(Math::Max(width, height));
			for (float a = 0; a <= 2.0f * (float)Math::PI; a += da)
			{
				float xv = x + width / 2 * (float)Math::Cos(a);
				float yv = y + height / 2 * (float)Math::Sin(a);
				float xend = x + width / 2 * (float)Math::Cos(a + da);
				float yend = y + height / 2 * (float)Math::Sin(a + da);

				mLines->AddVertex(xv, yv, mZ, color);
				mLines->AddVertex(xend, yend, mZ, color);
			}
			UpdateDepth();
		}

		UpdateLimits(x - width / 2.0f, y - height / 2.0f);
		UpdateLimits(x + width / 2.0f, y + height / 2.0f);
	}

	System::Void GLGraphics2D::FillEllipse(float x, float y, float width, float height, Drawing::Color color) 
	{ 
		bool check = mView.IntersectsWith(Drawing::RectangleF(x - width / 2, y - height / 2, width, height));
		if (check)
		{
			float da = 2.0f * (float)Math::PI / (float)GetCirclePrecision(Math::Max(width, height));
			for (float a = 0; a <= 2.0f * (float)Math::PI; a += 2.0f * (float)Math::PI / (float)GetCirclePrecision(Math::Max(width, height)))
			{
				float xv = x + width / 2 * (float)Math::Cos(a);
				float yv = y + height / 2 * (float)Math::Sin(a);
				float xend = x + width / 2 * (float)Math::Cos(a + da);
				float yend = y + height / 2 * (float)Math::Sin(a + da);

				mTriangles->AddVertex(x, y, mZ, color);
				mTriangles->AddVertex(xv, yv, mZ, color);
				mTriangles->AddVertex(xend, yend, mZ, color);
			}
			UpdateDepth();
		}

		UpdateLimits(x - width / 2.0f, y - height / 2.0f);
		UpdateLimits(x + width / 2.0f, y + height / 2.0f);
	}

	System::Void GLGraphics2D::DrawPolygon(array<Drawing::PointF, 1> ^ points, Drawing::Color color) 
	{
		if (points->Length < 2) return;

		for (int i = 0; i <= points->Length - 1; i++)
		{
			int j = (i == points->Length - 1 ? 0 : i + 1);
			mLines->AddVertex(points[i].X, points[i].Y, mZ, color);
			mLines->AddVertex(points[j].X, points[j].Y, mZ, color);
			UpdateLimits(points[i].X, points[i].Y);
			UpdateLimits(points[j].X, points[j].Y);
		}
		UpdateDepth();
	}

	System::Void GLGraphics2D::FillPolygon(array<Drawing::PointF, 1> ^ points, Drawing::Color color) 
	{ 
		if (points->Length < 3) return;

		// Calculate center coordinates. Polygons are drawn as triangle fans sharing this point.
		// This is why only concave polygons are supported.
		float x = 0;
		float y = 0;
		for (int i = 0; i < points->Length; i++)
		{
			x += points[i].X;
			y += points[i].Y;
			UpdateLimits(points[i].X, points[i].Y);
		}
		x /= (float)points->Length;
		y /= (float)points->Length;

		for (int i = 0; i <= points->Length - 1; i++)
		{
			int j = (i == points->Length - 1 ? 0 : i + 1);
			mTriangles->AddVertex(x, y, mZ, color);
			mTriangles->AddVertex(points[i].X, points[i].Y, mZ, color);
			mTriangles->AddVertex(points[j].X, points[j].Y, mZ, color);
		}
		UpdateDepth();
	}

	Drawing::SizeF GLGraphics2D::MeasureString(System::String ^ text)
	{
		Drawing::SizeF szf = mGDIGraphics->MeasureString(text, mCanvas->Font);
		Drawing::SizeF sz = mCanvas->ScreenToWorld(Drawing::Size((int)szf.Width, (int)szf.Height));
		if (sz.Width < 0) sz.Width = -sz.Width;
		if (sz.Height < 0) sz.Height = -sz.Height;
		return sz;
	}

}