#include"function.h"
#include<iostream>
#include<cstring>
#include<fstream>
#include<sstream>
#include<algorithm>
#include<stdlib.h>
#include<cmath>
#include<ctime>
#define MAX_NUM 99999999.0
void Solution::ReadProblem(char *filename)
{

    float vehCapacity;
     string s;
    ifstream openFile(filename);
    if (openFile.is_open())
    {
        getline(openFile,s);

        getline(openFile,s);

    //read  number of Customer, number of Locaton, number of Vehicle, time horizon
        openFile >> nbOfCustomer >> nbOfLocation >> timeHorizon >> vehCapacity;
     //   cout << nbOfCustomer <<" " << nbOfLocation << " " << timeHorizon << " " << vehCapacity<<endl;


      nbOfVehicle = nbOfCustomer;
       //   nbOfVehicle = 6;

    // delete line text and empty line
        getline(openFile,s);
       // cout <<"1 " <<s << endl;
        getline(openFile,s);
        // cout <<"2 " <<s << endl;
        getline(openFile,s);
         //cout <<"3 "<<s << endl;
        getline(openFile,s);
         //cout <<"4 " <<s << endl;
        getline(openFile,s);
          //cout <<"5 " <<s << endl;

    //init vehicleList
        Vehicle *v = new Vehicle();
        v->capacity = vehCapacity;
        for(int i =0; i <nbOfVehicle;i++)
        {
            vehicleList.push_back(*v);
        }
    //read customer's information

        for(int indexCustom = 0; indexCustom < nbOfCustomer + 2; indexCustom++)
        {

            getline(openFile,s);
        // Xoa bo cac ki tu ] [ ,   thi day s se chi con lai cac so
            for (int i = 0; i < s.length(); i++) {
                if (s[i] == ']' || s[i] == '[' || s[i] == ',')
                    s[i] = ' ';
            }




            int idCus, demCus;

            stringstream iss(s);
            int value;
            int i = 1;
            while (iss >> value)
            {

                if (i == 1)
                {
                    idCus = value;
                    i++;
                }
                else if (i == 2)
                {
                    demCus = value;
                    i++;
                }
                else if (i % 3 == 0)
                {
                  Location *l = new Location();
                    l->id = value;
                    i++;


                    iss >> value;
                    l->windowStartTime = value;
                    i++;


                    iss >> value;
                    l->windowEndTime = value;

                    l->idCustomer = idCus;
                    l->deman = demCus;

                    locationList.push_back(*l);
                    delete l;
                    i++;
                }

            }

        }
    //    cout <<locationList[1].idCustomer <<" " << locationList[1].deman <<" " << locationList[1].id << "["<< locationList[1].windowStartTime <<" "<<locationList[1].windowEndTime << endl;
        getline(openFile, s);
        getline(openFile, s);
        getline(openFile, s);
        getline(openFile, s);

        // read location coordinate
        int idTemp = 0;
        for (int j = 0; j < locationList.size(); j++)
        {

            openFile >> idTemp;
            openFile >> locationList[j].xCoor ;
            openFile >> locationList[j].yCoor;

        }


        getline(openFile, s);
        getline(openFile, s);
        getline(openFile, s);
        getline(openFile, s);
        getline(openFile, s);

        //read time travel and cost between 2 location:
        for(int i =0; i < nbOfLocation*nbOfLocation ; i++)
        {
            getline(openFile, s);

            for (int i = 0; i < s.length(); i++)
            {
                if (s[i] == ')' || s[i] == '(' || s[i] == ',')
                    s[i] = ' ';

            }
            int locationId1,locationId2;
            float timeBw2location, cost2Location;
            stringstream iss(s);
            iss >> locationId1 >> locationId2 >> timeBw2location >> cost2Location;
            timeTravel[locationId1][locationId2] = timeBw2location;
            costBtwLocation[locationId1][locationId2] = cost2Location;

        }
        openFile.close();
    }
    else cout <<"unable to open file";

}
void Solution ::CalculateChronoInfo()
{

    for(int i = 0; i <vehicleList.size(); i++)
    {
        vehicleList[i].arrivalTime[0] = 0.0;
        vehicleList[i].startWorkingTime[0] = 0.0;
        vehicleList[i].waitingTime[0] = 0.0;
        for(int j = 1; j < vehicleList[i].route.size(); j++)
        {
            vehicleList[i].arrivalTime[j] = vehicleList[i].startWorkingTime[j-1] + timeTravel[vehicleList[i].route[j-1].id][vehicleList[i].route[j].id];
            vehicleList[i].startWorkingTime[j] = max(vehicleList[i].arrivalTime[j], vehicleList[i].route[j].windowStartTime);
            vehicleList[i].waitingTime[j] = max(0.0, vehicleList[i].startWorkingTime[j]-vehicleList[i].arrivalTime[j]);
        }
    }
}

