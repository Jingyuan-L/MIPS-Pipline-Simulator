#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>
#include <cstring>
#include <algorithm>

using namespace std;

int main (int argc, char** argv) {

	//char c1[11] = "config.txt";
	//argv[1] = c1;
	//char c2[10] = "trace.txt";
	//argv[2] = c2;

	ifstream config;
	config.open(argv[1]);

	int m, k;
	config >> m >> k;
 
	config.close();

	ofstream out;
	string out_file_name = string(argv[2]) + ".out";
	out.open(out_file_name.c_str());
	
	ifstream trace;
	trace.open(argv[2]);


	int c = pow(2,k);
	int myBHR = c - 1;
	int sizePHT = pow(2,m);
	//cout << sizePHT << endl;
	//cout << myBHR << endl;
	//myBHR = (myBHR << 1)% c;
	//cout << myBHR << endl;
	
	vector <vector <int> > myPHT(pow(2,m), vector<int>(pow(2,m), 3));
	vector <unsigned long> myPC;


	unsigned long pc; bool taken;
	trace >> std::hex >> pc >> taken;

	int n = 0;
	int i = 0;
	while (!trace.eof() ) {
		i++;
		//cout << pc << " " << taken <<endl;
		bool prediction;
		int pcPHTaddr = pc % sizePHT;
		//cout << i << " " << pcPHTaddr <<  " " << taken << " " ;
		auto pcVecIndex = find(myPC.begin(), myPC.end(), pcPHTaddr);
		int pcIndex = distance(myPC.begin(), pcVecIndex);
		if (pcVecIndex == myPC.end())
		{
			myPC.push_back(pcPHTaddr);
			for (int i = 1; i <= pow(2,k); i++)
			{
				myPHT[n].push_back(3);
			}
			prediction = 1;
			//cout << myBHR << "(old) ";
			if (taken == 0)
			{
				myPHT[n][myBHR] = 2;
				myBHR = (myBHR << 1)% c;
			}
			else
			{
				myBHR = ((myBHR << 1)+ 1)% c;
			}
			n++;
		}
		else
		{
			
			//cout << myPHT[pcIndex][myBHR] << "PHT " ;
			//cout << myBHR << "(old) ";
			if (myPHT[pcIndex][myBHR] == 0)//SN
			{
				prediction = 0;
				if (taken == 0)
				{
					myPHT[pcIndex][myBHR] = 0;//next SN
					myBHR = (myBHR << 1)% c;
				}
				else
				{
					myPHT[pcIndex][myBHR] = 1;//next WN
					myBHR = ((myBHR << 1)+ 1)% c;
				}
			}
			else if (myPHT[pcIndex][myBHR] == 1)//WN
			{
				prediction = 0;
				if (taken == 0)
				{
					myPHT[pcIndex][myBHR] = 0;//next SN
					myBHR = (myBHR << 1)% c;
				}
				else
				{
					myPHT[pcIndex][myBHR] = 2;//next WT
					myBHR = ((myBHR << 1)+ 1)% c;
				}
			}
			else if (myPHT[pcIndex][myBHR] == 2)//WT
			{
				prediction = 1;
				if (taken == 0)
				{
					myPHT[pcIndex][myBHR] = 0;//next SN
					myBHR = (myBHR << 1)% c;
				}
				else
				{
					myPHT[pcIndex][myBHR] = 3;//next ST
					myBHR = ((myBHR << 1)+ 1)% c;
				}
			}
			else //ST 
			{
				prediction = 1;
				if (taken == 0)
				{
					myPHT[pcIndex][myBHR] = 2;//next WT
					myBHR = (myBHR << 1)% c;
				}
				else
				{
					myPHT[pcIndex][myBHR] = 3;//next ST
					myBHR = ((myBHR << 1)+ 1)% c;
				}
			}
		}
		//prediction = true;
		out << prediction << endl;
		//cout << prediction << " " << myBHR << "(new)" << endl;
		trace >> std::hex >> pc >> taken;
	}
	 
	//cout << n << endl;
	trace.close();	
	out.close();
}
