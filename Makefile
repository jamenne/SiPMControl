CXX		= g++
CXXFLAGS	= -O2 -Wall 
LDFLAGS		= -lgpib
SRCFILES	= SiPM.cpp SiPMMain.cpp
OBJFILES    = SiPM.o SiPMMain.o ../gpib/gpib.o ../MultiMeter/MultiMeter.o ../SourceMeter/SourceMeter.o ../PelztierControl/PelztierControl.o

SiPM:		$(OBJFILES)
		$(CXX) $(OBJFILES) $(LDFLAGS) -o $@

clean:
		rm -f $(OBJFILES) SiPM 

SiPM.o:	SiPM.cpp SiPM.h

SiPMMain.o: SiPMMain.cpp SiPM.h ../gpib/gpib.h ../MultiMeter/MultiMeter.h ../SourceMeter/SourceMeter.h ../PelztierControl/PelztierControl.h
