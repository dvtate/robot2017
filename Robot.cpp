#include <time.h>
#include <GripPipeline.hpp>
#include "Robot.hpp"
#include "utils.hpp"
#include "GripPipeline.hpp"



Robot::Robot():
	myRobot(0, 4, 1, 3),// drive train
	//myRobot(2,3,0,1), // TESTING ONLY PLS REMOVE!!!!
	xBox(0), climber(1),// xbox360 controller
	winch(2),	   		// climbing motor
	sonar(1, 0),		// ultrasonic range finder
	gyro()
{
	myRobot.SetExpiration(0.1);
}


void Robot::RobotInit() {

	std::cout <<"Robot On" <<std::endl;

	// autonomous chooser code
	chooser.AddDefault(autoDriveForward, autoDriveForward); // drive past baseline
	chooser.AddObject(autoGoMiddle, autoGoMiddle); // middle peg
	//chooser.AddObject(autoGoDist, autoGoDist);
	chooser.AddObject(autoDoNothing, autoDoNothing); // don't do anything7
	//chooser.AddObject(autoLeftTurnRight, autoLeftTurnRight);
	//chooser.AddObject(autoRightTurnLeft, autoRightTurnLeft);
	//chooser.AddObject(autoVisionTest, autoVisionTest);
	frc::SmartDashboard::PutData("Auto Modes", &chooser);

	//get camera feed and post it to the smartdashboard
	CameraServer::GetInstance()->StartAutomaticCapture(0).SetResolution(640, 480);


	// enable the ultrasonic sensor
	sonar.SetAutomaticMode(true);

	frc::SmartDashboard::GetNumber("Distance: ", 0);

}


void Robot::AutonomousInit() {

	// enable the motor controllers
	myRobot.SetSafetyEnabled(false);


	autoSelected = chooser.GetSelected();

	// std::string autoSelected = SmartDashboard::GetString("Auto Selector", autoNameDefault);
	std::cout << "Auto selected: " << autoSelected << std::endl;

	if (autoSelected == autoDoNothing) {

	} else if (autoSelected == autoVisionTest) {
		//grip::GripPipeline camPipe;
		//camPipe.Process();
	// go to middle peg and deposit the gear
	} else if (autoSelected == autoGoMiddle) {

		// drive until 8 inches from peg

		utils::driveStraight(gyro, myRobot, 2.9, 0.3);
		myRobot.Drive(0.0, 0.0);


		// at this point the human player would lift the gear
		// out of the robots pocket and earn us a fuckton of
		// points to start the game on a good footing

	} else if (autoSelected == autoLeftTurnRight) {

		// reset gyro
		gyro.Reset();

		// drive forward 200in to the turning point
		while (sonar.GetRangeInches() < 200)
			myRobot.Drive(0.5, 0.0);

		// stop moving
		myRobot.Drive(0.0, 0.0);

		// turn 315 degrees
		double gyroAngle = gyro.GetAngle() + 315;
		while (gyroAngle > gyro.GetAngle())
			myRobot.Drive(0.0, 0.3);

		// drive forward 6ft to the left peg
		while (sonar.GetRangeInches() < 72)
			myRobot.Drive(0.5, 0.0);


		// stop moving
		myRobot.Drive(0.0, 0.0);

		// at this point the human player would lift the gear
		// out of the robots pocket and earn us a fuckton of
		// points to start the game on a good footing


	} else if (autoSelected == autoLeftTurnRight) {

		// reset gyro
		gyro.Reset();

		// drive forward 200in to the turning point
		while (sonar.GetRangeInches() < 200)
			myRobot.Drive(0.5, 0.0);

		// stop moving
		myRobot.Drive(0.0, 0.0);

		// turn 315 degrees
		double gyroAngle = gyro.GetAngle() - 315;
		while (gyroAngle <  gyro.GetAngle())
			myRobot.Drive(0.0, -0.3);

		// drive forward 6ft to the left peg
		while (sonar.GetRangeInches() < 72)
			myRobot.Drive(0.5, 0.0);

		// stop moving
		myRobot.Drive(0.0, 0.0);

		// at this point the human player would lift the gear
		// out of the robots pocket and earn us a fuckton of
		// points to start the game on a good footing

	// drive forward for 2 seconds and stop
	} else if (autoSelected == autoDriveForward) {
		// drive straight for 2 seconds @ 80% pwr
		utils::driveStraight(gyro, myRobot, 2, 0.75);

		// this is stupid code
		//myRobot.Drive(0.8, 0.0);
		//Wait(2);
		myRobot.Drive(0.0, 0.0);

	// drives until ultrasonic reads given value
	} else if (autoSelected == autoGoDist) {
		clock_t feedTimeout = clock();

		// drive until given inches from peg
		while (sonar.GetRangeInches() > frc::SmartDashboard::GetNumber("Distance: ", 0)
			   && ((float)(clock() - feedTimeout) / CLOCKS_PER_SEC) < 4)
			utils::driveStraight(gyro, myRobot, 0.06, 0.4);
		myRobot.Drive(0.0, 0.0);

	}

}

void Robot::AutonomousPeriodic() {
	/*
	if (autoSelected == autoDoNothing) {
		// NOP
	} else {

	}
	*/

	// put distance from ultrasonic in inches
	//frc::SmartDashboard::PutNumber("Distance: ", sonar.GetRangeInches());

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
	if (slowable && xBox.GetRawButton(6)) {
		isForward = !isForward;
		slowable = false;
	} else if (!slowable && !xBox.GetRawButton(6)) {
		slowable = true;
	}


	// joystick data from previous cycle
	static double stickX = 0, stickY = 0; // `static` keeps this local variable in memory

	// drive the robot
	myRobot.ArcadeDrive(
		utils::expReduceBrownout((isFast ? 1 : 0.25) * (isForward ? -1 : 1)
								 * xBox.GetRawAxis(1), stickY),
		-utils::expReduceBrownout(xBox.GetRawAxis(4), stickX) * 0.8
	);

	// control the winch for climbing
	static bool climb = false;

	// A starts climbing
	if (xBox.GetRawButton(1)) {
		climb = true;
		std::cout <<"Do you even lift???" <<std::endl;
	// B stops climbing
	} else if (xBox.GetRawButton(2))
		climb = false;

	// set it on or off depending on the value of climber.leftTrigger
	winch.Set(climb || (climber.GetRawAxis(3) > 0.60) ? 1 : 0);

	/*
	// test ultrasonic
	frc::SmartDashboard::PutNumber("Dist (recieved): ", sonar.GetRangeInches() + 1);
	std::cout <<sonar.GetRangeInches() <<std::endl;
	*/

}

void Robot::RobotPeriodic(){
//	frc::SmartDashboard::PutNumber("Dist (recieved): ", sonar.GetRangeInches());
}

START_ROBOT_CLASS(Robot)
