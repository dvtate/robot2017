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
	//Solenoid hamperLift; // piston to lift the hamper
	//DoubleSolenoid gearGrabber; // this is what holds the gear in place
	BuiltInAccelerometer accel; // accelerometer in the RoboRIO

	frc::LiveWindow* lw = LiveWindow::GetInstance();

	// multiple autonomous codes
	frc::SendableChooser<std::string> chooser;
	const std::string autoNameDefault = "Default";
	const std::string autoNameCustom = "My Auto";
	std::string autoSelected;

public:

	Robot();
	/*
	 * This autonomous (along with the chooser code above) shows how to select
	 * between different autonomous modes using the dashboard. The sendable
	 * chooser code works with the Java SmartDashboard. If you prefer the
	 * LabVIEW Dashboard, remove all of the chooser code and uncomment the
	 * GetString line to get the auto name from the text box below the Gyro.
	 *
	 * You can add additional auto modes by adding additional comparisons to the
	 * if-else structure below with additional strings. If using the
	 * SendableChooser make sure to add them to the chooser code above as well.
	 */
	void RobotInit();
	void AutonomousInit();
	void AutonomousPeriodic();

	void TeleopInit();

	void TeleopPeriodic();

	void TestPeriodic()
		{ lw->Run(); }



};



#endif
