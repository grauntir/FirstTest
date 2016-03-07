//============================================================================
// Name        : Drv.cpp
// Author      : Grau
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>
using namespace std;

double Framp = 1.0;							// Hz per turn
double Ftarget = 100; 						// target frequency in Hz
double Fstep = 120;							// discritization freq
double Tstep = 1.0 / Fstep;					// period of one step
double Fnom = 50.0;							// nominal work frequency of drive
double Umax = 100.0;						// voltage limiter in percents
double Uburst = 10.0;
int	PointsForModel = 24000;
double Framp_per_step = Framp / Fstep;


	ofstream OutFile;

void StepProc(){
	static double CurFreq = 0;
	double Fnext = CurFreq;

	if(CurFreq != Ftarget){
		if(Ftarget-CurFreq < Framp_per_step){
			Fnext = Ftarget;
		}else{
			if(Ftarget > CurFreq){
				Fnext = CurFreq + Framp_per_step;
			}else{
				Fnext = CurFreq - Framp_per_step;
			}
		}
	}

	static double Ucur = 0;
	double Unext = Ucur;
	if(Fnext > Fnom){
		Unext = Umax;
	}else{
		Unext = ((Umax - Uburst)*Fnext/Fnom) + Uburst;
	}

	CurFreq = Fnext;
	Ucur = Unext;




	OutFile << Fnext << "; " << Unext;
	OutFile << endl;
	OutFile.flush();
}

int main() {
	OutFile.open("OutData.txt");
	OutFile << "Freq; Voltage\n";

	int i=0;
	for(; i<PointsForModel; ++i){
		StepProc();
	}

	cout << "total " << i <<" steps" << endl; // prints !!!Hello World!!!
	OutFile.close();
	return 0;
}
