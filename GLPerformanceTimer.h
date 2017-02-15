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

using namespace System;

namespace GLView {

	/// <summary>
	/// Represents a performance timer with 1 ms resolution.
	/// </summary>
	ref class GLPerformanceTimer
	{
	// Member variables
	private:
		unsigned long mStartTime, mStopTime, mDelta;
		bool mStarted;

	// Constructors
	public:
		GLPerformanceTimer()
		{
			mStarted = false;
			Start();
			mDelta = Stop();
		}

		GLPerformanceTimer(bool StartTimer)
		{
			mStarted = false;
			Start();
			mDelta = Stop();
			if (StartTimer) Start();
		}

	public:
		/// <summary>
		/// Starts the timer.
		/// </summary>
		inline void Start(void) 
		{
			mStartTime = GetTickCount();
			mStarted = true;
		}

		/// <summary>
		/// Stops the timer and returns the elapsed time.
		/// </summary>
		/// <returns>Elapsed time in ticks</returns>
		inline unsigned long Stop() 
		{
			if (!mStarted) throw gcnew Exception(L"Timer is not running. GLPerformanceTimer.Stop() called before GLPerformanceTimer.Start().");
			mStarted = false;
			mStopTime = GetTickCount();
			unsigned long mDuration = (mStopTime - mStartTime);
			if (mDuration < mDelta)
				return 0;
			else
				return (mDuration - mDelta);
		}
	};

}