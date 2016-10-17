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
SiPM::SiPM(double biasVoltage, SourceMeter &SourceM, int smuX, Pelztier &Peltier):_SourceM(SourceM), _smuX(smuX), _Peltier(Peltier)
{
	_biasVoltage=biasVoltage;

};

//Destructor
SiPM::~SiPM(){};

// Init of Peltier and SourceMeter outside of SiPM Init.
void SiPM::Initialize(double biasVoltage, string currentlimit){

	this->_SourceM.SelectVoltageFunction(this->_smuX);
	this->_SourceM.SetCurrentLimit(this->_smuX,currentlimit);
	this->_SourceM.SetOutputOnOff(this->_smuX,true);

	this->_biasVoltage = biasVoltage;
}

SourceMeter SiPM::GetSourceMeter(){

	return this->_SourceM;
}

Pelztier SiPM::GetPelztier(){

	return this->_Peltier;
}

double SiPM::GetBiasVoltage(){

	return this->_biasVoltage;
}

void SiPM::RampToBiasVoltage(){

	string biasVolt;
	this->_SourceM.SetSourceVoltage(this->_smuX, biasVolt);
}

vector<double> SiPM::MeasureIV(){

	return this->_SourceM.MeasureIV(this->_smuX);
}

double SiPM::MeasureI(){

	return this->_SourceM.MeasureI(this->_smuX);
}

double SiPM::MeasureV(){

	return this->_SourceM.MeasureV(this->_smuX);
}









