#include <iostream>
#include <string>
#include "function.h"
#include <conio.h>
using namespace std;

int main()
{
   Solution *s = new Solution();
   char *filename = "instance_0-triangle.txt";
	s->ReadProblem(filename);
    s->InitSolution();

    /*in moi route
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
    cout << endl;*/
    double costTm = 0;
    while(s->locationList.size()!=0 && costTm != 99999999.0)
    {
        s->CalculateChronoInfo();
        s->CalculateMaxDelay();
        s->CalculateInsertionCost();
        //costTm = s->RegretOne();
      // costTm = s->Regret2();
        costTm = s->Regret3();
    }

    s->Objective();
    cout <<"Objective: " << s->obj << endl;

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

 /*   s->CalculateChronoInfo();
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

    s->PrintInput();
    return 0;
}
