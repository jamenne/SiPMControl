/*

SiPM.h


Created by Janine Müller on 14.10.2016


*/

#ifndef ____SiPM__
#define ____SiPM__

#include "../SourceMeter/SourceMeter.h"
#include "../PelztierControl/PelztierControl.h"
#include "../MultiMeter/MultiMeter.h"
#include "../LogFileDev/LogDev.h"


class SiPM
{

public:
	SiPM(double biasVoltage, SourceMeter &SourceM, int smuX, Pelztier &Peltier);
	~SiPM();
	void Close();
	SourceMeter& GetSourceMeter();
	Pelztier& GetPelztier();
	LogDev& GetLogFile();
	double GetBiasVoltage();

	void Initialize(double biasVoltage, const std::string currentlimit);
	void RampToVoltage(double ramp_volt);
	void RampToBiasVoltage();
	void RampDownVoltage();

	void SetSourceVoltage(double set_voltage);

	vector<double> MeasureIV();
	double MeasureI();
	double MeasureV();

private:

	double _biasVoltage;
	SourceMeter& _SourceM; // SourceMeter as voltage source for SiPM
	int _smuX; // which Output of SourceMeter
	Pelztier& _Peltier; // Peltier element of SiPM
	LogDev _LogFile;

};


#endif /* defined(____SiPM__) */