void Solution::CalculateMaxDelay()
{

    for(int i =0; i < vehicleList.size(); i++)
    {
        int last = vehicleList[i].route.size()-1;
        vehicleList[i].maxDelay[last] = vehicleList[i].route[last].windowEndTime - vehicleList[i].startWorkingTime[last];
        for(int j = vehicleList[i].route.size()-2; j>=0; j--)
        {
            vehicleList[i].maxDelay[j] = min(vehicleList[i].route[j].windowEndTime - vehicleList[i].startWorkingTime[j], vehicleList[i].waitingTime[j+1]+vehicleList[i].maxDelay[j+1]);
        }
    }

}

void Solution::CalculateInsertionCost()
{
    for(int l = 0; l <locationList.size(); l++)
    {
        if(isInserted[locationList[l].idCustomer]==true)
        {
            for(int v =0; v < vehicleList.size(); v++)
            {
                for(int lc = 1; lc < vehicleList[v].route.size(); lc++)
                {
                    locationList[l].insertionCost[v][lc]= MAX_NUM;
                }
            }
        }
        else
        {
             for(int v =0; v < vehicleList.size(); v++)
            {
                for(int lc = 1; lc < vehicleList[v].route.size(); lc++)
                {

                    double arrivalTimeAtL = vehicleList[v].startWorkingTime[lc-1] + timeTravel[vehicleList[v].route[lc-1].id][locationList[l].id];
                    double startWorkingTimeAtL =  max(arrivalTimeAtL, locationList[l].windowStartTime);
                    double waitingTimeAtL = startWorkingTimeAtL - arrivalTimeAtL;

                    double diffTime = timeTravel[vehicleList[v].route[lc-1].id][locationList[l].id] + timeTravel[locationList[l].id][vehicleList[v].route[lc].id]
                                    - timeTravel[vehicleList[v].route[lc-1].id][vehicleList[v].route[lc].id];

                   if(vehicleList[v].currentCapacity+locationList[l].deman <= vehicleList[v].capacity && diffTime + waitingTimeAtL <= vehicleList[v].waitingTime[lc] + vehicleList[v].maxDelay[lc] && arrivalTimeAtL <=locationList[l].windowEndTime)
                   {

                       locationList[l].insertionCost[v][lc] = costBtwLocation[vehicleList[v].route[lc-1].id][locationList[l].id]
                                                            + costBtwLocation[locationList[l].id][vehicleList[v].route[lc].id]
                                                            - costBtwLocation[vehicleList[v].route[lc-1].id][vehicleList[v].route[lc].id];
                   }
                   else
                   {

                       locationList[l].insertionCost[v][lc] = MAX_NUM;
                   }
                }
            }
        }

    }
}
void Solution::CalculateRemovalCost()
{
    for(int v =0; v <vehicleList.size(); v++)
    {
        for(int pos =1; pos <vehicleList[v].route.size()-1; pos++)
        {
            vehicleList[v].removalCost[pos] = costBtwLocation[vehicleList[v].route[pos-1].id][vehicleList[v].route[pos].id]
                                              + costBtwLocation[vehicleList[v].route[pos].id][vehicleList[v].route[pos+1].id]
                                              - costBtwLocation[vehicleList[v].route[pos-1].id][vehicleList[v].route[pos+1].id];
        }
    }
}
void Solution::InitSolution()
{
    nbOfCusServiced = 0;
    for(int i = 0; i < 100; i++)
    {
        isInserted[i]= false;
    }
    for(int i = 0; i < nbOfVehicle; i++)
    {
        vehicleList[i].route.push_back(locationList[0]);
        vehicleList[i].route.push_back(locationList.back());
    }
    //sau khi them thi xoa diem da them khoi locationList
    locationList.erase(locationList.begin());
    locationList.erase(locationList.begin() + locationList.size()-1);

}
double Solution::RegretOne()
{
    int minInsert_LocationId = -1;
    int minInsert_VehicleId  = -1;
    int minInsert_PositionId = -1;

    double minInsertCost = MAX_NUM;
    for(int l = 0; l < locationList.size(); l++)
    {
        for(int v = 0; v < vehicleList.size(); v++)
        {
            for(int pos = 1; pos < vehicleList[v].route.size(); pos++)
            {
                if(locationList[l].insertionCost[v][pos] < minInsertCost)
                {
                    minInsertCost = locationList[l].insertionCost[v][pos];
                    minInsert_LocationId = l;
                    minInsert_VehicleId  = v;
                    minInsert_PositionId = pos;
                }
            }
        }
    }
    // them diem co insertionCost it nhat vao
    if(minInsertCost!=MAX_NUM)
    {
        vehicleList[minInsert_VehicleId].route.insert(vehicleList[minInsert_VehicleId].route.begin()+ minInsert_PositionId, locationList[minInsert_LocationId] );
        nbOfCusServiced++;
        isInserted[locationList[minInsert_LocationId].idCustomer] = true;
        vehicleList[minInsert_VehicleId].currentCapacity += locationList[minInsert_LocationId].deman;
        locationList.erase(locationList.begin()+ minInsert_LocationId);

    }

    return minInsertCost;
}
void Solution::Objective()
{
    obj = 0;
    for(int v =0; v < vehicleList.size(); v++)
    {
        for(int pos = 1; pos < vehicleList[v].route.size(); pos++)
        {
           // cout <<"cost("<<vehicleList[v].route[pos-1].id<<", "<<vehicleList[v].route[pos].id<<")" << costBtwLocation[vehicleList[v].route[pos-1].id][vehicleList[v].route[pos].id] <<"\t";
            obj += costBtwLocation[vehicleList[v].route[pos-1].id][vehicleList[v].route[pos].id];
        }
       // cout << endl;
    }

}
double Solution::Regret2()
{
    int lcSize = locationList.size();
    int vhSize = vehicleList.size();

    double **minCost_Lc_R = new double*[lcSize];    //minCost_Lc_R[i][j]  cost it nhat cua location i tren xe j.
    double **minPost_index = new double* [lcSize];  //minPost_index[i][j] vi tri chen co cost it nhat cua location i tren xe j
    for(int i=0; i< lcSize; i++)
    {
        minCost_Lc_R[i] = new double[vhSize];
        minPost_index[i] = new double[vhSize];
    }


    for(int i = 0; i < lcSize; i++)
    {
        for(int j =0; j < vhSize; j++)
        {
            minCost_Lc_R[i][j]  = 99999999.0;   //khoi tao
            minPost_index[i][j] = -1;
        }
    }

    double* regretValue = new double[lcSize];
    for(int i =0; i <lcSize; i++)
    {
        regretValue[i]= 0-MAX_NUM; // khoi tao regret la mot so rat be
    }
    double min1 = 99999999.0, min2 = 999999999.0;

    //tim InsertionCost nho nhat cua tung location  tai moi route
    for(int l = 0; l < locationList.size(); l++)
    {
        for(int v = 0; v <vehicleList.size(); v++)
        {
            for(int pos = 1; pos < vehicleList[v].route.size(); pos++)
            {
                 if(minCost_Lc_R[l][v] > locationList[l].insertionCost[v][pos])
                {
                    minCost_Lc_R[l][v] = locationList[l].insertionCost[v][pos];
                    minPost_index[l][v] = pos;
                }

            }
        }

    }

    //Tim regretValue cua moi Location
   for(int l = 0; l < locationList.size(); l++)
    {
        min1 = 999999999.0;
        min2 = 999999999.0;
        for(int v = 0; v <vehicleList.size(); v++)
        {
            if(min1 > minCost_Lc_R[l][v])
            {
                min2 = min1;
                min1 = minCost_Lc_R[l][v];
            }
            else if( min2 > minCost_Lc_R[l][v])
            {
                min2 = minCost_Lc_R[l][v];
            }
        }
      //  regretValue[l] = min2 - min1;
       if(min1 != MAX_NUM)
        {
            regretValue[l] = min2 - min1;
        }

    }

    //tim chi so  cua location co regretValue lon nhat
    double maxRegretValue = 0-MAX_NUM -1;  //khoi tao maxRegretValue nho hon gia trị -MAX_NUM de de
                                        //trong truong hop locationList con toan cac diem ko chen dc (insertion cost = MAX_NUM)  thi van chon ra dc 1 location thu l. sau day vi insertionCost = Max_NuM thi ket thuc viec chen.
    int chosenLocation = -1;
    for(int l = 0; l < locationList.size(); l++)
    {

       if(maxRegretValue < regretValue[l])
       {
           maxRegretValue = regretValue[l];
           chosenLocation = l;
       }

    }

    //tim route (co insertionCost it nhat) cua location co regretValue lon nhat
    int chosenRoute = -1;
    double minCost = 999999999.0;
    for(int v = 0; v < vehicleList.size(); v++)
    {
        if(minCost_Lc_R[chosenLocation][v] < minCost )
        {
            minCost = minCost_Lc_R[chosenLocation][v];
            chosenRoute = v;
        }
    }


   // int chosenPosition = minPost_index[chosenLocation][chosenRoute];
    int minInsert_LocationId = chosenLocation;
    int minInsert_VehicleId  = chosenRoute;
    int minInsert_PositionId = minPost_index[chosenLocation][chosenRoute];
    double minInsertCost = minCost_Lc_R[chosenLocation][chosenRoute];


    // them diem co insertionCost it nhat vao
    if(minInsertCost!=MAX_NUM)
    {
        vehicleList[minInsert_VehicleId].route.insert(vehicleList[minInsert_VehicleId].route.begin()+ minInsert_PositionId, locationList[minInsert_LocationId] );
         nbOfCusServiced++;
        isInserted[locationList[minInsert_LocationId].idCustomer] = true;
        vehicleList[minInsert_VehicleId].currentCapacity += locationList[minInsert_LocationId].deman;
        locationList.erase(locationList.begin()+ minInsert_LocationId);

    }

    for(int i=0; i< lcSize; i++)
    {
        delete(minCost_Lc_R[i]);
        delete(minPost_index[i]);
    }
    delete(minCost_Lc_R);
    delete(minPost_index);
    delete (regretValue);

    return minInsertCost;
}


