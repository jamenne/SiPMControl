/*

SiPMMain.cpp

Created by Janine Müller on 07.10.2016

*/

#include <iostream>
#include <fcntl.h>
#include <cstdlib>
#include <iomanip>

#include "../gpib/gpib.h"
#include "SiPM.h"
#include "../PT1000/PT1000Control.h"

using namespace std;

int main(int argc, char const *argv[])
{

	// ----------Flags for while(true) interrupt------------- //
	int fd = STDIN_FILENO;
	int flags = fcntl(fd, F_GETFL, 0);
	fcntl(fd, F_SETFL, flags | O_NONBLOCK);

	//--------------------------GPIB adapter----------------------------//
	int masterUD = InitializeMaster();

	// -------------------------SourceMeter----------------------------//
	SourceMeter SMSiPM;

	int SourceMeterPad1 = 26;

	SMSiPM.Initialize(masterUD, SourceMeterPad1);

	SourceMeter SMPelztier;

	int SourceMeterPad2 = 24;

	SMPelztier.Initialize(masterUD, SourceMeterPad2);


	// -------------------------MultiMeter----------------------------//	

	MultiMeter MultiM;

	int MultiMeterPad = 18;

	MultiM.Initialize(masterUD, MultiMeterPad);



	//--------------------------Pelztier----------------------------//

	int smuX_pelztier = 1;
	// SourceMeter, Output, MultiMeter
	Pelztier Peltier(SourceM, smuX_pelztier, MultiM);

	const string voltagelimit = "0.6";

	Peltier.Initialize(voltagelimit);



	//--------------------------SiPM----------------------------//

	double biasVoltage = 53.11; //Volt
	const string currentlimit = "0.001"; //Ampere
	int smuX_SiPM = 2;

	SiPM Ham(biasVoltage, SourceM, smuX_SiPM, Peltier);

	Ham.Initialize(biasVoltage, currentlimit);


	//--------------------------UI Curve--------------------------//
	// double startVoltage = biasVoltage - 3;
	// double endVoltage = biasVoltage+1;

	// Ham.RampToVoltage(startVoltage);

	// double actualVoltage = Ham.MeasureV();
	// vector<double> measure(2,0);

	// // variables for the temperature control of the peltier element
	// double temp_target = -10;
	// int index = 0;
	// double integral = 0;
	// vector<double> TempDiff(5,0);
	// double current = 0;

	// int meas_timer = -5*60; // wait 5min before starting with SiPM measurements
	// int logfile_timer = 0;

	// do{

	// 	Peltier.OneTempControl(TempDiff, integral, index, current, temp_target);

	// 	if (meas_timer == 2*60) // sleep 1 sec * 2*60 = measure every 2 minutes
	// 	{
	// 		measure = Ham.MeasureIV();
	// 		actualVoltage = measure[1];
	// 		cout << "actualVoltage:\t" << actualVoltage << endl;

	// 		if (actualVoltage < endVoltage)
	// 		{
	// 			Ham.SetSourceVoltage(actualVoltage + 0.1);
	// 		}

	// 		else if(actualVoltage >= endVoltage){
	// 			cout << "Reached end of UI range!" << endl;
	// 			break;
	// 		}

	// 		meas_timer = 0;
	// 	}		

	// 	if (logfile_timer == 60*60*24) // 60*60*24 every day a new logfile
	// 	{
	// 		Peltier.GetLogFile().Initialize("PelztierControl");
	// 		Ham.GetSourceMeter().GetLogFile().Initialize("SourceMeter");
	// 		Ham.GetLogFile().Initialize("SiPM");
	// 		logfile_timer = 0;
	// 	}


	// 	sleep(1);
	// 	meas_timer++;
	// 	logfile_timer++;

	// }while(getchar() != 'q');


	
	//-------------------Current Measurement loop----------------------//

	Ham.RampToBiasVoltage();


	vector<double> measure(2,0);

	// variables for the temperature control of the peltier element
	double temp_target = 5;
	int index = 0;
	double integral = 0;
	vector<double> TempDiff(5,0);
	double current = 0;

	int meas_timer = 0;
	int logfile_timer = 0;

	do{

		Peltier.OneTempControl(TempDiff, integral, index, current, temp_target);

		if (meas_timer == 1) // sleep 1 sec * 60 = measure IV every minute
		{
			measure = Ham.MeasureIV();
			meas_timer = 0;
		}		

		if (logfile_timer == 45*60*24) // 60*60*24 every day a new logfile
		{
			Peltier.GetLogFile().Initialize("PelztierControl");
			Ham.GetSourceMeter().GetLogFile().Initialize("SourceMeter");
			Ham.GetLogFile().Initialize("SiPM");
			logfile_timer = 0;
		}


		sleep(1);
		meas_timer++;
		logfile_timer++;

	}while(getchar() != 'q');

	//--------------------------SURFACE TEMPERATURE WITH PT1000--------------------------//
	
	// -------------------------------MultiMeter 2-----------------------------------//	

	// vector<double> measure(2,0);

	// MultiMeter MultiM2;
	// int MultiMeterPad2 = 17;

	// MultiM2.Initialize(masterUD, MultiMeterPad2);

	// PT1000 Thermometer(MultiM2);

	// Thermometer.Initialize();
	// double PT1000_temp=0;

	// Ham.RampToBiasVoltage();

	// // variables for the temperature control of the peltier element
	// double temp_target = 0;
	// double end_temp = -20;
	// int index = 0;
	// double integral = 0;
	// vector<double> TempDiff(5,0);
	// double current = 0;

	// int meas_timer = -2*60; // wait 2min before starting with SiPM measurements
	// int logfile_timer = 0;

	// do{

	// 	Peltier.OneTempControl(TempDiff, integral, index, current, temp_target);

	// 	cout << "temp_target:\t" << temp_target << endl;
	// 	if (meas_timer == 60*3) // sleep 1 sec * 5*60 = measure every 5 minutes
	// 	{
	// 		measure = Ham.MeasureIV();
	// 		PT1000_temp = Thermometer.GetTemperature(false);

	// 		Ham.GetLogFile().WriteSingleMeasurement(PT1000_temp);

	// 		if (Ham.GetPelztier().GetTemperature() > end_temp)
	// 		{
	// 			temp_target = temp_target-5;
	// 		}

	// 		else break;

	// 		meas_timer = 0;
	// 	}		

	// 	if (logfile_timer == 60*60*24) // 60*60*24 every day a new logfile
	// 	{
	// 		Peltier.GetLogFile().Initialize("PelztierControl");
	// 		Ham.GetSourceMeter().GetLogFile().Initialize("SourceMeter");
	// 		Ham.GetLogFile().Initialize("SiPM");
	// 		logfile_timer = 0;
	// 	}


	// 	sleep(1);
	// 	meas_timer++;
	// 	logfile_timer++;

	// }while(getchar() != 'q');


	//----------------------------------------------------------------//

	Ham.Close();

	Peltier.Close();


	
	return 0;
}