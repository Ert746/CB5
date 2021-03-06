#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <WPILib.h>
#include <CameraServer.h>
#include <IterativeRobot.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/core/types.hpp>//all of these might not be neccisary
#include <LiveWindow/LiveWindow.h>
#include <SmartDashboard/SendableChooser.h>
#include <SmartDashboard/SmartDashboard.h>

class Robot: public frc::IterativeRobot {//uncoment to enable vision
	static void VisionThread(){// multithreading is required for the image proccessing so yah
			 cs::UsbCamera camera = CameraServer::GetInstance()->StartAutomaticCapture();//starts capturing basic images into camera

			// camera.SetExposureAuto();
			 camera.SetResolution(640,480);

			 cs::CvSink sinker = CameraServer::GetInstance()->GetVideo();//Grabs video to sink into the mat image cruncher

			 cs::CvSource cheese = CameraServer::GetInstance()->PutVideo("Rectangle",640,480);//Serves up the images gathered your on camera

			 cv::Mat cruncher(640,480,CV_8UC1);//this is the magic image cruncher when converting make sure there both the same type try this next CV_16UC1 its 16nit the other is 8
			 // also there is CV_32FC1 i think its 32bit these also appere to not need the C1 so try CV_8U and the likes

			 //cvtColor(cruncher,cruncher_grey,COLOR_BGR2HSV);//bam makes it grey I think

		 while(true){//image processing happens in here

				 if(sinker.GrabFrame(cruncher)==0){// if theres nothing there you got problems

					 cheese.NotifyError(sinker.GetError());//HEY LISTEN! you got some problems tell me about them
					 continue;//restarts the thread I think
			 }
				 rectangle(cruncher, cv::Point(0, 0), cv::Point(50, 50),cv::Scalar(255, 255, 255), 5);//draw some rectangles on that thing WOOT RECTANGLES


				 	 cheese.PutFrame(cruncher);//finally put the final modified frame
				 	SmartDashboard::PutNumber("Point 25,25",cruncher.at<uchar>(25,25));



		 }
	}
public:

	double Leftgo,Rightgo;
	bool light;


	Joystick *rightDrive =new Joystick(0,2,9);
	Joystick *leftDrive =new Joystick(1,2,9);
	Joystick *gamePad =new Joystick(2,6,9);

	Talon *fLeft =new Talon(0);
	Talon *fRight =new Talon(1);
	Talon *bLeft =new Talon(2);
	Talon *bRight =new Talon(3);

	Encoder *encRight=new Encoder(0,1);
	Encoder *encLeft=new Encoder(2,3);

	DigitalOutput *lightpwm =new DigitalOutput(0);

	RobotDrive *robotDrive =new RobotDrive(fLeft,fRight,bLeft,bRight);

	void RobotInit() {
		std::thread camthread(VisionThread);//makes a new thread
		camthread.detach();//snaps the thread off to do its own thing

		chooser.AddDefault(autoNameDefault, autoNameDefault);//I don't like this look into making it logical
		chooser.AddObject(autoNameCustom, autoNameCustom);
		frc::SmartDashboard::PutData("Auto Modes", &chooser);

	}

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
	void AutonomousInit() override {
		autoSelected = chooser.GetSelected();
		// std::string autoSelected = SmartDashboard::GetString("Auto Selector", autoNameDefault);
		std::cout << "Auto selected: " << autoSelected << std::endl;

		if (autoSelected == autoNameCustom) {
			// Custom Auto goes here
		} else {
			// Default Auto goes here
		}
	}

	void AutonomousPeriodic() {
		if (autoSelected == autoNameCustom) {
			// Custom Auto goes here
		} else {
			// Default Auto goes here
		}
	}

	void TeleopInit() {
		Leftgo=0;
		Rightgo=0;
		encRight->Reset();
			encLeft->Reset();
			light=0;

	}

	void TeleopPeriodic() {
		Leftgo=.75*leftDrive->GetRawAxis(1);
		Rightgo=.75*rightDrive->GetRawAxis(1);

		robotDrive->TankDrive(Leftgo,Rightgo);

		light=gamePad->GetRawButton(1);

		if(light){
			lightpwm->Set(1);
		}
		else{
			lightpwm->Set(0);
		}

		SmartDashboard::PutNumber("encRight",encRight->GetRaw());
		SmartDashboard::PutNumber("encLeft",encLeft->GetRaw());

		SmartDashboard::PutNumber("Leftgo",Leftgo);
		SmartDashboard::PutNumber("Rightgo",Rightgo);
	}

	void TestPeriodic() {
		lw->Run();
	}

private://why is this down here?
	frc::LiveWindow* lw = LiveWindow::GetInstance();
	frc::SendableChooser<std::string> chooser;
	const std::string autoNameDefault = "Default";
	const std::string autoNameCustom = "Dance Party";
	std::string autoSelected;
};

START_ROBOT_CLASS(Robot)
