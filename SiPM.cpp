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
#include "LogSiPM.h"

#include "SiPM.h"

using namespace std;

//---------------------SiPM class---------------------//

// Constructor
SiPM::SiPM(double biasVoltage, SourceMeter &SourceM, int smuX, Pelztier &Peltier):_SourceM(SourceM), _smuX(smuX), _Peltier(Peltier)
{
	_biasVoltage=biasVoltage;

};

//Destructor
SiPM::~SiPM(){

};

// Init of Peltier and SourceMeter outside of SiPM Init.
void SiPM::Initialize(double biasVoltage, string currentlimit){

	this->_SourceM.SelectVoltageFunction(this->_smuX);
	this->_SourceM.SetCurrentLimit(this->_smuX,currentlimit);
	this->_SourceM.SetOutputOnOff(this->_smuX,true);

	this->_biasVoltage = biasVoltage;
}

void SiPM::Close(){
	this->_SourceM.SetSourceVoltage(this->_smuX, "0");
	this->_SourceM.SetOutputOnOff(this->_smuX,false);
	this->~SiPM();

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

	stringstream voltage;

	double actualvoltage = this->MeasureV();

	while(_biasVoltage != actualvoltage){

		if(_biasVoltage - actualvoltage > 5){

			voltage << actualvoltage+5;
			this->_SourceM.SetSourceVoltage(this->_smuX, voltage.str());
			voltage.str();
		}

		else if (_biasVoltage - actualvoltage < -5){

			voltage << actualvoltage-5;
			this->_SourceM.SetSourceVoltage(this->_smuX, voltage.str());
			voltage.str();
		}

		else{

			if (_biasVoltage != actualvoltage)
			{
				voltage << _biasVoltage;
				this->_SourceM.SetSourceVoltage(this->_smuX, voltage.str());
				voltage.str();
				actualvoltage = this->MeasureV();
				break;
			}

		}

		sleep(2);
		actualvoltage = this->MeasureV();
	}

}

vector<double> SiPM::MeasureIV(){

	vector<double> res = this->_SourceM.MeasureIV(this->_smuX);
	this->_Logfile.Write(res);
	
	return res;
}

double SiPM::MeasureI(){

	return this->_SourceM.MeasureI(this->_smuX);
}

double SiPM::MeasureV(){

	return this->_SourceM.MeasureV(this->_smuX);
}








