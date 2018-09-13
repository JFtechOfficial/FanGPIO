#include <unistd.h>
#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>


int main (int sleepTime, int maxTemp, int cutoffTemp)
{
  wiringPiSetup ();
  pinMode (1, OUTPUT);
  softPwmCreate (1, 0, 100);
  double maxTemperature = maxTemp * 1000;
  double cutoffTemperature = cutoffTemp * 1000;
  double step = (70 - maxTemp) * 1000 / 3;
  for (;;)
  {
	usleep(sleepTime * 1000);
	FILE *temperatureFile;
		double T;
		temperatureFile = fopen ("/sys/class/thermal/thermal_zone0/temp", "r");
		fscanf (temperatureFile, "%lf", &T);
		if (T < cutoffTemperature){
			softPwmWrite (1, 0);
		}
		if (T >= maxTemperature && T < maxTemperature + step){
			softPwmWrite (1, 25);
		}
		if (T >= maxTemperature + step && T < maxTemperature + (step * 2)){
			softPwmWrite (1, 50);
		}
		if (T >= maxTemperature + (step * 2) && T < 70000){
			softPwmWrite (1, 75);
		}
		if (T >= 70000){
			softPwmWrite (1, 100);
		}
		fclose (temperatureFile);
  }
  return 0;
}
