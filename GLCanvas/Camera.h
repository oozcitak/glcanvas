#pragma once

using namespace System;

#include "Utility.h"
#include "Point3D.h"

namespace GLCanvas
{
	/// <summary>
	/// Represents a target camera.
	/// </summary>
	public ref class Camera sealed
	{
	// Properties
	public:
		/// <summary>
		/// Gets or sets the camera position.
		/// </summary>		
		property Point3D Position;
		/// <summary>
		/// Gets or sets the camera target.
		/// </summary>		
		property Point3D Target;
		/// <summary>
		/// Gets the camera up direction.
		/// </summary>
		property Point3D Up
		{
			virtual Point3D get(void) 
			{ 
				float pitch = (this->Pitch + 90) * (float)Math::PI / 180.0f;
				float yaw = this->Yaw * (float)Math::PI / 180.0f;
				float x = 1.0f * (float)Math::Cos(pitch) * (float)Math::Cos(yaw);
				float y = 1.0f * (float)Math::Cos(pitch) * (float)Math::Sin(yaw);
				float z = 1.0f * (float)Math::Sin(pitch);
				return Point3D(x, y, z);
			}
		}
		/// <summary>
		/// Gets or sets the camera pitch angle in degrees.
		/// </summary>
		property float Pitch
		{
			virtual float get(void) 
			{ 
				double hdist = Math::Sqrt((Position.X - Target.X) * (Position.X - Target.X) + (Position.Y - Target.Y) * (Position.Y - Target.Y));
				return (float)(Math::Atan2(Position.Z - Target.Z, hdist) * 180.0 / Math::PI);
			}
			virtual void set(float value) 
			{
				UpdatePosition(value, this->Yaw, this->Distance);
			}
		}
		/// <summary>
		/// Gets or sets the camera yaw angle in degrees.
		/// </summary>
		property float Yaw
		{
			virtual float get(void) 
			{ 
				return (float)(Math::Atan2(Position.Y - Target.Y, Position.X - Target.X) * 180.0 / Math::PI);
			}
			virtual void set(float value) 
			{
				UpdatePosition(this->Pitch, value, this->Distance);
			}
		}
		/// <summary>
		/// Gets or sets the distance to camera target.
		/// </summary>
		property float Distance
		{
			virtual float get(void) 
			{ 
				return (float)Math::Sqrt((Position.X - Target.X) * (Position.X - Target.X) + (Position.Y - Target.Y) * (Position.Y - Target.Y) + (Position.Z - Target.Z) * (Position.Z - Target.Z));
			}
			virtual void set(float value) 
			{
				UpdatePosition(this->Pitch, this->Yaw, value);
			}
		}

	// Public methods
	public:
		/// <summary>
		/// Sets camera position and target.
		/// </summary>
		/// <param name="position">Camera position.</param>
		/// <param name="target">Camera target.</param>
		void Set(Point3D position, Point3D target)
		{
			Position = position;
			Target = target;
		}
		/// <summary>
		/// Pans the camera.
		/// </summary>
		/// <param name="dx">x offset from current position.</param>
		/// <param name="dy">y offset from current position.</param>
		/// <param name="dz">z offset from current position.</param>
		void Pan(float dx, float dy, float dz)
		{
			Target = Point3D(Target.X + dx, Target.Y + dy, Target.Z + dz);
			Position = Point3D(Position.X + dx, Position.Y + dy, Position.Z + dz);
		}
		/// <summary>
		/// Pans the camera.
		/// </summary>
		/// <param name="oldPosition">old position.</param>
		/// <param name="newPosition">new position.</param>
		void Pan(Point3D oldPosition, Point3D newPosition)
		{
			Pan(newPosition.X - oldPosition.X, newPosition.Y - oldPosition.Y, newPosition.Z - oldPosition.Z);
		}

		virtual System::String ^ ToString() override
		{
			return String::Format("{0} <- {1} (Pitch: {2:0.00} Yaw: {3:0.00})", 
				Target, Position, Pitch, Yaw);
		}

	// Helpers
	protected:
		/// <summary>
		/// Updates the camera position.
		/// </summary>
		/// <param name="pitch">New pitch angle in degrees.</param>
		/// <param name="yaw">New yaw angle in degrees.</param>
		/// <param name="distance">New distance to camera target.</param>
		void UpdatePosition(float pitch, float yaw, float distance)
		{
			if(pitch > 85.0f) pitch = 85.0f;
			if(pitch < -85.0f) pitch = -85.0f;
			pitch = pitch * (float)Math::PI / 180.0f;
			yaw = yaw * (float)Math::PI / 180.0f;
			float x = Target.X + distance * (float)Math::Cos(pitch) * (float)Math::Cos(yaw);
			float y = Target.Y +distance * (float)Math::Cos(pitch) * (float)Math::Sin(yaw);
			float z = Target.Z +distance * (float)Math::Sin(pitch);
			Position = Point3D(x, y, z);
		}

	// Constructor
	private:
		Camera() { }
	public:
		/// <summary>
		/// Initializes a new instance of the Camera class.
		/// </summary>
		/// <param name="position">Camera position.</param>
		/// <param name="target">Camera target.</param>
		Camera(Point3D position, Point3D target)
		{
			Position = position;
			Target = target;
		}
	};
}