//============================================================================
// Name        : Drv.cpp
// Author      : Grau
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>
#include <math.h>
using namespace std;

double Framp = 1.0;							// Hz per turn
double Ftarget = 100; 						// target frequency in Hz
double Fstep = 1200;							// discritization freq
double Tstep = 1.0 / Fstep;					// period of one step
double Fnom = 50.0;							// nominal work frequency of drive
double Umax = 100.0;						// voltage limiter in percents
double Uburst = 10.0;
int	PointsForModel = 240000;
double Framp_per_step = Framp / Fstep;

#define PI 3.1415926L


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

	static double PhiCur = 0;

	double dPhi_next = 2*PI*Fnext*Tstep;
	double PhiNext = fmod ((PhiCur + dPhi_next), (2*PI));

	double d_next = Unext * cos(PhiNext);
	double q_next = Unext * sin(PhiNext);

	CurFreq = Fnext;
	Ucur = Unext;
	PhiCur = PhiNext;



	OutFile << Fnext << "; " << Unext<< "; " << PhiNext;
	OutFile << endl;
	OutFile.flush();
}

int main() {
	OutFile.open("OutData.txt");
	OutFile << "Freq; Voltage; Angle\n";

	int i=0;
	for(; i<PointsForModel; ++i){
		StepProc();
	}

	cout << "total " << i <<" steps" << endl; // prints !!!Hello World!!!
	OutFile.close();
	return 0;
}
