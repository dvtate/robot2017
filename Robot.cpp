/* Robot.hpp
 *  Created on: Jan 17, 2017
 */

#include "Robot.hpp"
#include "utils.hpp"



Robot::Robot():
	myRobot(1, 0), // drive train
	xBox(0), // xbox360 controllers???
	winch(2),
	//airPump(0), // compressor
	//gearGrabber(4, 5), // this is what holds the gear in place
	sonar(1, 0),
	accel() // accelerometer in the RoboRIO
{
	myRobot.SetExpiration(0.1);
}

void Robot::RobotInit() {

	std::cout <<"Robot On" <<std::endl;

	// autonomous chooser code
	chooser.AddDefault(autoDoNothing, autoDoNothing);
	chooser.AddObject(autoDriveForward, autoDriveForward);
	frc::SmartDashboard::PutData("Auto Modes", &chooser);

	//get camera feed and post it to the smartdashboard
	CameraServer::GetInstance()->StartAutomaticCapture(0).SetResolution(640, 480);


	// enable the ultrasonic sensor
	sonar.SetAutomaticMode(true);
}

void Robot::AutonomousInit() {

	// enable the motor controllers
	myRobot.SetSafetyEnabled(false);



	autoSelected = chooser.GetSelected();

	// std::string autoSelected = SmartDashboard::GetString("Auto Selector", autoNameDefault);
	std::cout << "Auto selected: " << autoSelected << std::endl;

	if (autoSelected == autoDoNothing) {


	} else {
		// drive forward for 2 seconds and stop
		myRobot.Drive(0.75f, 0.0f);
		Wait(2);
		myRobot.Drive(0.0f, 0.0f);
	}
}

void Robot::AutonomousPeriodic() {
	if (autoSelected == autoDoNothing) {

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


	// control the winch for climbing
	static bool climb = false;

	// x starts climbing
	if (xBox.GetRawButton(3))
		climb = true;
	// y stops climbing
	else if (xBox.GetRawButton(4))
		climb = false;

	// set it to full power or off depending on the value of climb
	winch.Set(climb ? 1 : 0);


	// put distance from ultrasonic in inches
	frc::SmartDashboard::PutNumber("Distance: ", sonar.GetRangeInches());

}

START_ROBOT_CLASS(Robot)
