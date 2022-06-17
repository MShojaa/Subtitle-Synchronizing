#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
using namespace std;

// Structures
struct Sync
{
    float Time;

    int hour;
    int min;
    int sec;
    int msec;
};

// Functions
string fput_time(int sec);
string fput_milli(int sec);
void SetTime(struct Sync *Time);

int main()
{
    string Address;
    string Output_Address;
    // string Address = "./subtitle.srt";
    // string Output_Address = "./test1.srt";

    struct Sync Start;
    struct Sync Stop;

    int n, sign = 1;
    float time_inout;
    string line;
    streampos oldpos;

    cout << "Enter the .srt file address: ";
    cin >> Address;

    cout << "\nWhich one do you prefer?";
    do
    {
        cout << "\n1. Forward(+)\n2. Backward(-)\n(1/2): ";
        cin >> n;
    } while (n != 1 && n != 2);
    if (n == 1)
        sign = -1;
    do
    {
        cout << "\n1. Time in second(s)\n2. Time in seperate mode\n(1/2): ";
        cin >> n;
    } while (n != 1 && n != 2);
    if (n == 1)
    {
        cout << "\nTime (sec): ";
        cin >> time_inout;
    }
    else
    {
        int Hour, Min, Sec, Msec;
        do
        {
            cout << "Hour(s): ";
            cin >> Hour;
        } while (Hour < 0 && Hour > 9);
        do
        {
            cout << "Minute(s): ";
            cin >> Min;
        } while (Min < 0 && Min > 59);
        do
        {
            cout << "Second(s): ";
            cin >> Sec;
        } while (Sec < 0 && Sec > 59);
        do
        {
            cout << "Millisecond(s): ";
            cin >> Msec;
        } while (Msec < 0 && Msec > 999);
        time_inout = Hour * 3600 + Min * 60 + Sec + Msec / 1000.0;
    }
    time_inout *= sign;
    Output_Address = Address;
    if (sign == 1)
        Output_Address = Output_Address.erase(Output_Address.length() - 4) + " -" + to_string(time_inout) + "s.srt";
    else
        Output_Address = Output_Address.erase(Output_Address.length() - 4) + " +" + to_string(time_inout) + "s.srt";

    // Read from the text file
    ofstream Output_Sub(Output_Address);
    ifstream Sub(Address);

    n = 1;
    // For the first sub that doesn't fit the algorithm
    {
        getline(Sub, line);
        getline(Sub, line);
        sscanf(line.c_str(), "%d:%d:%d,%d --> %d:%d:%d,%d", &Start.hour, &Start.min, &Start.sec, &Start.msec, &Stop.hour, &Stop.min, &Stop.sec, &Stop.msec);
        Start.Time = Start.hour * 3600 + Start.min * 60 + Start.sec + Start.msec / 1000.0 - time_inout;
        if (Start.Time >= 0)
        {
            Output_Sub << n++ << endl;
            Stop.Time = Stop.hour * 3600 + Stop.min * 60 + Stop.sec + Stop.msec / 1000.0 - time_inout;

            SetTime(&Start);
            SetTime(&Stop);

            line = fput_time(Start.hour) + ":" + fput_time(Start.min) + ":" + fput_time(Start.sec) + "," + fput_milli(Start.msec) + " --> ";
            line = line + fput_time(Stop.hour) + ":" + fput_time(Stop.min) + ":" + fput_time(Stop.sec) + "," + fput_milli(Stop.msec) + "\n";
            Output_Sub << line;
            // Output_Sub << Start.hour << ":" << Start.min << ":" << Start.sec << "," << Start.msec << " --> " << Stop.hour << ":" << Stop.min << ":" << Stop.sec << "," << Stop.msec << endl;
            while (line.length() != 1)
            {
                oldpos = Sub.tellg(); // stores the position
                getline(Sub, line);
                Output_Sub << line;
            }
            Sub.seekg(oldpos); // get back to the position
        }
    }

    // Use a while loop together with the getline() function to read the file line by line
    while (getline(Sub, line))
    {
        if (line.length() == 1)
        {
            getline(Sub, line);
            getline(Sub, line);
            sscanf(line.c_str(), "%d:%d:%d,%d --> %d:%d:%d,%d", &Start.hour, &Start.min, &Start.sec, &Start.msec, &Stop.hour, &Stop.min, &Stop.sec, &Stop.msec);
            Start.Time = Start.hour * 3600 + Start.min * 60 + Start.sec + Start.msec / 1000.0 - time_inout;
            if (Start.Time >= 0)
            {
                Output_Sub << n++ << endl;
                Stop.Time = Stop.hour * 3600 + Stop.min * 60 + Stop.sec + Stop.msec / 1000.0 - time_inout;

                SetTime(&Start);
                SetTime(&Stop);

                line = fput_time(Start.hour) + ":" + fput_time(Start.min) + ":" + fput_time(Start.sec) + "," + fput_milli(Start.msec) + " --> ";
                line = line + fput_time(Stop.hour) + ":" + fput_time(Stop.min) + ":" + fput_time(Stop.sec) + "," + fput_milli(Stop.msec) + "\n";
                Output_Sub << line;
                while (line.length() != 1)
                {
                    oldpos = Sub.tellg(); // stores the position
                    getline(Sub, line);
                    Output_Sub << line;
                }
                if (!getline(Sub, line))
                    break;
                else
                    Sub.seekg(oldpos); // get back to the position
            }
        }
    }
    Sub.close();
    Output_Sub.close();

    return 0;
}

// Functions
string fput_time(int sec)
{
    string str;
    if (sec < 10)
        str = "0";
    str = str + to_string(sec);
    return str;
}
string fput_milli(int sec)
{
    string str;
    if (sec < 10)
        str = "00";
    else if (sec < 100)
        str = "0";
    str = str + to_string(sec);
    return str;
}
void SetTime(struct Sync *Time)
{
    Time->hour = floor(Time->Time / 3600);
    Time->Time -= Time->hour * 3600;
    Time->min = floor(Time->Time / 60);
    Time->Time -= Time->min * 60;
    Time->sec = floor(Time->Time);
    Time->Time -= Time->sec;
    Time->msec = floor(Time->Time * 1000);
}