
#ifndef FRC4941_UTILS_HPP
#define FRC4941_UTILS_HPP


/// a few useful functions for the robot
namespace utils {

	/// remove 'ghost-input' resulting from inaccurate joysticks
	inline float removeGhost(const float val)
		{ return (val > 0.15f || val < -0.15f) ? val : 0.0f; }

	/// plots input on a curve to make driving different
	inline float unsignedSqrt(const float val)
		{ return val > 0 ? sqrt(val) : -sqrt(-val); }


	/// a linear approach to preventing brownout (not effective) (probably doesn't work)
	float linReduceBrownout(const float limit, const float current, float& past){
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

}




#endif /* SRC_UTILS_HPP_ */
