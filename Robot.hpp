#ifndef ROBOT_HPP
#define ROBOT_HPP

#include <iostream>
#include <memory>
#include <string>

#include "WPILib.h"

class Robot: public frc::IterativeRobot {

private:

	// robot parts:
	RobotDrive myRobot; 	 // drive train
	Joystick xBox, climber;  // single xbox360 controller
	VictorSP winch; 		 // used for climbing
	Ultrasonic sonar;		 // distance probe
	ADXRS450_Gyro gyro;		 // get angles and stuff

	frc::LiveWindow* lw = LiveWindow::GetInstance();

	// multiple autonomous codes
	frc::SendableChooser<std::string> chooser;
	const std::string autoDriveForward = "drive forward 2s";
	const std::string autoDoNothing = "sit and wait";
	const std::string autoGoMiddle = "drive to middle peg";
	const std::string autoLeftTurnRight = "start left goto the left peg";
	const std::string autoRightTurnLeft = "start right goto the right peg";
	const std::string autoVisionTest = "vision test";
	std::string autoSelected;

public:

	Robot();

	void RobotInit() override;
	void RobotPeriodic() override;

	void AutonomousInit() override;
	void AutonomousPeriodic() override;

	void TeleopInit() override;
	void TeleopPeriodic() override;

	void TestInit() override
		{ std::cout <<"testing something?" <<std::endl; }
	void TestPeriodic() override
		{ lw->Run(); }

	void DisabledInit() override
		{ std::cout <<"Robot disabled" <<std::endl;	}
	void DisabledPeriodic() override
	{
		// put distance from ultrasonic in inches
		frc::SmartDashboard::PutNumber("Distance: ", sonar.GetRangeInches());
	}



};

#endif
