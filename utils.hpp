#ifndef FRC4941_UTILS_HPP
#define FRC4941_UTILS_HPP

#include "WPILib.h"
#include <math.h>
#include <time.h>

/// a few useful functions for the robot
namespace utils {

	/// remove 'ghost-input' resulting from inaccurate joysticks
	// input < 15% gets ignored
	inline double removeGhost(const double val)
		{ return (val > 0.15 || val < -0.15) ? val : 0.0; }


	/// a linear approach to preventing brownout (untested)
	// prevents input from changing faster than a certain rate
	// could probably be combined with expReduceBrounout to improve effectiveness
	double linReduceBrownout(const double limit, const double current, double& past)
	{
		/// limit = maximum amount of change per cycle
		/// current = the most recent value coming from input
		/// past = the value returned by this function in the last frame

		// null or ghost input doesn't affect robot (also good for breaking)
		if (utils::removeGhost(current) == 0.0f) return 0.0;

		double change = current - past;

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

	/// plots input on a curve to make driving different
	inline double unsignedPow2(const double val)
		{ return val > 0 ? pow(val, 2) : -pow(val, 2); }

	/// we haz math 2 win
	inline double huntFunt(const double val)
		{ return val > 0 ? -cos(val * M_PI / 2) + 1 : -(-cos(val * M_PI / 2) + 1); }

	/// an exponential approach to preventing brown-out (also gives more reasonable responses)
	/// averages in the previous value to make the change less drastic
	// TODO: try using a cube-root/cubic/x^2 curve
	inline double expReduceBrownout(const double current, double& past)
		{ return current ? unsignedPow2(past = ((past + utils::removeGhost(current)) / 2)) : 0.0; }

/*
	// turns the robot a set number of degrees
	// make sure to zero the gyro before running this
	void turnDegrees(RobotDrive& mots, ADXRS450_Gyro& gyro, const double angle)
	{
		gyro.Reset();

		// note: this will get replaced with the PIDController class
		// note: this isn't functional


		// how often the turning value is calculated
		#define RTD_DELAY 0.5

		// how close do we need to be to the destination angle
		#define RTD_TOLERANCE 0.01

		// these must be determined by experimentation
		#define RTD_Kp (1.0 / 360)
		#define RTD_KD (RTD_DELAY / 360 + 0.1)


		// while accuracy doesn't matter turn fast
		while (abs(gyro.GetAngle() - angle) > 25)
			mots.ArcadeDrive(0.0, (angle - gyro.GetAngle()) / 90);


		// these will be used for calculations
		volatile double turn;
		double pastAngle = gyro.GetAngle();

		// we need to be in the tolerance zone for at least 1 second
		bool clockStarted = false;
		clock_t toleranceTimer;

		do {
			// Kp * deg_to_dest + Kd * (change_in_deg)
			turn = RTD_Kp * (gyro.GetAngle() - angle) + RTD_KD
				 * ((gyro.GetAngle() - angle) - (pastAngle - angle));
			std::cout <<"turn0: " <<turn <<std::endl;

			// this will get used next frame
			pastAngle = gyro.GetAngle();

			mots.ArcadeDrive(0.0, turn);

			// we want to be within the tolerance zone for at least a second
			if (!clockStarted && abs(turn) <= RTD_TOLERANCE) {
				toleranceTimer = clock();
				clockStarted = true;
			// if we exit the tolerance zone we need to reset the clock
			} else if (clockStarted && abs(turn) > RTD_TOLERANCE)
				clockStarted = false;


			// might want to wait a bit so turning actually does something
			Wait(RTD_DELAY);


		// while we haven't been in the tolerance zone for more than
		} while (!clockStarted && !((float)(clock() - toleranceTimer) / CLOCKS_PER_SEC) < 1);


		// these constant names are actually a bit too vague to keep...
		#undef RTD_DELAY
		#undef RTD_TOLERANCE
		#undef RTD_Kp
		#undef RTD_KD

	}
	*/

	void driveStraight(ADXRS450_Gyro& gyro, RobotDrive& mots, const double time, const double speed = 0.5){
		const double DS_kP = 0.03, DS_CYCLETIME = 0.004;

		// get angle to maintain as zero
		gyro.Reset();

		// drive forward for the set ammount of time
		for (int i = (int) (time / DS_CYCLETIME); i > 0; i--) {
			// turn to correct heading
			mots.Drive(speed, -gyro.GetAngle() * DS_kP); // add negatives for inverted steering/drive
			Wait(DS_CYCLETIME);
			frc::SmartDashboard::PutNumber("angle: ", gyro.GetAngle());
		}

		mots.Drive(0.0, 0.0);

	}
	// should be more accurate, but untested
	void driveStraight2(ADXRS450_Gyro& gyro, RobotDrive& mots, const double time, const double speed = 0.5){
		const double DS_kP = 0.03, DS_CYCLETIME = 0.004;

		// get angle to maintain as zero
		gyro.Reset();

		// drive forward for the set ammount of time
		for (int i = (int) (time / (DS_CYCLETIME / speed)); i > 0; i--) {
			// turn to correct heading
			mots.Drive(speed, -gyro.GetAngle() * DS_kP); // add negatives for inverted steering/drive
			Wait(DS_CYCLETIME);
			frc::SmartDashboard::PutNumber("angle: ", gyro.GetAngle());
		}

		mots.Drive(0.0, 0.0);

	}


}




#endif /* SRC_UTILS_HPP_ */
