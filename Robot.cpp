#include "Robot.hpp"
#include "utils.hpp"



Robot::Robot():
	myRobot(0, 1),		// drive train
	xBox(0), climber(1),// xbox360 controller
	winch(3),	   		// climbing motor
	winchLimit(2), 		// limit-switch for climbing
	sonar(1, 0),		// ultrasonic range finder
	gyro()
{
	myRobot.SetExpiration(0.1);
}

void Robot::RobotInit() {

	std::cout <<"Robot On" <<std::endl;

	// autonomous chooser code
	chooser.AddDefault(autoDoNothing, autoDoNothing);
	chooser.AddObject(autoDriveForward, autoDriveForward);
	chooser.AddObject(autoGoMiddle, autoGoMiddle);
	chooser.AddObject(autoGyroFun, autoGyroFun);
	chooser.AddObject(autoLeftTurnRight, autoLeftTurnRight);
	chooser.AddObject(autoRightTurnLeft, autoRightTurnLeft);
	frc::SmartDashboard::PutData("Auto Modes", &chooser);

	//get camera feed and post it to the smartdashboard
	CameraServer::GetInstance()->StartAutomaticCapture(0).SetResolution(640, 480);


	// enable the ultrasonic sensor
	sonar.SetAutomaticMode(true);

}


// delete me later (this associated with the gyro-fun auto mode used for testing)
double gyroAngle;
// XDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD


void Robot::AutonomousInit() {

	// enable the motor controllers
	myRobot.SetSafetyEnabled(false);


	autoSelected = chooser.GetSelected();

	// std::string autoSelected = SmartDashboard::GetString("Auto Selector", autoNameDefault);
	std::cout << "Auto selected: " << autoSelected << std::endl;

	if (autoSelected == autoDoNothing) {

	// go to middle peg and deposit the gear
	} else if (autoSelected == autoGoMiddle) {

		gyro.Reset();

		#define GYRO_TURNING_CONST 0.03

		// drive *straight* forward 24in from the middle peg
		while (sonar.GetRangeInches() > 24) {
			myRobot.Drive(0.4f, GYRO_TURNING_CONST * -gyro.GetAngle());
			Wait(0.004);
		}

		// stop moving
		myRobot.Drive(0.0f, 0.0f);

		// at this point the human player would lift the gear
		// out of the robots pocket and earn us a fuckton of
		// points to start the game on a good footing

	} else if (autoSelected == autoGyroFun) {
		gyro.Reset();
		gyroAngle = gyro.GetAngle();

	} else if (autoSelected == autoLeftTurnRight) {

		// reset gyro
		gyro.Reset();

		// drive forward 200in to the turning point
		while (sonar.GetRangeInches() < 200)
			myRobot.Drive(0.5f, 0.0f);

		// stop moving
		myRobot.Drive(0.0f, 0.0f);

		// turn 315 degrees
		gyroAngle = gyro.GetAngle() + 315;
		while (gyroAngle > gyro.GetAngle())
			myRobot.Drive(0.0f, 0.3f);

		// drive forward 6ft to the left peg
		while (sonar.GetRangeInches() < 72)
			myRobot.Drive(0.5f, 0.0f);


		// stop moving
		myRobot.Drive(0.0f, 0.0f);

		// at this point the human player would lift the gear
		// out of the robots pocket and earn us a fuckton of
		// points to start the game on a good footing


	} else if (autoSelected == autoLeftTurnRight) {

		// reset gyro
		gyro.Reset();

		// drive forward 200in to the turning point
		while (sonar.GetRangeInches() < 200)
			myRobot.Drive(0.5f, 0.0f);

		// stop moving
		myRobot.Drive(0.0f, 0.0f);

		// turn 315 degrees
		gyroAngle = gyro.GetAngle() - 315;
		while (gyroAngle <  gyro.GetAngle())
			myRobot.Drive(0.0f, -0.3f);

		// drive forward 6ft to the left peg
		while (sonar.GetRangeInches() < 72)
			myRobot.Drive(0.5f, 0.0f);

		// stop moving
		myRobot.Drive(0.0f, 0.0f);

		// at this point the human player would lift the gear
		// out of the robots pocket and earn us a fuckton of
		// points to start the game on a good footing

	} else {
		// drive forward for 2 seconds and stop
		myRobot.Drive(0.75f, 0.0f);
		Wait(2);
		myRobot.Drive(0.0f, 0.0f);
	}
}

void Robot::AutonomousPeriodic() {
	if (autoSelected == autoGyroFun) {
		frc::SmartDashboard::PutNumber("Gyro get measurement: ", gyro.GetAngle() - gyroAngle);
		std::cout <<"gyro = " <<gyro.GetAngle() - gyroAngle <<std::endl;
	} else {

	}

	// put distance from ultrasonic in inches
	frc::SmartDashboard::PutNumber("Distance: ", sonar.GetRangeInches());

}

void Robot::TeleopInit() {

	std::cout <<"Teleop has begun :))\n";


	// enable the motor controllers
	myRobot.SetSafetyEnabled(false);


}

void Robot::TeleopPeriodic() {

		// turn a button into a switch
	static bool dir_reversable = true, isForward = true;

	// Y switches directions
	if (dir_reversable && xBox.GetRawButton(4)) {
		isForward = !isForward;
		dir_reversable = false;
	} else if (!dir_reversable && !xBox.GetRawButton(4)) {
		dir_reversable = true;
	}

	// turn a button into a switch
	static bool slowable = true, isFast = true;

	// Y switches directions
	if (slowable && xBox.GetRawButton(4)) {
		isForward = !isForward;
		slowable = false;
	} else if (!slowable && !xBox.GetRawButton(4)) {
		slowable = true;
	}


	// joystick data from previous cycle
	static struct vector2_t { // `static` keeps this local variable in memory
		float x = 0, y = 0;
	} stick;

	// drive the robot
	myRobot.ArcadeDrive(
		utils::expReduceBrownout((isFast ? 1 : 0.25f) * (isForward ? -1 : 1)
								 * xBox.GetRawAxis(1), stick.y),
		-utils::expReduceBrownout(xBox.GetRawAxis(4), stick.x) * 0.8f
	);

/*
	// control the winch for climbing
	static bool climb = false;

	// A starts climbing
	if (xBox.GetRawButton(1)) {
		climb = true;
		std::cout <<"Do you even lift???" <<std::endl;
	// B stops climbing
	} else if (xBox.GetRawButton(2))
		climb = false;
*/

	// set it on or off depending on the value of climber.leftTrigger
	winch.Set(climber.GetRawAxis(2) > 0.60 ? 1 : 0);


	// put distance from ultrasonic in inches
	frc::SmartDashboard::PutNumber("Distance: ", sonar.GetRangeInches());

}

void Robot::RobotPeriodic(){
		frc::SmartDashboard::PutBoolean("Gear: ", sonar.GetRangeInches() < 5);

}

START_ROBOT_CLASS(Robot)
