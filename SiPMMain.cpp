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

void SurfaceTemp(MultiMeter &MultiM2, SiPM &Ham){

	//--------------------------SURFACE TEMPERATURE WITH PT1000--------------------------//
	// copy and pasted it form the main file. Not again tested yet....

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

}

void UICurve(SiPM &Ham1, SiPM &Ham2, Pelztier &Peltier1, Pelztier &Peltier2){ // needs to be tested again

	//------current date for Logfiles and time for measurements------//

	time_t now = time(NULL);

	struct tm * timeinfo = localtime(&now);

	int today = timeinfo->tm_mday;

	int sec = now;


	//--------------------------UI Curve--------------------------//
	double startVoltage = Ham1.GetBiasVoltage() - 3;
	double endVoltage = Ham1.GetBiasVoltage() + 1;

	Ham1.RampToVoltage(startVoltage);
	Ham2.RampToVoltage(startVoltage);

	double actualVoltage1 = Ham1.MeasureV();
	double actualVoltage2 = Ham2.MeasureV();
	vector<double> measure1(2,0);
	vector<double> measure2(2,0);

	// variables for the temperature control of the peltier element
	double temp_target1 = 0;
	double temp_target2 = 0;

	int index1 = 0;
	int index2 = 0;

	double integral1 = 0;
	double integral2 = 0;

	vector<double> TempDiff1(5,0);
	vector<double> TempDiff2(5,0);

	double current1 = 0;
	double current2 = 0;

	// time for aclimatisation
	for (int i = 0; i < 180; ++i)
	{
		Peltier1.OneTempControl(TempDiff1, integral1, index1, current1, temp_target1);
		Peltier2.OneTempControl(TempDiff2, integral2, index2, current2, temp_target2);
		sleep(1);
	}

	do{

		Peltier1.OneTempControl(TempDiff1, integral1, index1, current1, temp_target1);
		Peltier2.OneTempControl(TempDiff2, integral2, index2, current2, temp_target2);

		now = time(NULL);

		if (sec+120 <= now ) // measure every 2 minutes
		{
			measure1 = Ham1.MeasureIV();
			measure2 = Ham2.MeasureIV();
			actualVoltage1 = measure1[1];
			actualVoltage2 = measure2[1];
			cout << "actualVoltage1:\t" << actualVoltage1 << endl;
			cout << "actualVoltage2:\t" << actualVoltage2 << endl;

			if (actualVoltage1 < endVoltage && actualVoltage2 < endVoltage)
			{
				Ham1.SetSourceVoltage(actualVoltage1 + 0.1);
				Ham2.SetSourceVoltage(actualVoltage2 + 0.1);
			}

			else if(actualVoltage1 >= endVoltage || actualVoltage2 >= endVoltage){
				cout << "Reached end of UI range!" << endl;
				break;
			}

			sec = now;
		}		

		if (today != timeinfo->tm_mday) // every day a new logfile
		{
			stringstream ss;

			ss << "PelztierControl_" << Peltier1.GetSourceMeterChannel();
			Peltier1.GetLogFile().Initialize(ss.str().c_str());
			Peltier1.GetLogFile().WriteString("#date\ttemp\tcurrent\tvoltage");
			ss.str("");

			ss << "PelztierControl_" << Peltier2.GetSourceMeterChannel();
			Peltier2.GetLogFile().Initialize(ss.str().c_str());
			Peltier2.GetLogFile().WriteString("#date\ttemp\tcurrent\tvoltage");
			ss.str("");			

			ss << "SiPM_" << Ham1.GetSourceMeterChannel();
			Ham1.GetLogFile().Initialize(ss.str().c_str());
			Ham1.GetLogFile().WriteString("#time\ttemperature\tcurrent\tvoltage");
			ss.str("");
			ss << "SiPM_" << Ham2.GetSourceMeterChannel();
			Ham2.GetLogFile().Initialize(ss.str().c_str());
			Ham2.GetLogFile().WriteString("#time\ttemperature\tcurrent\tvoltage");
			ss.str("");

			Ham1.GetSourceMeter().GetLogFile().Initialize("SourceMeterSiPM");

			today = timeinfo->tm_mday;

		}


		sleep(1);

	}while(getchar() != 'q');

}

