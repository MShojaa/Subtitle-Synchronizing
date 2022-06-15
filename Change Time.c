#include <stdio.h>
#include <string.h>
#include <math.h>
#include <string.h>

// Functions
int Num(char Address[], unsigned MAX_LENGTH);
void fput_time(int sec, FILE *Sub);
void fput_millitime(int sec, FILE *Sub);

int main()
{
    const unsigned MAX_LENGTH = 256;
    char Address[] = "./test.srt";

    char line[MAX_LENGTH], sub_ith[10];
    int start_hour, start_minute, start_second, start_millisecond;
    int end_hour, end_minute, end_second, end_millisecond;
    float start_sec_in, start_sec_out, end_sec_in, end_sec_out, sec_inout;
    int h, min, sec, msec;
    int dummy;
    int n_old = 1, n_new = 1;
    h = 2;
    min = 8;
    sec = 15;
    msec = 470;
    sec_inout = h * 3600 + min * 60 + sec + msec / 1000.0;

    int i = 0, n = Num(Address, MAX_LENGTH);

    //
    FILE *Sub = fopen(Address, "r");         // Open & check if the file exists!
    FILE *Sub_out = fopen("test1.srt", "w"); // Open & check if the file exists!
    if (Sub == NULL || Sub_out == NULL)
    {
        printf("\nThe .srt file not found\n\n");
        return -1;
    }
    while (i < n)
    {
        sprintf(line, "%s", fgets(line, MAX_LENGTH, Sub));
        sprintf(sub_ith, "%d\n", n_old);
        sscanf(line, "%d\n", &dummy);
        i++;
        if (!strcmp(line, sub_ith))
        {
            i++;
            n_old++;
            sprintf(sub_ith, "%d\n", n_old);
            sprintf(line, "%s", fgets(line, MAX_LENGTH, Sub));
            sscanf(line, "%d:%d:%d,%d --> %d:%d:%d,%d",
                   &start_hour, &start_minute, &start_second, &start_millisecond,
                   &end_hour, &end_minute, &end_second, &end_millisecond);

            start_sec_in = start_hour * 3600 + start_minute * 60 + start_second + start_millisecond / 1000.0;
            end_sec_in = end_hour * 3600 + end_minute * 60 + end_second + end_millisecond / 1000.0;

            start_sec_out = start_sec_in - sec_inout;
            end_sec_out = end_sec_in - sec_inout;

            if (start_sec_out > 0)
            {
                start_hour = (start_sec_out / 3600);
                start_sec_out -= start_hour * 3600;
                start_minute = (start_sec_out / 60);
                start_sec_out -= start_minute * 60;
                start_second = (start_sec_out);
                start_sec_out -= start_second;
                start_millisecond = (start_sec_out * 1000);

                end_hour = (end_sec_out / 3600);
                end_sec_out -= end_hour * 3600;
                end_minute = (end_sec_out / 60);
                end_sec_out -= end_minute * 60;
                end_second = (end_sec_out);
                end_sec_out -= end_second;
                end_millisecond = (end_sec_out * 1000);

                fprintf(Sub_out, "%d\n", n_new++);
                fput_time(start_hour, Sub_out);
                fputs(":", Sub_out);
                fput_time(start_minute, Sub_out);
                fputs(":", Sub_out);
                fput_time(start_second, Sub_out);
                fputs(",", Sub_out);
                fput_millitime(start_millisecond, Sub_out);
                fputs(" --> ", Sub_out);
                fput_time(end_hour, Sub_out);
                fputs(":", Sub_out);
                fput_time(end_minute, Sub_out);
                fputs(":", Sub_out);
                fput_time(end_second, Sub_out);
                fputs(",", Sub_out);
                fput_millitime(end_millisecond, Sub_out);
                fputs("\n", Sub_out);

                do
                {
                    fgets(line, MAX_LENGTH, Sub);
                    fputs(line, Sub_out);
                    i++;
                } while (strcmp(line, "\n") && i < n);
            }
            else
                do
                {
                    fgets(line, MAX_LENGTH, Sub);
                    i++;
                } while (strcmp(line, "\n"));
        }
    }
    fclose(Sub);
    fclose(Sub_out);

    return 0;
}

// Functions
int Num(char Address[], unsigned MAX_LENGTH)
{
    int n = 0;

    FILE *Sub = fopen(Address, "r"); // Open & check if the file exists!
    if (Sub == NULL)
    {
        printf("\nThe .srt file not found\n\n");
        return -1;
    }
    char line[MAX_LENGTH];
    while (fgets(line, MAX_LENGTH, Sub))
        n++;
    fclose(Sub);

    return n;
}
void fput_time(int sec, FILE *Sub)
{
    if (sec < 10)
        fputs("0", Sub);
    fprintf(Sub, "%d", sec);
}
void fput_millitime(int sec, FILE *Sub)
{
    if (sec < 10)
        fputs("00", Sub);
    else if(sec < 100)
        fputs("0", Sub);
    fprintf(Sub, "%d", sec);
}