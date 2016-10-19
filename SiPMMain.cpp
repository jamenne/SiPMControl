/*

MultiMeterMain.cpp

Created by Janine Müller on 07.10.2016

*/

#include <iostream>
#include <fcntl.h>
#include <cstdlib>
#include <iomanip>

#include "../gpib/gpib.h"
#include "SiPM.h"

using namespace std;

int main(int argc, char const *argv[])
{

	// ----------Flags for while(true) interrupt------------- //
	int fd = STDIN_FILENO;
	int flags = fcntl(fd, F_GETFL, 0);
	fcntl(fd, F_SETFL, flags | O_NONBLOCK);
	int masterUD = InitializeMaster();

	// -------------------------SourceMeter----------------------------//
	SourceMeter SourceM;

	int SourceMeterPad = 26;

	SourceM.Initialize(masterUD, SourceMeterPad);



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

	Ham.RampToBiasVoltage();


	//-------------------Measurement loop----------------------//


	vector<double> measure(2,0);

	// variables for the temperature control of the peltier element
	double temp_target = 5;
	int index = 0;
	double integral = 0;
	vector<double> TempDiff(10,0);
	double current = 0;

	int meas_timer = 0;
	int logfile_timer = 0;

	do{

		Peltier.OneTempControl(TempDiff, integral, index, current, temp_target);

		if (meas_timer == 60) // sleep 1 sec * 60 = measure IV every minute
		{
			measure = Ham.MeasureIV();
			meas_timer = 0;
		}		

		if (logfile_timer == 60*60*24) // every day a new logfile
		{
			Peltier.GetLogFile().Initialize("PelztierControl");
			logfile_timer = 0;
		}


		sleep(1);
		meas_timer++;
		logfile_timer++;

	}while(getchar() != 'q');

	Ham.Close();

	Peltier.Close();


	
	return 0;
}
