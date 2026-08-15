#include "stdio.h"
#include "stdint.h"
#include "math.h"

#define PI                          3.14159265358979323846f
#define Om                          0.0639f

#define MAX_LENGTH                  7

float arr_thrust [MAX_LENGTH] = {0.534f, 0.842f, 1.182f, 1.554f, 1.966f, 2.399f, 2.771f};
float arr_speed [MAX_LENGTH] = {7140.0f, 8743.0f, 10250.0f, 11624.0f, 12976.0f, 14250.0f, 15275.0f};
float arr_power [MAX_LENGTH] = {94.0f, 166.0f, 262.0f, 386.0f, 541.0f, 726.0f, 912.0f};
float arr_curent [MAX_LENGTH] = {3.9f, 6.9f, 10.9f, 16.0f, 22.5f, 30.2f, 37.9f};
float Kf[MAX_LENGTH] = {};
float Km[MAX_LENGTH] = {};


#define RPM_TO_RAD(val)             ((2*PI*(val))/60)
#define KG_TO_NEWTON(val)           ((val)*9.80665f)


int main(void)
{
    float aver = 0, aver_Kf = 0, aver_Km = 0;

    for (int i = 0;i < MAX_LENGTH;i++)
    {
        Kf[i] = (float)(KG_TO_NEWTON(arr_thrust[i])/(pow(RPM_TO_RAD(arr_speed[i]), 2)))*1e5;
        Km[i] = (float)((arr_power[i] - Om*pow(arr_curent[i], 2))/(pow(RPM_TO_RAD(arr_speed[i]), 3)))*1e5;
        aver += Km[i]/Kf[i];
        aver_Kf += Kf[i];
        aver_Km += Km[i];
        printf("Kf_%d = %.5f    Km_%d = %.5f    proportion_%d = %.5f \n",i, Kf[i], i, Km[i], i, Km[i]/Kf[i]);
    }
    printf("aver is %.5f \n", aver/MAX_LENGTH);
    printf("aver Kf is %.5f \n", aver_Kf/MAX_LENGTH);
    printf("aver Km is %.5f \n", aver_Km/MAX_LENGTH);

    return 0;
}