/*

SiPM.h


Created by Janine Müller on 14.10.2016


*/

#ifndef ____SiPM__
#define ____SiPM__

#include "../SourceMeter/SourceMeter.h"
#include "../PelztierControl/PelztierControl.h"
#include "../MultiMeter/MultiMeter.h"
#include "LogSiPM.h"

using namespace std;


class SiPM
{

public:
	SiPM(double biasVoltage, SourceMeter &SourceM, int smuX, Pelztier &Peltier);
	~SiPM();
	void Close();
	SourceMeter GetSourceMeter();
	Pelztier GetPelztier();
	double GetBiasVoltage();

	void Initialize(double biasVoltage, string currentlimit);
	void RampToBiasVoltage();

	vector<double> MeasureIV();
	double MeasureI();
	double MeasureV();

private:

	double _biasVoltage;
	SourceMeter _SourceM; // SourceMeter as voltage source for SiPM
	int _smuX; // which Output of SourceMeter
	Pelztier _Peltier; // Peltier element of SiPM
	LogSiPM _Logfile;

};


#endif /* defined(____SiPM__) */