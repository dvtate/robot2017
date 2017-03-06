# main-bot-2017
This is our primary cobot code.

# What We Want to Show Off:
### Controller code
 - we have spent 2 years perfecting our driving code, it has better consumption characteristics and behavior than the standard arcadeDrive function
  + `utils::removeGhost(double)`: removes input which could come from inacurate controllers
  + `utils::expReduceBrownout(double, double&)`: averages previous value in with the first one to make changes less extreme and round edges; plots calculated input onto a curve
  + `utils::linReduceBrownout(double, double, dobule&)`: if the change in input value is too great it changes the change to the limit given. This hasn't been tested, but could be used in combination with `utils::expReduceBrownout`. 
### Automatic Driver
 - Because the drive-train isn't perfect and the carpet doesn't give uniform traction, we can gain from being able to drive perfectly straight without veering off course.
 - `utils::driveStraight(ADXRS450_Gyro&, RobotDrive&, double, double)`: uses the angle measurement from the gyro as the turning value. Drives straight at a given speed for a given number of seconds


# Goals & Progress:
* Side-peg autonomous:
 * (done) use gyro to drive straight and correct for imperfect drive-train
 * (nope) use ultrasonic to stop once we are the correct distance out (hardware issues)
 * (done) use vision to identify the retro-reflective tape on the sides of the peg 
 * (nope) calculate degrees needed to point at the peg. We also need to account for offset camera (we have an equation for this)
 * (nope) use gyro and PIDController object to turn the calculated degrees (const calibration consumes time)


# Rules for editing:
 - Put all changes in your commit description
 - If you need a text editor, I recommend you install eclipse to take full advantage of it's syntax highlighting, error detection, and integrated documentation
 - Include lots of comments (use consice language so we don't have to read an essay)
 - a great person once said "good code explains itself" : use self-explanatory names, syntax, organization, grouping, etc. and write code that is easy to read.
 - KISS = Keep It Simple Stupid
 - Have Fun :)
 - **You must be a RoboBibb Student to commit/PR.**
 
