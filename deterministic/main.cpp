#include <iostream>
#include <fstream>
using namespace std;

int main(int argc, char* argv[])
{
    int tmp,i,map_x,map_y,no_st,no_dy,st_x,st_y,g_x,g_y;
    ifstream indata;
    ofstream outdata;


    if (argc<2)
    {
        cout<<"error input file is missing \n";
        return 1;
    }



    indata.open(argv[1]);
    if (!indata)
    {
        cout << "There was a problem opening file "<< endl;
        return 1;
    }

    indata>>map_x;
    indata>>map_y;
    indata>>no_st;
    indata>>no_dy;
    int st_arr [no_st][2];
    int dy_arr [no_dy][2];
    for(i=0;i<no_st;i++)
    {
        indata>>tmp;
        indata>>st_arr[i][0];
        indata>>st_arr[i][1];
    }
    for(i=0;i<no_dy;i++)
    {
        indata>>tmp;
        indata>>dy_arr[i][0];
        indata>>dy_arr[i][1];
    }
    indata>>st_x;
    indata>>st_y;
    indata>>g_x;
    indata>>g_y;
    indata.close();


    printf("Size of map is %d * %d \n",map_x,map_y);
    printf("No of static = %d and No of dynamic = %d \n",no_st,no_dy);
    printf("Start position is %d * %d \n",st_x,st_y);
    printf("Goal position is %d * %d \n",g_x,g_y);

    printf("static array \n");
    for(i=0;i<no_st;i++)
    {
        cout <<i<<"\t"<<st_arr[i][0]<<"\t"<<st_arr[i][1]<<endl;
    }

    printf("dynamic array \n");
    for(i=0;i<no_dy;i++)
    {
        cout <<i<<"\t"<<dy_arr[i][0]<<"\t"<<dy_arr[i][1]<<endl;
    }




    outdata.open("outFile.txt");
    outdata<<"u,d,r,l";


    return 0;
}
