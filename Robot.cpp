/* Robot.hpp
 *  Created on: Jan 17, 2017
 */

#include "Robot.hpp"
#include "utils.hpp"


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

	// autonomous chooser code
	chooser.AddDefault(autoNameDefault, autoNameDefault);
	chooser.AddObject(autoNameCustom, autoNameCustom);
	frc::SmartDashboard::PutData("Auto Modes", &chooser);

	//get camera feed and post it to the smartdashboard
	//CameraServer::GetInstance()-SetQuality(50);
	CameraServer::GetInstance()->StartAutomaticCapture(0);// camera name in the web interface

	CameraServer::GetInstance()->GetVideo();

	CameraServer::GetInstance()->PutVideo("Blur", 640, 480);
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


}

void Robot::TeleopPeriodic() {



	static bool triggerable = true, isForward = true;

	if (triggerable && xBox.GetRawButton(1)) {
		isForward = !isForward;
		triggerable = false;
	} else if (!triggerable && !xBox.GetRawButton(1)) {
		triggerable = true;

	}


	// joystick data from previous cycle
	static struct vector2_t { // `static` keeps this local variable in memory
		float x = 0, y = 0;
	} stick;

	// drive the robot
	myRobot.ArcadeDrive(
		utils::expReduceBrownout((isForward ? -1 : 1) * xBox.GetRawAxis(1), stick.y),
		-utils::expReduceBrownout(xBox.GetRawAxis(0), stick.x) * 0.8f
	);


}

START_ROBOT_CLASS(Robot)
