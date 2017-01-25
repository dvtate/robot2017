#ifndef ROBOT_HPP
#define ROBOT_HPP


#include <iostream>
#include <memory>
#include <string>

#include "WPILib.h"

class Robot: public frc::IterativeRobot {

private:

	// robot parts:
	RobotDrive myRobot; // drive train
	Joystick xBox; // xbox360 controllers???
	//Compressor airPump; // compressor
	//DoubleSolenoid gearGrabber; // this is what holds the gear in place
	Ultrasonic sonar;
	BuiltInAccelerometer accel; // accelerometer in the RoboRIO

	frc::LiveWindow* lw = LiveWindow::GetInstance();

	// multiple autonomous codes
	frc::SendableChooser<std::string> chooser;
	const std::string autoDriveForward = "drive forward 2s";
	const std::string autoDoNothing = "sit and wait";
	std::string autoSelected;

public:

	Robot();

	void RobotInit();
	void AutonomousInit();
	void AutonomousPeriodic();

	void TeleopInit();
	void TeleopPeriodic();

	void TestInit()
		{ std::cout <<"testing something?" <<std::endl; }
	void TestPeriodic()
		{ lw->Run(); }

	void DisabledInit()
		{ std::cout <<"Robot disabled" <<std::endl;	}
	void DisabledPeriodic()
	{
		// put distance from ultrasonic in inches
		frc::SmartDashboard::PutNumber("Distance: ", sonar.GetRangeInches());
	}



};



#endif
