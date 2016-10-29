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

#include "SiPM.h"

using namespace std;

//---------------------SiPM class---------------------//

// Constructor
SiPM::SiPM(double biasVoltage, SourceMeter &SourceM, int smuX, Pelztier &Peltier) :
	_biasVoltage(biasVoltage),
	_SourceM(SourceM), 
	_smuX(smuX), 
	_Peltier(Peltier),
	_LogFile()
{


};

//Destructor
SiPM::~SiPM(){

};

// Init of Peltier and SourceMeter outside of SiPM Init.
void SiPM::Initialize(const string currentlimit){

	this->_SourceM.SelectVoltageFunction(this->_smuX);
	this->_SourceM.SetCurrentLimit(this->_smuX,currentlimit);
	this->_SourceM.SetOutputOnOff(this->_smuX,true);

	stringstream ss;
	ss << "SiPM_" << this->_smuX;

	this->_LogFile.Initialize(ss.str().c_str());
	this->_LogFile.WriteString("#time\ttemperature\tcurrent\tvoltage");
}

void SiPM::Close(){
	this->RampDownVoltage();
	this->_SourceM.SetSourceVoltage(this->_smuX, "0");
	this->_SourceM.SetOutputOnOff(this->_smuX,false);

}

SourceMeter& SiPM::GetSourceMeter(){

	return this->_SourceM;
}

Pelztier& SiPM::GetPelztier(){

	return this->_Peltier;
}

LogDev& SiPM::GetLogFile(){

	return this->_LogFile;
}

double SiPM::GetBiasVoltage(){

	return this->_biasVoltage;
}

void SiPM::SetSourceVoltage(double set_voltage){
	stringstream voltage;
	voltage << set_voltage;

	this->_SourceM.SetSourceVoltage(this->_smuX, voltage.str());
	voltage.str("");

}

void SiPM::RampToVoltage(double ramp_volt){

	stringstream out;

	out << "Start to ramp up to " << ramp_volt << "V!";
	cout << out.str() << endl;
	this->_SourceM.GetLogFile().Write(out.str());

	stringstream voltage;

	double actualvoltage = this->MeasureV();

	while(ramp_volt != actualvoltage){

		if(ramp_volt - actualvoltage > 5){

			voltage << actualvoltage+5;
			this->_SourceM.SetSourceVoltage(this->_smuX, voltage.str());
			voltage.str("");
		}

		else if (ramp_volt - actualvoltage < -5){

			voltage << actualvoltage-5;
			this->_SourceM.SetSourceVoltage(this->_smuX, voltage.str());
			voltage.str("");
		}

		else{

			if (ramp_volt != actualvoltage)
			{
				voltage << ramp_volt;
				this->_SourceM.SetSourceVoltage(this->_smuX, voltage.str());
				voltage.str("");
				actualvoltage = this->MeasureV();
				break;
			}

		}

		sleep(2);
		actualvoltage = this->MeasureV();
	}

}

void SiPM::RampToBiasVoltage(){

	stringstream out;

	out << "Start to ramp up to biasVoltage!";
	cout << out.str() << endl;
	this->_SourceM.GetLogFile().Write(out.str());

	stringstream voltage;

	double actualvoltage = this->MeasureV();

	while(_biasVoltage != actualvoltage){

		if(_biasVoltage - actualvoltage > 5){

			voltage << actualvoltage+5;
			this->_SourceM.SetSourceVoltage(this->_smuX, voltage.str());
			voltage.str("");
		}

		else if (_biasVoltage - actualvoltage < -5){

			voltage << actualvoltage-5;
			this->_SourceM.SetSourceVoltage(this->_smuX, voltage.str());
			voltage.str("");
		}

		else{

			if (_biasVoltage != actualvoltage)
			{
				voltage << _biasVoltage;
				this->_SourceM.SetSourceVoltage(this->_smuX, voltage.str());
				voltage.str("");
				actualvoltage = this->MeasureV();
				break;
			}

		}

		sleep(2);
		actualvoltage = this->MeasureV();
	}

}

void SiPM::RampDownVoltage(){

	stringstream out;

	out << "Start to ramp down voltage to 0!";
	cout << out.str() << endl;
	this->_SourceM.GetLogFile().Write(out.str());

	stringstream voltage;

	this->_biasVoltage = 0;

	double actualvoltage = this->MeasureV();

	while(_biasVoltage != actualvoltage){

		if(_biasVoltage - actualvoltage > 5){

			voltage << actualvoltage+5;
			this->_SourceM.SetSourceVoltage(this->_smuX, voltage.str());
			voltage.str("");
		}

		else if (_biasVoltage - actualvoltage < -5){

			voltage << actualvoltage-5;
			this->_SourceM.SetSourceVoltage(this->_smuX, voltage.str());
			voltage.str("");
		}

		else{

			if (_biasVoltage != actualvoltage)
			{
				voltage << _biasVoltage;
				this->_SourceM.SetSourceVoltage(this->_smuX, voltage.str());
				voltage.str("");
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
	double temp = this->_Peltier.GetTemperature();
	this->_LogFile.WriteDoubleAndVector(temp, res);
	
	return res;
}

double SiPM::MeasureI(){

	return this->_SourceM.MeasureI(this->_smuX);
}

double SiPM::MeasureV(){

	return this->_SourceM.MeasureV(this->_smuX);
}








