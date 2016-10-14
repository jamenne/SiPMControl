/*

MultiMeterMain.cpp

Created by Janine Müller on 07.10.2016

*/

#include <iostream>

#include "../MultiMeter/MultiMeter.h"
#include "../SourceMeter/SourceMeter.h"
#include "../gpib/gpib.h"
#include "..Pelztier/PelztierControl.h"

int main(int argc, char const *argv[])
{

	int masterUD = InitializeMaster();

	SourceMeter SourceM;

	MultiMeter MultiM;

	
	return 0;
}