double Solution::Regret3()
{
    int lcSize = locationList.size();
    int vhSize = vehicleList.size();

    double **minCost_Lc_R = new double*[lcSize];    //minCost_Lc_R[i][j]  cost it nhat cua location i tren xe j.
    double **minPost_index = new double* [lcSize];  //minPost_index[i][j] vi tri chen co cost it nhat cua location i tren xe j
    for(int i=0; i< lcSize; i++)
    {
        minCost_Lc_R[i] = new double[vhSize];
        minPost_index[i] = new double[vhSize];
    }


    for(int i = 0; i < lcSize; i++)
    {
        for(int j =0; j < vhSize; j++)
        {
            minCost_Lc_R[i][j]  = 99999999.0;   //khoi tao
            minPost_index[i][j] = -1;
        }
    }

    double* regretValue = new double[lcSize];
    for(int i =0; i <lcSize; i++)
    {
        regretValue[i]= 0-MAX_NUM; // khoi tao regret la mot so rat be
    }


    //tim InsertionCost nho nhat cua tung location  tai moi route
    for(int l = 0; l < locationList.size(); l++)
    {
        for(int v = 0; v <vehicleList.size(); v++)
        {
            for(int pos = 1; pos < vehicleList[v].route.size(); pos++)
            {
                 if(minCost_Lc_R[l][v] > locationList[l].insertionCost[v][pos])
                {
                    minCost_Lc_R[l][v] = locationList[l].insertionCost[v][pos];
                    minPost_index[l][v] = pos;
                }

            }
        }

    }

    //Tim regretValue cua moi Location
     double min1 = 99999999.0, min2 = 999999999.0, min3 = 99999999.0;
   for(int l = 0; l < locationList.size(); l++)
    {
        min1 = 99999999.0;
        min2 = 99999999.0;
        min3 = 99999999.0;
        for(int v = 0; v <vehicleList.size(); v++)
        {
            if(min1 > minCost_Lc_R[l][v])
            {
                min3 = min2;
                min2 = min1;
                min1 = minCost_Lc_R[l][v];
            }
            else if( min2 > minCost_Lc_R[l][v])
            {
                min3 = min2;
                min2 = minCost_Lc_R[l][v];
            }
            else if(min3 > minCost_Lc_R[l][v])
            {
                min3 = minCost_Lc_R[l][v];
            }
        }
        if(min1 != MAX_NUM)
        {
            regretValue[l] = min3 - min1 + min2 - min1;
        }

    }

    //tim chi so  cua location co regretValue lon nhat
    double maxRegretValue = 0-MAX_NUM -1;  //khoi tao maxRegretValue nho hon gia trị -MAX_NUM de de
                                        //trong truong hop locationList con toan cac diem ko chen dc (insertion cost = MAX_NUM)  thi van chon ra dc 1 location thu l. sau day vi insertionCost = Max_NuM thi ket thuc viec chen.
    int chosenLocation = -1;
    for(int l = 0; l < locationList.size(); l++)
    {

       if(maxRegretValue < regretValue[l])
       {
           maxRegretValue = regretValue[l];
           chosenLocation = l;
       }

    }

    //tim route (co insertionCost it nhat) cua location co regretValue lon nhat
    int chosenRoute = -1;
    double minCost = 999999999.0;
    for(int v = 0; v < vehicleList.size(); v++)
    {
        if(minCost_Lc_R[chosenLocation][v] < minCost )
        {
            minCost = minCost_Lc_R[chosenLocation][v];
            chosenRoute = v;
        }
    }


   // int chosenPosition = minPost_index[chosenLocation][chosenRoute];
    int minInsert_LocationId = chosenLocation;
    int minInsert_VehicleId  = chosenRoute;
    int minInsert_PositionId = minPost_index[chosenLocation][chosenRoute];
    double minInsertCost = minCost_Lc_R[chosenLocation][chosenRoute];


    // them diem co insertionCost it nhat vao
    if(minInsertCost!=MAX_NUM)
    {
        vehicleList[minInsert_VehicleId].route.insert(vehicleList[minInsert_VehicleId].route.begin()+ minInsert_PositionId, locationList[minInsert_LocationId] );
         nbOfCusServiced++;
        isInserted[locationList[minInsert_LocationId].idCustomer] = true;
        vehicleList[minInsert_VehicleId].currentCapacity += locationList[minInsert_LocationId].deman;
        locationList.erase(locationList.begin()+ minInsert_LocationId);

    }

    for(int i=0; i< lcSize; i++)
    {
        delete(minCost_Lc_R[i]);
        delete(minPost_index[i]);
    }
    delete(minCost_Lc_R);
    delete(minPost_index);
    delete (regretValue);

    return minInsertCost;
}

