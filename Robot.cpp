/*
 * Robot.hpp
 *
 *  Created on: Jan 17, 2017
 *      Author: tate
 */

#include "Robot.hpp"

Robot::Robot():
	myRobot(1, 0), // drive train
	xBox(0), // xbox360 controllers???
	//airPump(0), // compressor
	//hamperLift(3), // piston to lift the hamper
	//gearGrabber(4, 5), // this is what holds the gear in place
	accel() // accelerometer in the RoboRIO
{
	myRobot.SetExpiration(0.1);
}


void Robot::RobotInit() {
	chooser.AddDefault(autoNameDefault, autoNameDefault);
	chooser.AddObject(autoNameCustom, autoNameCustom);
	frc::SmartDashboard::PutData("Auto Modes", &chooser);
}

void Robot::AutonomousInit() {

	// enable the motor controllers
	myRobot.SetSafetyEnabled(false);



	autoSelected = chooser.GetSelected();

	// std::string autoSelected = SmartDashboard::GetString("Auto Selector", autoNameDefault);
	std::cout << "Auto selected: " << autoSelected << std::endl;

	if (autoSelected == autoNameCustom) {
		// Custom Auto goes here
	} else {
		// Default Auto goes here
	}
}

void Robot::AutonomousPeriodic() {
	if (autoSelected == autoNameCustom) {
		// Custom Auto goes here
	} else {
		// Default Auto goes here
	}
}

void Robot::TeleopInit() {

	// enable the motor controllers
	myRobot.SetSafetyEnabled(false);


	myRobot.ArcadeDrive(xBox);

}

void Robot::TeleopPeriodic() {

}


START_ROBOT_CLASS(Robot)
