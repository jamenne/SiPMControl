/*

MultiMeterMain.cpp

Created by Janine Müller on 07.10.2016

*/

#include <iostream>

#include "../MultiMeter/MultiMeter.h"
#include "../SourceMeter/SourceMeter.h"
#include "../gpib/gpib.h"
#include "../PelztierControl/PelztierControl.h"
#include "SiPM.h"

int main(int argc, char const *argv[])
{

	int masterUD = InitializeMaster();

	SourceMeter SourceM;

	int SourceMeterPad = 26;

	SourceM.Initialize(masterUD, SourceMeterPad);

	MultiMeter MultiM;

	// SourceMeter, Output, MultiMeter
	Pelztier Peltier(SourceM, 1, MultiM);

	// masterUD, SourceMeterPad, MultiMeterPad, voltagelimit
	Peltier.Initialize(masterUD, 18, "0.6");

	double biasVoltage = 50; //Volt

	SiPM Ham(biasVoltage, SourceM, 2, Peltier);



	
	return 0;
}
