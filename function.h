
#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include<sstream>
#include<algorithm>
using namespace std;

class Location {
public:
    int idCustomer;
    double deman;
	int id;
	double windowStartTime;
	double windowEndTime;
	double insertionCost[50][100]; //insertionCost[i][j] chi phi khi chenh lech khi chen them diem vao vi tri j trong tua i.
	long double xCoor, yCoor;
	void printTimeWindow() {
       cout << id <<"(" << xCoor <<","<<yCoor<<") "
           << " [" << windowStartTime << "," << windowEndTime << "] \t";

    }
};

class Vehicle {
public:
	double capacity;
	double currentCapacity;
	vector<Location> route;

	double arrivalTime[100];
	double startWorkingTime[100];
	double waitingTime[100];
	double maxDelay[100];
	double removalCost[100];

};

class Solution{
public:

	int nbOfCustomer;
	int nbOfCusServiced;
	int nbOfLocation;
	int timeHorizon;

	int nbOfVehicle;
    bool isInserted[100];
    double obj;
	vector<Vehicle> vehicleList;

	vector<Location> locationList;

	double costBtwLocation[500][500];
    double timeTravel[500][500];

	void ReadProblem(char* filename);

	void CalculateChronoInfo();
    void CalculateMaxDelay();
    void CalculateInsertionCost();
    void CalculateRemovalCost();
    void InitSolution();
    double RegretOne();
    double Regret2();
    double Regret3();
    void Objective();
    void RandomRemoval(int nbOfRemove);
    void PrintInput();
    void WorstRemoval(int q, double p);
    void ShawRemoval(int q, double p);
};
class SortingHelper{
public:

	double value;
	int firstIndex;
	int secondIndex;

	void showInfo(){
		cout  << value <<"  fistIndex "<<firstIndex <<" second index: " << secondIndex  <<endl;
	}
};
class D_listRemove
{
public:
    int indexRoute;
    int indexPos;
};
