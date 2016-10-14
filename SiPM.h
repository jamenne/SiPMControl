/*

SiPM.h


Created by Janine Müller on 14.10.2016


*/

#ifndef ____Pelztier__
#define ____Pelztier__

#include "../SourceMeter/SourceMeter.h"
#include "../MultiMeter/MultiMeter.h"

using namespace std;


class Pelztier
{

public:
	Pelztier(SourceMeter &SourceM, int smuX, MultiMeter &MultiM);
	~Pelztier();
	void Initialize(int masterUD, int SourceMeterPad, int MultiMeterPad, string voltagelimit);

private:

	SourceMeter _SourceM; // SourceMeter as current source for peltier
	int _smuX; // which Output of SourceMeter
	MultiMeter _MultiM; // MultiMeter for 4 wire measurement of resistance


};


#endif /* defined(____Pelztier__) */