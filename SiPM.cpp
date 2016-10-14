/*

SiPM.cpp


Created by Janine Müller on 14.10.2016

*/

#include <gpib/ib.h>
#include <gpib/gpib_user.h>
#include <string>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <fstream>
#include "time.h"
#include <sstream>
#include <cmath>

#include "../PelztierControl/PelztierControl.h"
#include "../MultiMeter/MultiMeter.h"
#include "../SourceMeter/SourceMeter.h"

#include "SiPM.h"

using namespace std;

//---------------------SiPM class---------------------//

// Constructor
SiPM::SiPM(SourceMeter &SourceM, int smuX, MultiMeter &MultiM):_SourceM(SourceM), _smuX(smuX), _MultiM(MultiM)
{
	
};

//Destructor
SiPM::~SiPM(){};

void SiPM::Initialize(int masterUD, int SourceMeterPad, int MultiMeterPad, string voltagelimit){

	_SourceM.Initialize(masterUD, SourceMeterPad);
	_SourceM.SelectCurrentFunction(this->_smuX);
	_SourceM.SetVoltageLimit(this->_smuX,voltagelimit);
	_SourceM.SetOutputOnOff(this->_smuX,true);

	_MultiM.Initialize(masterUD, MultiMeterPad);
	_MultiM.Set4WireFunction();
	_MultiM.SetAutorange4Wire();
	_MultiM.SetTriggerContinously();

}