void Solution::RandomRemoval(int nbOfRemove)
{
   // srand (time(NULL));
    int removed = 0;
    while(removed!=nbOfRemove)
    {
        int rdIndexRoute = rand()%(nbOfVehicle-1)+0;
        if(vehicleList[rdIndexRoute].route.size()==2)
        {
            continue;
        }
        else
        {
             int rdPosInRoute = rand()%(vehicleList[rdIndexRoute].route.size()-2) + 1;
             locationList.push_back(vehicleList[rdIndexRoute].route[rdPosInRoute]);
             isInserted[vehicleList[rdIndexRoute].route[rdPosInRoute].idCustomer] = false;
             vehicleList[rdIndexRoute].currentCapacity -= vehicleList[rdIndexRoute].route[rdPosInRoute].deman;
             vehicleList[rdIndexRoute].route.erase(vehicleList[rdIndexRoute].route.begin() + rdPosInRoute);
              nbOfCusServiced--;
             removed++;
        }

    }
}

void Merge(SortingHelper A[], int c, int b){
    //b: so phan tu cua mang A, c la vi tri phan tu o giua
    // mang a[0...c] va a[c+1...b-1] la cac mang da sap xep

    int i = 0, k =0, j = c+1;

    SortingHelper *B = new SortingHelper[b];
    while((i < c+1)&& (j <b)){
        if(A[i].value < A[j].value)
            B[k++] = A[i++];
        else
            B[k++] = A[j++];
    }
    while(i<c+1){
        B[k++] = A[i++];
    }
    while(j < b){
        B[k++] = A[j++];
    }
    i = 0;
    for(k = 0; k < b; k++){
        A[i++] = B[k];
    }
	delete []B;
}
void mergeSort(SortingHelper p[], int size){
    if(size>1){
       int t = size/2;
        mergeSort(p,t );
        mergeSort(p,size - t);
        Merge(p,t-1,size);

    }
}

