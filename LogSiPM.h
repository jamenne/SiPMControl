/*

LogSiPM.h


Created by Janine Müller on 18.10.2016


*/

#ifndef ____LogSiPM__
#define ____LogSiPM__

#include <fstream>
#include <sstream>

using namespace std;


class LogSiPM
{

public:
	LogSiPM();
	~LogSiPM();
	void Close();

	void Initialize();

	void Write(vector<double> write);
	

private:

	stringstream _path;
	fstream _file;

};


#endif /* defined(____LogSiPM__) */