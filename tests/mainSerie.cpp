#include "TestSerialCommunication.h"

#include <iostream>

TestSerialCommunication serial("/dev/ttyUSB0");
clock_t tMessage;

using namespace std;

int main()
{

	while(1)
	{
		int j=0;
		cout<<"Please enter an integer value: ";
		cin>>j;
		tMessage = clock();
		
		switch(j)
	    {


		case 2:
		{   // send "focus minus" message
		    cout << "focal --" << endl;
            cout << serial.messageFocusMinus() << endl;				    
		    break;
		}

		case 3:
		{   // send "focus plus" message
		    cout << "focal ++" << endl;
            cout << serial.messageFocusPlus() << endl;		
		    break;
		}

		case 4:
		{   // send "power minus" message
		    cout << "puiss --" << endl;
            cout << serial.messagePowerMinus() << endl;		
		    break;
		}

		case 5:
		{   // send "power minus" message
		    cout << "puiss ++" << endl;
            cout << serial.messagePowerPlus() << endl;		
		    break;
		}
		case 6:
		{   // send "power minus" message
		    cout << "color --" << endl;
            cout << serial.messageColorMinus() << endl;		
		    break;
		}

		case 7:
		{   // send "power minus" message
		    cout << "color ++" << endl;
            cout << serial.messageColorPlus() << endl;		
		    break;
		}
		case 8:
		{   // Ask current status: on or off,focus,color,power
            int status,focus,color,power;
            if (serial.askInformation(status,focus,color,power))              
                printf("s=%d,f=%d,c=%d,p=%d\n",status,focus,color,power);   
            else
                cout << "Demande ratee" << endl;
            
		    break;
		}
		default:
		    cout << "n'existe pas" << endl;
		    break;
	    }

		printf("temps écoulé: %f\n",(float)(clock()-tMessage)/CLOCKS_PER_SEC);
	    
	    cout << endl;
	    
	}
	
	//*/

}
