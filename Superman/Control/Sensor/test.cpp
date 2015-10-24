#include <stdio.h>
#include <iostream>
#include "BMP180.h"

// You will need to create an SFE_BMP180 object, here called "pressure":

using namespace std;
BMP180 pressure;

#define ALTITUDE 1655.0 // Altitude of SparkFun's HQ in Boulder, CO. in meters

int main(int argc,char* argv[])
{
	if (pressure.init())
		printf("BMP180 init success\n");
	else
	{
		// Oops, something went wrong, this is usually a connection problem,
		// see the comments at the top of this sketch for the proper connections.

		printf("BMP180 init fail\n\n");
		return 0; // Pause forever.
	}


	while(1)
	{
		char status;
		double T,P,p0,a;

		// Loop here getting pressure readings every 10 seconds.

		// If you want sea-level-compensated pressure, as used in weather reports,
		// you will need to know the altitude at which your measurements are taken.
		// We're using a constant called ALTITUDE in this sketch:

		printf("provided altitude: ");
		cout<<ALTITUDE<<endl;
		printf(" meters,\n ");
		cout<<ALTITUDE*3.28084<<endl;

		status = pressure.startTemperature();
		if (status != 0)
		{
			// Wait for the measurement to complete:
			delay(status);

			// Retrieve the completed temperature measurement:
			// Note that the measurement is stored in the variable T.
			// Function returns 1 if successful, 0 if failure.

			status = pressure.getTemperature(T);
			if (status != 0)
			{
				// Print out the measurement:
				printf("temperature: ");
				printf("%.2f",T);
				printf(" deg C, ");
				printf("%.2f",(9.0/5.0)*T+32.0);
				printf(" deg F");

				status = pressure.startPressure(3);
				if (status != 0)
				{
					delay(status);


					status = pressure.getPressure(P,T);
					if (status != 0)
					{
						// Print out the measurement:
						printf("absolute pressure: ");
						printf("%.2f",P);
						printf(" mb, ");
						printf("%.2f",P*0.0295333727);
						printf(" inHg");

					}
					else printf("error retrieving pressure measurement\n");
				}
				else printf("error starting pressure measurement\n");
			}
			else printf("error retrieving temperature measurement\n");
		}
		else printf("error starting temperature measurement\n");

		delay(5000);  // Pause for 5 seconds.

	}

}
