#ifndef FRC4941_UTILS_HPP
#define FRC4941_UTILS_HPP

#include "WPILib.h"
#include <math.h>
#include <time.h>

/// a few useful functions for the robot
namespace utils {

	/// remove 'ghost-input' resulting from inaccurate joysticks
	inline float removeGhost(const float val)
		{ return (val > 0.15f || val < -0.15f) ? val : 0.0f; }

	/// plots input on a curve to make driving different
	inline float unsignedSqrt(const float val)
		{ return val > 0 ? sqrt(val) : -sqrt(-val); }


	/// a linear approach to preventing brownout (not effective)
	float linReduceBrownout(const float limit, const float current, float& past)
	{
		/// limit = maximum ammount of change per cycle
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


	// an exponential approach to preventing brownout (also gives more reasonable responses)
	inline float expReduceBrownout(const float current, float& past)
		{ return unsignedSqrt(past = ((past + utils::removeGhost(current)) / 2)); }



	// turns the robot a set number of degrees
	// make sure to zero the gyro before running this
	void turnDegrees(RobotDrive mots, double (*getAngle)(), const double angle)
	{
		// these must be determined by experimentation
		#define ROBOT_TURNING_Kp 0.5
		#define ROBOT_TURNING_KD 0.5
		#define ROBOT_TURNING_TOLERANCE 0.01


		volatile double turn;
		double pastAngle = getAngle();

		// we need to be in the tolerance zone for at least 1 second
		bool clockStarted = false;
		clock_t toleranceTimer;

		do {

			turn = ROBOT_TURNING_Kp * (getAngle() - angle) + ROBOT_TURNING_KD
				 * ((getAngle() - angle) - (pastAngle - angle));

			mots.Drive(0.0, turn);

			// we want to be within the tolerance zone for at least a second
			if (!clockStarted && abs(turn) <= ROBOT_TURNING_TOLERANCE)
				toleranceTimer = clock();
			// if we exit the tolerance zone we need to reset the clock
			else if (clockStarted && abs(turn) > ROBOT_TURNING_TOLERANCE)
				clockStarted = false;


			// might want to wait a bit so turning actually does something
			// Wait(0.002);

			pastAngle = getAngle();

		// while we haven't been in the tolerance zone for more than
		} while (clockStarted && ((float)(clock() - toleranceTimer) / CLOCKS_PER_SEC) < 1);


	}
}




#endif /* SRC_UTILS_HPP_ */
