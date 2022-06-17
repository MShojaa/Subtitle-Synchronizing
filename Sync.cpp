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
    struct Sync Start;
    struct Sync Stop;

    int n = 1;
    float time_inout = 0;

    string Address = "./subtitle.srt";
    string Output_Address = "./test1.srt";
    string line;
    string eol = "\n";
    streampos oldpos;

    // Read from the text file
    ofstream Output_Sub(Output_Address);
    ifstream Sub(Address);
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