void AttenuationMeasurement(SiPM &Ham1, SiPM &Ham2, Pelztier &Peltier1, Pelztier &Peltier2){ // needs to be tested again

	//------current date for Logfiles and time for measurements------//

	time_t now = time(NULL);

	int sec_IV = now;

	int sec_Sr90 = now;

	//----------------Attenuation length measurement-------------------//

	Ham1.RampToBiasVoltage();
	Ham2.RampToBiasVoltage();


	vector<double> measure1(2,0);
	vector<double> measure2(2,0);

	// variables for the temperature control of the peltier element
	double temp_target1 = 5;
	double temp_target2 = 5;

	int index1 = 0;
	int index2 = 0;

	int pos = 0;
	string keypress;

	double integral1 = 0;
	double integral2 = 0;

	vector<double> TempDiff1(5,0);
	vector<double> TempDiff2(5,0);

	double current1 = 0;
	double current2 = 0;

	// time for aclimatisation
	for (int i = 0; i < 180; ++i)
	{
		Peltier1.OneTempControl(TempDiff1, integral1, index1, current1, temp_target1);
		Peltier2.OneTempControl(TempDiff2, integral2, index2, current2, temp_target2);
		sleep(1);
	}

	do{

		Peltier1.OneTempControl(TempDiff1, integral1, index1, current1, temp_target1);
		Peltier2.OneTempControl(TempDiff2, integral2, index2, current2, temp_target2);

		now = time(NULL);

		if (sec_IV+1 <= now) // measure IV every second
		{
			measure1 = Ham1.MeasureIV();
			measure2 = Ham2.MeasureIV();
			sec_IV = now;
		}		

		if (sec_Sr90 + 20 <= now) // every 20s new logfile and move Sr90!
		{
			stringstream ss;			

			ss << "SiPM_" << Ham1.GetSourceMeterChannel() << "_" << pos;
			Ham1.GetLogFile().Initialize(ss.str().c_str());
			Ham1.GetLogFile().WriteString("#time\ttemperature\tcurrent\tvoltage");
			ss.str("");
			ss << "SiPM_" << Ham2.GetSourceMeterChannel() << "_" << pos;
			Ham2.GetLogFile().Initialize(ss.str().c_str());
			Ham2.GetLogFile().WriteString("#time\ttemperature\tcurrent\tvoltage");

			cout << "MOVE Sr90 source to next position and press 'c' followed by ENTER" << endl;
			
			while(getchar() != 'c'){
				sleep(1);
			}

			sec_Sr90 = now;
			pos ++;
		}


		sleep(1);

	}while(getchar() != 'q');

}

void InsituMeasurement(SiPM &Ham1, SiPM &Ham2, Pelztier &Peltier1, Pelztier &Peltier2){

	//--------- current date for Logfiles and time for measurements ---------//

	time_t now = time(NULL);

	struct tm * timeinfo = localtime(&now);

	int today = timeinfo->tm_mday;

	int sec = now;

	
	//---------------------- Measurement loop -------------------------//

	Ham1.RampToBiasVoltage();
	Ham2.RampToBiasVoltage();

	// measurement interval for IV measurement
	int measinterval = 2 //seconds

	vector<double> measure1(2,0);
	vector<double> measure2(2,0);

	// target temperature of SiPMs
	double temp_target1 = 0;
	double temp_target2 = 0;

	// variables for the temperature control of the peltier element
	int index1 = 0;
	int index2 = 0;

	double integral1 = 0;
	double integral2 = 0;

	vector<double> TempDiff1(5,0);
	vector<double> TempDiff2(5,0);

	double current1 = 0;
	double current2 = 0;

	// do-while loop: exit by hitting 'q' followed by 'ENTER'
	do{

		Peltier1.OneTempControl(TempDiff1, integral1, index1, current1, temp_target1);
		Peltier2.OneTempControl(TempDiff2, integral2, index2, current2, temp_target2);

		now = time(NULL);

		if (sec+measinterval <= now ) // measure every 2 minutes / 5 times
		{
			for (int i = 0; i < 5; i++)
			{
				measure1 = Ham1.MeasureIV();
				measure2 = Ham2.MeasureIV();
				sec = now;
			}
		}		

		if (today != timeinfo->tm_mday) // every day a new logfile
		{
			stringstream ss;

			ss << "PelztierControl_" << Peltier1.GetSourceMeterChannel();
			Peltier1.GetLogFile().Initialize(ss.str().c_str());
			Peltier1.GetLogFile().WriteString("#date\ttemp\tcurrent\tvoltage");
			ss.str("");

			ss << "PelztierControl_" << Peltier2.GetSourceMeterChannel();
			Peltier2.GetLogFile().Initialize(ss.str().c_str());
			Peltier2.GetLogFile().WriteString("#date\ttemp\tcurrent\tvoltage");
			ss.str("");			

			ss << "SiPM_" << Ham1.GetSourceMeterChannel();
			Ham1.GetLogFile().Initialize(ss.str().c_str());
			Ham1.GetLogFile().WriteString("#time\ttemperature\tcurrent\tvoltage");
			ss.str("");
			ss << "SiPM_" << Ham2.GetSourceMeterChannel();
			Ham2.GetLogFile().Initialize(ss.str().c_str());
			Ham2.GetLogFile().WriteString("#time\ttemperature\tcurrent\tvoltage");
			ss.str("");

			Ham1.GetSourceMeter().GetLogFile().Initialize("SourceMeterSiPM");
			Peltier1.GetSourceMeter().GetLogFile().Initialize("SourceMeterPeltiers");

			today = timeinfo->tm_mday;


		}

		sleep(1);

	}while(getchar() != 'q');
}

