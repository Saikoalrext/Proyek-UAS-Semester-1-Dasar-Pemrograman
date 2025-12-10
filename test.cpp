#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <thread>
#include <algorithm>
#include <cmath>
#include <ctime>
#include <fstream>
#include <limits>
#include <cstdlib>
#include <cstring>
#include <atomic>
#include <future>
#include <conio.h>
#include <iomanip>
#include <cstdint>

using namespace std;

void wait(int s= 1){
    const int CHECKS_PER_SECOND= 10;
    const int SLEEP_MS= 1000/ CHECKS_PER_SECOND;

    for (int i = 0; i < s* CHECKS_PER_SECOND; i++)
    {
        if (_kbhit())
        {
            char key= _getch();
            if (key== '\r'|| key== '\n')
            {
                while (_kbhit()) _getch();
                return;
            }
        }
        this_thread::sleep_for(chrono::milliseconds(SLEEP_MS));   
    }
}

void waitms(int ms= 10){
    this_thread::sleep_for(chrono::milliseconds(ms));
}

void storyIntro(){
    vector<string> story= {"You", " ", "wake", " ", "up", " ", "stranded", ",", " ", "your", " ", "past", " ", "a", " ", "blank", " ", "slate", "."};
    for (size_t i = 0; i < story.size(); i++)
    {
        cout<< story[i];
        waitms();
    }
    wait();
}

void printTypewriter(const string& text, int ms_per_char= 30){
    for (char c : text)
    {
        cout<< c<< flush;
        waitms(ms_per_char);
    }
    
}

void storyIntr(){
    const string story= "You wake up stranded, your past a blank slate.";

    printTypewriter(story);
    wait();
}

int main(){
    // int x;
    // int y;
    // int z;
    // int a;

    // x= 10;
    // y= x;
    // z= y;
    // a= 10;

    // int j= 20;

    // vector<int> xx = {1, -1, 20, -10};
    // vector<int> xx1= {5, -7, 25, -20};
    // vector<int> yy= {0, 0, -5, 10};
    // vector<int> yy1= {10};

    // z= 1;
    

    // int l;
    // int f= min(xx[0], xx1[0]);
    // int g= max(xx[0], xx1[0]);
    // int h= min(yy[0], yy1[0]);
    // int i= max(yy[0], yy1[0]);

    // int konz;
    // int konts;
    // int l;

    // vector<string> a= {"b", "b", "c", "d"};
    // vector<int> b= {1, 2, 3, 4};

    // cout<< "1. A\n2. B\n3. C\n";
    // cout<< "Input asal: ";
    // cin>> konz;
    // cout<< "Input tujuan: ";
    // cin>> konts;
    
    // if (konz> konts)
    // {
    //     for (int i = konts; i < konz; i++)
    //     {
    //         l = b[konts]+= b[konz];
    //     }
    //     cout<< l;
    // }
    // else if(konz< konts){
    //     for (int i = konz; i < konts; i++)
    //     {
    //         l = b[konz]+= b[konts];
    //     }
    //     cout<< l;
    // }
    
    // int x;
    // int y;
    // bool xy= true;


    // cout<< "1. A\n2. B\n3. C\nChoice: ";

    // cin>> x;
    // switch (x)
    // {
    // case 1:
    //     while (xy)
    //     {
    //         cout<< "1. A\n2. B\n3. C\n0. Back\nChoice: ";

    //         cin>> y;
    //         switch (y)
    //         {
    //         case 1:
    //             cout<< "1. A\n2. B\n3. C\n0. Back\nChoice: ";
    //             break;
    //         case 2: 
    //             cout<< "1. A\n2. B\n3. C\n0. Back\nChoice: ";
    //             break;
    //         case 3:
    //             cout<< "1. A\n2. B\n3. C\n0. Back\nChoice: ";
    //             break;
    //         default:
    //         xy= false;
    //             break;
    //         }
    //     } 
    //     break;
    
    // case 2:
    //     while (xy)
    //     {
    //         cout<< "1. A\n2. B\n3. C\n0. Back\nChoice: ";

    //         cin>> y;
    //         switch (y)
    //         {
    //         case 1:
    //             cout<< "1. A\n2. B\n3. C\n0. Back\nChoice: ";
    //             break;
    //         case 2: 
    //             cout<< "1. A\n2. B\n3. C\n0. Back\nChoice: ";
    //             break;
    //         case 3:
    //             cout<< "1. A\n2. B\n3. C\n0. Back\nChoice: ";
    //             break;
    //         default:
    //         xy= false;
    //             break;
    //         }
    //     } 
    //     break;
    // }

    // int x;

    

    // while (x!= 21)
    // {
    //     x= rand()% 81+ 20;
    //     cout<< x<< " ";
    // }
    
    storyIntr();
}
