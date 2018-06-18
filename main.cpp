#include <iostream>
#include <string>
#include "function.h"
#include <conio.h>
#include<stdlib.h>
#include<ctime>
using namespace std;

int main()
{
   // srand (time(NULL));
    Solution *s = new Solution();
    char *filename = "instance_0-triangle.txt";
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
        //costTm = s->RegretOne();
      //costTm = s->Regret2();
        costTm = s->Regret3();
    }
        s->Objective();
        cout <<"khoi tao regret3 obj = "<<s->obj<<endl << endl;

    //LNS
    int nbLoops = 0;
    while(nbLoops < 5000)
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
        for(int i = 0; i <500; i++)
        {
            for(int j =0; j <500; j++)
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
        rdUpdate = rand()%1;
        q = rand()%((s->nbOfCusServiced)/2) + 4;

    //cout <<"a b c" << rdRepair <<" "<< rdUpdate<<" " <<q << endl;

        //destroysolutiion
        if(rdUpdate==0)
        {
            potential->RandomRemoval(q);
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
            cout<<"obj cua s = " << s->obj << "\t" << "obj potentail = "<<potential->obj<<endl;
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
            cout <<"moi " <<s->obj<<endl;
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
        else kocaithien++;
        nbLoops++;
        delete(potential);
    }

cout <<"cai thien = " <<solancaithien <<"\t ko cai thien = " << kocaithien<<endl;



    //s->Objective();
    cout <<"Objective: " << s->obj <<endl << endl;

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
    cout << "before remove number of customer serviced= " << s->nbOfCusServiced <<endl;


 /*
    //xoa bot diem trong route
    s->RandomRemoval(5);
    cout << "remove" << "number of customer serviced= " << s->nbOfCusServiced <<endl;
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
*/

 /* s->CalculateChronoInfo();
    s->CalculateMaxDelay();
    s->CalculateInsertionCost();
    cout <<"insertionCost[1][1] cua location co id = 2:  " << s->locationList[1].insertionCost[1][1] << endl;
    //them node 2 vao route dau tien tai vi tri thu 1
    s->vehicleList[0].route.insert(s->vehicleList[0].route.begin()+1, s->locationList[1] );
    s->locationList.erase(s->locationList.begin()+1);
    s->CalculateChronoInfo();
    s->CalculateMaxDelay();
    s->CalculateInsertionCost();


   cout << s->vehicleList[0].maxDelay[1]<<endl;

   cout << s->vehicleList[0].route[1].id <<" " <<s->vehicleList[0].arrivalTime[1] <<" " << s->vehicleList[0].startWorkingTime[1] << " "<<s->vehicleList[0].waitingTime[1] << endl;

    // tinh insertionCost cua cac lcation con lai trong locationList
*/

   // s->PrintInput();
    return 0;
}
