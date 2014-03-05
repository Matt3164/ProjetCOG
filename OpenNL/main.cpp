/*
 * main.cpp
 *
 *  Created on: 2 mars 2014
 *      Author: matthieu
 */

#include <iostream>
#include "nl.h"


using namespace std;
int main(){
	nlNewContext();

	nlSolverParameteri(NL_NB_VARIABLES, 3);
	nlSolverParameteri(NL_SOLVER, NL_BICGSTAB);
	nlSolverParameteri(NL_LEAST_SQUARES, NL_TRUE);
	nlSolverParameteri(NL_MAX_ITERATIONS, 10);
	//nlSolverParameterd(NL_THRESHOLD, 0.1);

	nlBegin(NL_SYSTEM);
	nlSetVariable(0,0.1);
	nlSetVariable(1,0.1);
	nlSetVariable(2,0.1);

	//nlLockVariable(1);
	//nlUnlockVariable(1);

	nlBegin(NL_MATRIX);

	for(int i = 1; i <=3; i++){
			nlRowParameterd(NL_RIGHT_HAND_SIDE, 5.);

			nlBegin(NL_ROW);
			nlCoefficient(0, 1.);
			nlCoefficient(1, 1.);
			nlCoefficient(2, 1.);
			nlEnd(NL_ROW);

	}

	nlEnd(NL_MATRIX);
	nlEnd(NL_SYSTEM);

	nlSolve();

	cout << "Les résultats sont : ";
	cout << "0 : " << nlGetVariable(0) << endl;
	cout << "1 : " << nlGetVariable(1) << endl;
	cout << "2 : " << nlGetVariable(2) << endl;


	cout << "Modif 1" << endl;

	cout << "Hello World" << endl;

}



