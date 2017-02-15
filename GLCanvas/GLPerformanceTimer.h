#pragma once

using namespace System;

namespace GLCanvas {

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