void Solution::WorstRemoval(int q,double p)
{
    while(q>0)
    {

        SortingHelper *sh = new SortingHelper[100];
        int count = 0;
        for(int v = 0; v < vehicleList.size(); v++)
        {
            for(int pos = 1; pos <vehicleList[v].route.size()-1; pos++)
            {
                sh[count].value = vehicleList[v].removalCost[pos];
                sh[count].firstIndex = v;
                sh[count].secondIndex = pos;
                count++;
            }
        }
        mergeSort(sh,count);
       /* for(int i =0; i <count; i++)
        {
            sh[i].showInfo();
        }*/
        double y = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        while(y==1)
        {
            y = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        }
        int chosenIndex =count - 1 - floor(pow(y,p)*count);


        int rdIndexRoute = sh[chosenIndex].firstIndex;
        int rdPosInRoute = sh[chosenIndex].secondIndex;
        locationList.push_back(vehicleList[rdIndexRoute].route[rdPosInRoute]);
        isInserted[vehicleList[rdIndexRoute].route[rdPosInRoute].idCustomer] = false;
        vehicleList[rdIndexRoute].currentCapacity -= vehicleList[rdIndexRoute].route[rdPosInRoute].deman;
        vehicleList[rdIndexRoute].route.erase(vehicleList[rdIndexRoute].route.begin() + rdPosInRoute);
        nbOfCusServiced--;


        q = q-1;

    }
}
void Solution::PrintInput()
{
  /*  cout << nbOfCustomer <<" " << nbOfLocation << " "<< nbOfVehicle << endl;
    cout<<locationList[0].xCoor <<" " << locationList[0].yCoor << endl;
    cout <<timeTravel[0][1] <<" " <<costBtwLocation[0][1] << endl;
  */   ofstream coutFile("instance_14-triangle.out");
     coutFile << obj << endl;
     for(int v = 0; v < vehicleList.size(); v++)
     {
         if(vehicleList[v].route.size()!=2)
         {
            for(int pos = 0; pos < vehicleList[v].route.size(); pos++)
             {
                 coutFile <<vehicleList[v].route[pos].idCustomer <<" " <<vehicleList[v].route[pos].id << endl;

             }
         }

     }

}