int main(int argc, char const *argv[])
{

	// ----------------Flags for while(true) interrupt------------------//
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

	MultiMeter MM1;
	MultiMeter MM2;

	int MultiMeterPad1 = 17;
	int MultiMeterPad2 = 18;

	MM1.Initialize(masterUD, MultiMeterPad1);
	MM2.Initialize(masterUD, MultiMeterPad2);



	//--------------------------Pelztiere----------------------------//

	int smuX_pelztier1 = 1; // channel A
	int smuX_pelztier2 = 2; // channel B

	// SourceMeter, Output, MultiMeter
	Pelztier Peltier1(SMPelztier, smuX_pelztier1, MM1);
	Pelztier Peltier2(SMPelztier, smuX_pelztier2, MM2);

	const string voltagelimit = "1";

	Peltier1.Initialize(voltagelimit);
	Peltier2.Initialize(voltagelimit);



	//----------------------------SiPMs------------------------------//

	double biasVoltage1 = 53.11; //Volt
	double biasVoltage2 = 53.11; //Volt
	const string currentlimit = "0.001"; //Ampere

	int smuX_SiPM1 = 1; // channel A
	int smuX_SiPM2 = 2; // channel B

	SiPM Ham1(biasVoltage1, SMSiPM, smuX_SiPM1, Peltier1);
	SiPM Ham2(biasVoltage2, SMSiPM, smuX_SiPM2, Peltier2);

	Ham1.Initialize(currentlimit);
	Ham2.Initialize(currentlimit);

	//------------------Select Measurement from above--------------------//


	int measurementtype

	cout << "-----------------------INSTRUCTIONS-----------------------" << endl;
	cout << "To START measurement hit 's' and press 'ENTER'." << endl;
	cout << "STOP measurement or programm by entering 'q' and 'ENTER'." << endl;
	

	while(getchar() != 'q' || 's'){
				sleep(1);
			}

	if (getchar() == 'q')
	{
		//---------------------------Close devices---------------------------//

		cout << "Exiting program..." << endl;

		Ham1.Close();
		Ham2.Close();

		Peltier1.Close();
		Peltier2.Close();

		return 0;
	}

	if (getchar() == 's')
	{
		cout << "Choose a measurement:" << endl;
		cout << "Insitu Measurement: '1' + 'ENTER'" << endl;
		cout << "UI Curve: '2' +'ENTER' " << endl;
		cin >> measurementtype;

		if (measurementtype == 1)
		{
			InsituMeasurement(Ham1, Ham2, Peltier1, Peltier2);
		}

		else if (measurementtype == 2)
		{
			UICurve(Ham1, Ham2, Peltier1, Peltier2);
		}

		else{
			cout << "Exiting program..." << endl;
			continue;
		}
	}


	//---------------------------Close devices---------------------------//

	Ham1.Close();
	Ham2.Close();

	Peltier1.Close();
	Peltier2.Close();
	
	return 0;
}
