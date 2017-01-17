/*
 * Robot.cpp
 *
 *  Created on: Jan 17, 2017
 *      Author: tate
 */

#include "Robot.hpp"

Robot::Robot():
	myRobot(1, 2), // drive train
	driveCtl(0), shootCtl(1), // xbox360 controllers???
	airPump(0), // compressor
	hamperLift(3), // piston to lift the hamper
	gearGrabber(4, 5), // this is what holds the gear in place
	accel() // accelerometer in the RoboRIO
{ }


void Robot::RobotInit() {
	chooser.AddDefault(autoNameDefault, autoNameDefault);
	chooser.AddObject(autoNameCustom, autoNameCustom);
	frc::SmartDashboard::PutData("Auto Modes", &chooser);
}

void Robot::AutonomousInit() {
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

}

void Robot::TeleopPeriodic() {

}


START_ROBOT_CLASS(Robot)
