#include "BH_cross_sections.h"
#include "stdio.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <iomanip>

using namespace std;

int main(int argc, char *argv[])
{

	double PI = 3.1415926;
	double DEG = PI/180.0;

	// Number of points in plots
	int Npts = 120;
	//cout << "Set number of points:" << endl:
	//cin >> Npts;
	double Theta;
	double Phi = 180.0*DEG;
	
	cerr << "Theta for both electron and positron is (in degree 3 to 13): " << endl;
	cin >> Theta;
	cerr << endl;
	Theta*=DEG;

	cerr << "Theta = " << Theta/DEG << ", Phi = " << Phi/DEG << endl;
	cerr << "In radians: Theta = " << Theta << ", Phi = " << Phi << endl;

	// Needed for cross section constructor
	double Me = 0.511;
	double Egamma = 60.0;
	double Z = 92.0;
	double Per;
	// These are used for setting the range of points will be plot in the TGraph
	// Since the maximum/minmum of asymmetry is near Del = 0, the data near Del = 0
	// will be closer to real shape if they're denser.
	cerr << "How many percent of largest range will be plot? (0, 100]" << endl;
	cin >> Per;
	Per/= 100.0;

	double E_p[Npts];
	double E_q[Npts];
	double Del[Npts];
	double Xsec[Npts];
	double Xsec_s[Npts];
	double Xsec_a[Npts];
	double Asym[Npts];

	BH_cross_sections* XS = new  BH_cross_sections(Z, Egamma);

	for (int i = 0.0; i < Npts; ++i)
	{
		E_p[i] = (Egamma/2.0)-(Egamma/2.0)*Per+Egamma*Per*(double)i/((double)Npts);
		if(E_p[i] > Me && E_p[i] < Egamma-Me){
			E_q[i] = Egamma - E_p[i];
			Del[i] = E_p[i] - E_q[i];
			Xsec[i] = XS->xsec_full(E_p[i], E_q[i], Theta, Theta, Phi);
			Xsec_s[i] = XS->xsec_s(E_p[i], E_q[i], Theta, Theta, Phi);
			Xsec_a[i] = XS->xsec_a(E_p[i], E_q[i], Theta, Theta, Phi);
			Asym[i] = Xsec_a[i]/Xsec_s[i];
		}
	}

	// Now put all data into a .dat file.
	ifstream runin;
	ofstream runout;
	int runno;
	// if the file "events.runno" exists - use its contents as the run number
	runin.open("events.runno");
	if(runin.good())
		{
		runin>>runno;
		runin.close();
		}
	else
		{
		runno = 0;
		}
	runno++;
	runout.open("events.runno", ios::trunc);
	if(runout.good())
		{
		runout << runno << endl;
		runout.close();
		}
	else
		{
		cout << "Cannot open events.runno" << endl;
		}

	char filename[100];
	sprintf(filename, "Xsec.run%.3d.dat", runno);
	ofstream Out;
	Out.open(filename);

	//string filename;
	//ostringstream OutFileName(filename);
	//OutFileName << "Xsec.run" << Form("%.3d.dat", runno);			
	//ofstream Out;
	//Out.open(filename.c_str());


	Out << setw(20) << "Npts=" << Npts << setw(20) << "Theta=" << Theta/DEG <<  setw(20) << "Phi=" << Phi/DEG << endl;
	Out << setw(20) << "Del" << setw(20) << "Xsec" << setw(20) <<"Xsec_s" << setw(20) << "Xsec_a" << setw(20) <<"Asymmetry" << endl;
	for (int i = 0; i < Npts; i++){
		Out << setw(20) << Del[i] << setw(20) << Xsec[i] << setw(20) << Xsec_s[i] << setw(20) << Xsec_a[i] << setw(20) << Asym[i] << endl;
	}
	Out.close();
}
