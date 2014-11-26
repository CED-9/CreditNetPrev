// Test generating a graph

#include<random>
#include"CN_Graph.h"
#include<iostream>
using namespace std;

int genTrans(int num);

int main(int argc, char* argv[]){

	/*
	if (argc<3){ cout << "Not enough input" << endl; return 0; }
	int finNum = atoi(argv[1]);
	int conNum = atoi(argv[2]);
	int proNum = atoi(argv[3]);
	*/

	int finNum = 5;
	int conNum = 3;
	int proNum = 3;

	cout << "finNum " << finNum << " conNum " << conNum << " proNum " << proNum << endl;

	Graph creditNet(finNum, conNum, proNum);
	creditNet.genErdosRenyiGraph();
	creditNet.print();

	system("pause"); 
	return 0;
}

