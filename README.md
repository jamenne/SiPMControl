# SiPM
Author: Janine Menne

## Description
C++ class to speak with a Hamamatsu SiPM. A SiPM is a solid-state single-photon-sensitive device built from an avalanche photodiode (APD) array on common silicon substrate, which is used for single photon detection.  
The SiPM is powered with the help of a Keithley SourceMeter. 
A cooling of the SiPM is possible with a peltier element, which is powered with a Keithley SourceMeter and read out by a Keithley MultiMeter.
  
This project was used for an in situ irradiation of scintillating fibres. Two SiPMs were connected and read out over a year and the data saved to disk.

The measurement devices are connected to the PC via gpib.

## Requirements to run terminal program

Availability of the following libraries (author: Janine Menne):  
* [SourceMeter][1]  
* [MultiMeter][2]  
* [Peltier][3]  
* [LogFileDev][4]  
* [gpib][5] 





[1]: https://github.com/jamenne/Keithley_SourceMeter_Control
[2]: https://github.com/jamenne/Keithley_MultiMeter_Control
[3]: https://github.com/jamenne/PeltierElementControl
[4]: https://github.com/jamenne/Device_Logfiles  
[5]: https://github.com/jamenne/gpib  