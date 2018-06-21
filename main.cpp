#include <iostream>
#include <string>
#include "function.h"
#include <conio.h>
#include<stdlib.h>
#include<ctime>
using namespace std;



int main()
{
/*
    sortingHelper *p = new sortingHelper[5];

	p[0].value = 20.9;
	p[1].value = 19.9;
	p[2].value = 22.9;
	p[3].value = 24.8;
	p[4].value = 23.5;

	for(int i=0; i<5; i++){
		p[i].showInfo();
	}

    cout << "\n merge sort" << endl;
    mergeSort(p,5);
    for(int i=0; i<5; i++){
		p[i].showInfo();
	}

*/


   // srand (time(NULL));
    Solution *s = new Solution();
    char *filename = "instance_8-triangle.txt";
    s->ReadProblem(filename);
    s->InitSolution();

   // in moi route
	for(int i = 0; i < s->nbOfVehicle; i++)
    {
        cout <<"route " << i << ": " ;
        for(int j =0; j < s->vehicleList[i].route.size();j++)
        {
            cout <<s->vehicleList[i].route[j].id << " ";
        }
        cout << endl;
    }
    //in ra tat ca id của location trong locationlist
    for(int i =0; i < s->locationList.size();i++)
    {
        cout <<s->locationList[i].id << " ";

    }
    cout << endl;

        int solancaithien =0;
        int kocaithien =0;
    double costTm = 0;
    while(s->locationList.size()!=0 && costTm != 99999999.0)
    {
        s->CalculateChronoInfo();
        s->CalculateMaxDelay();
        s->CalculateInsertionCost();
        costTm = s->RegretOne();
      //costTm = s->Regret2();
       // costTm = s->Regret3();
    }
        s->Objective();
        cout <<"khoi tao regret3 obj = "<<s->obj<<endl << endl;

        for(int i = 0; i < s->nbOfVehicle; i++)
    {
        cout <<"route " << i << ": " ;
        for(int j =0; j < s->vehicleList[i].route.size();j++)
        {
            cout <<s->vehicleList[i].route[j].id << " ";
        }
        cout << endl;
    }


    //LNS

    int nbLoops = 0;
    while(nbLoops < 10000)
    {

        //copy solution s to solution potential:
        Solution *potential        = new Solution();
        potential->nbOfCustomer    = s->nbOfCustomer;
        potential->nbOfCusServiced = s->nbOfCusServiced;
        potential->nbOfLocation    = s->nbOfLocation;
        potential->timeHorizon     = s->timeHorizon;
        potential->nbOfVehicle     = s->nbOfVehicle;
        for(int i =0; i < 100; i++)
        {
            potential->isInserted[i] = s->isInserted[i];
        }
        potential->obj = s->obj;
      //  vector<Vehicle> vehicleList;
        for(int i =0; i <s->vehicleList.size(); i++)
        {
            potential->vehicleList.push_back(s->vehicleList[i]);
        }

        //vector<Location> locationList;
        for(int i = 0; i < s->locationList.size(); i++)
        {
            potential->locationList.push_back(s->locationList[i]);
        }

        //double costBtwLocation[500][500];
        //double timeTravel[500][500];
        for(int i = 0; i <potential->nbOfLocation; i++)
        {
            for(int j =0; j <potential->nbOfLocation; j++)
            {
                potential->costBtwLocation[i][j] = s->costBtwLocation[i][j];
                potential->timeTravel[i][j]      = s->timeTravel[i][j];
            }
        }

        //chon ngau nhien mot trong cac Regret va removal
        int rdRepair = -1;
        int rdUpdate = -1;
        int q = -1;        //so location bi loai bo trong mot solution
        rdRepair = rand()%3;    //rdRepair in the range 0 to 2
        rdUpdate = rand()%3;
        q = rand()%((s->nbOfCusServiced)/2 - 4 +1) + 4;         // random tu a den b, su dụng: a + rand()%(b-a+1)

    //cout <<"rdR rdU q: " << rdRepair <<" "<< rdUpdate<<" " <<q << endl;

        //destroysolutiion

        if(rdUpdate==0)
        {
            //cout<<"ramdomremoval"<<endl;
           potential->RandomRemoval(q);

        }
        else if(rdUpdate==1)
        {
           // cout<<"WostRemoval"<<endl;
            potential->CalculateRemovalCost();
            potential->WorstRemoval(q,6);
        }
        else if(rdUpdate==2)
        {

                potential->ShawRemoval(q,6);

        }

        //repair
        if(rdRepair==0)
        {
             double costTm = 0;
            while(potential->locationList.size()!=0 && costTm != 99999999.0)
            {
                potential->CalculateChronoInfo();
                potential->CalculateMaxDelay();
                potential->CalculateInsertionCost();
                costTm = potential->RegretOne();

            }
        }
        else if(rdRepair ==1)
        {
             double costTm = 0;
            while(potential->locationList.size()!=0 && costTm != 99999999.0)
            {
                potential->CalculateChronoInfo();
                potential->CalculateMaxDelay();
                potential->CalculateInsertionCost();
                costTm = potential->Regret2();

            }
        }
        else
        {
             double costTm = 0;
            while(potential->locationList.size()!=0 && costTm != 99999999.0)
            {
                potential->CalculateChronoInfo();
                potential->CalculateMaxDelay();
                potential->CalculateInsertionCost();
                costTm = potential->Regret3();

            }
        }

        //kiem tra xem solution moi co tot hon khong
        potential->Objective();

        if(potential->nbOfCusServiced > s->nbOfCusServiced || (potential->nbOfCusServiced == s->nbOfCusServiced && potential->obj < s->obj))
        {
            solancaithien++;

            //copy solution potential to s:
            s->nbOfCustomer     = potential->nbOfCustomer;
            s->nbOfCusServiced  = potential->nbOfCusServiced;
            s->nbOfLocation     = potential->nbOfLocation  ;
            s->timeHorizon      = potential->timeHorizon     ;
            s->nbOfVehicle      = potential->nbOfVehicle    ;
            for(int i =0; i < 100; i++)
            {
                s->isInserted[i] = potential->isInserted[i];
            }
            s->obj = potential->obj;
            cout << "Improved! obj = " << s->obj<<endl ;
          //  vector<Vehicle> vehicleList;
            s->locationList.clear();
            s->vehicleList.clear();
            for(int i =0; i <potential->vehicleList.size(); i++)
            {
                s->vehicleList.push_back(potential->vehicleList[i]);
            }

            //vector<Location> locationList;
            for(int i = 0; i < potential->locationList.size(); i++)
            {
                s->locationList.push_back(potential->locationList[i]);
            }

            //double costBtwLocation[500][500];
            //double timeTravel[500][500];
            for(int i = 0; i <500; i++)
            {
                for(int j =0; j <500; j++)
                {
                    s->costBtwLocation[i][j] = potential->costBtwLocation[i][j];
                    s->timeTravel[i][j]      = potential->timeTravel[i][j];
                }
            }

        }
        else{ kocaithien++;}
        nbLoops++;
        delete(potential);
    }

    cout <<"cai thien = " <<solancaithien <<"\t ko cai thien = " << kocaithien<<endl;



    //s->Objective();


     //in moi route
    cout << "Check Route" <<endl;
     for(int v = 0; v < s->vehicleList.size(); v++)
     {
         cout <<"Route " << v << endl;
         for(int pos = 0; pos < s->vehicleList[v].route.size(); pos++)
         {
             cout <<s->vehicleList[v].route[pos].id <<"( " <<s->vehicleList[v].route[pos].idCustomer <<") "
                  << " (" << s->vehicleList[v].route[pos].windowStartTime << ", " << s->vehicleList[v].route[pos].windowEndTime <<")\t"
                  <<"[ " << s->vehicleList[v].arrivalTime[pos] << " " <<s->vehicleList[v].startWorkingTime[pos]<< "] " << endl;

         }
     }
	for(int i = 0; i < s->nbOfVehicle; i++)
    {
        cout <<"route " << i << ": " ;
        for(int j =0; j < s->vehicleList[i].route.size();j++)
        {
            cout <<s->vehicleList[i].route[j].id << " ";
        }
        cout << endl;
    }
    //in ra tat ca id của location trong locationlist
    for(int i =0; i < s->locationList.size();i++)
    {
        cout <<s->locationList[i].id << " ";

    }
    cout << endl;


     //removalCost
 /*       s->CalculateRemovalCost();
        cout <<"removalCost route0  pos 1= " << s->vehicleList[0].removalCost[1]<<endl;
        s->WorstRemoval(11,6);
*/
/*
   s->ShawRemoval(13,6);
    //s->RandomRemoval(10);
    //in moi route
     cout << "Check xoa (10,6)" <<endl;

	for(int i = 0; i <s->nbOfVehicle; i++)
    {
        cout <<"route " << i << ": " ;
        for(int j =0; j < s->vehicleList[i].route.size();j++)
        {
            cout <<s->vehicleList[i].route[j].id << " ";
        }
        cout << endl;
    }
    //in ra tat ca id của location trong locationlist
    for(int i =0; i < s->locationList.size();i++)
    {
        cout <<s->locationList[i].id << " ";

    }

*/
 cout <<"\nObjective: " << s->obj <<endl << endl;
 cout <<"da phuc vu: " <<s->nbOfCusServiced <<endl;
    s->PrintInput();
    return 0;
}
