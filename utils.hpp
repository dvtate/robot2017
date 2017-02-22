#ifndef FRC4941_UTILS_HPP
#define FRC4941_UTILS_HPP

#include "WPILib.h"
#include <math.h>
#include <time.h>

/// a few useful functions for the robot
namespace utils {

	/// remove 'ghost-input' resulting from inaccurate joysticks
	inline double removeGhost(const double val)
		{ return (val > 0.15 || val < -0.15) ? val : 0.0; }


	/// a linear approach to preventing brownout (untested)
	// could probably be combined with expReduceBrounout to improve effectiveness
	float linReduceBrownout(const float limit, const float current, float& past)
	{
		/// limit = maximum amount of change per cycle
		/// current = the most recent value coming from input
		/// past = the value returned by this function in the last frame

		// null or ghost input doesn't affect robot (also good for breaking)
		if (utils::removeGhost(current) == 0.0f) return 0.0f;

		float change = current - past;

		if (change > 0) { // increase speed
			if (change > limit) // too much change
				return (past += limit);
			// nominal change
			return (past = current);

		} else { // decrease speed
			if (change < -limit) // too much change
				return (past -= limit);
			// nominal change
			return (past = current);
		}
	}

	/// plots input on a curve to make driving different
	inline double unsignedSqrt(const double val)
		{ return val > 0 ? sqrt(val) : -sqrt(-val); }

	/// an exponential approach to preventing brown-out (also gives more reasonable responses)
	/// averages in the previous value to make the change less drastic
	inline float expReduceBrownout(const double current, double& past)
		{ return unsignedSqrt(past = ((past + utils::removeGhost(current)) / 2)); }



	// turns the robot a set number of degrees
	// make sure to zero the gyro before running this
	void turnDegrees(RobotDrive mots, double (*getAngle)(), const double angle)
	{
		// how often the turning value is calculated
		#define RTD_DELAY 0.02

		// how close do we need to be to the destination angle
		#define RTD_TOLERANCE 0.01

		// these must be determined by experimentation
		#define RTD_Kp ( 1 / 360 + 0.1)
		#define RTD_KD (RTD_DELAY / 360 + 0.001)


		// while accuracy doesn't matter turn fast
		while (abs(getAngle() - angle) > 25)
			mots.Drive(0.0, (angle - getAngle()) / 90);


		// these will be used for calculations
		volatile double turn;
		double pastAngle = getAngle();

		// we need to be in the tolerance zone for at least 1 second
		bool clockStarted = false;
		clock_t toleranceTimer;

		do {
			// Kp * deg_to_dest + Kd * (change_in_deg)
			turn = RTD_Kp * (getAngle() - angle) + RTD_KD
				 * ((getAngle() - angle) - (pastAngle - angle));

			// this will get used next frame
			pastAngle = getAngle();

			mots.Drive(0.0, turn);

			// we want to be within the tolerance zone for at least a second
			if (!clockStarted && abs(turn) <= RTD_TOLERANCE)
				toleranceTimer = clock();
			// if we exit the tolerance zone we need to reset the clock
			else if (clockStarted && abs(turn) > RTD_TOLERANCE)
				clockStarted = false;


			// might want to wait a bit so turning actually does something
			Wait(RTD_DELAY);


		// while we haven't been in the tolerance zone for more than
		} while (clockStarted && ((float)(clock() - toleranceTimer) / CLOCKS_PER_SEC) < 1);


		// these constant names are actually a bit too vague to keep...
		#undef RTD_DELAY
		#undef RTD_TOLERANCE
		#undef RTD_Kp
		#undef RTD_KD

	}
}


#endif /* SRC_UTILS_HPP_ */
