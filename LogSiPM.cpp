/*

LogSiPM.cpp


Created by Janine Müller on 18.10.2016

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

#include "LogSiPM.h"


using namespace std;

//---------------------LogSiPM class---------------------//

// Constructor
LogSiPM::LogSiPM(){

};

//Destructor
LogSiPM::~LogSiPM(){

};

// Init of Peltier and SourceMeter outside of LogSiPM Init.
void LogSiPM::Initialize(){

	time_t sec = time(NULL);

	tm *uhr = localtime(&sec);

	this->_path << "../data/LogSiPM_" << uhr->tm_year-100 << uhr->tm_mon+1 << uhr->tm_mday << "-" << uhr->tm_hour << uhr->tm_min << uhr->tm_sec << ".txt";
}

void LogSiPM::Close(){

	this->_file.close();
	this->~LogSiPM();
}

void LogSiPM::Write(vector<double> write){

	time_t sec = time(NULL);
	tm *uhr = localtime(&sec);

	this->_file.open(this->_path.str().c_str(), fstream::in | fstream::out | fstream::app);

	this->_file << uhr->tm_year-100 << uhr->tm_mon+1 << uhr->tm_mday << "-" << uhr->tm_hour << uhr->tm_min << uhr->tm_sec << "\t" << write[0] << "\t" << write[1] << endl;

	this->_file.close();

}












