#include "TestSerialCommunication.h"

using namespace std;

TestSerialCommunication::TestSerialCommunication(std::string COMName) :
    SerialCommunication(COMName)
{
    initCommunication();
}


bool TestSerialCommunication::messageFocusPlus()
{ 
    cout << "F+" << endl;
    string message = "\x46\x2B\x0A";
	
	return sendMessageWithVerification(message);
}
bool TestSerialCommunication::messageFocusMinus()
{   
    cout << "F-" << endl;
    string message = "\x46\x2D\x0A";
	
	return sendMessageWithVerification(message);
}
bool TestSerialCommunication::messageColorPlus()
{
    string message = "\x43\x2B\x0A";
	
	return sendMessageWithVerification(message);
}
bool TestSerialCommunication::messageColorMinus()
{
    string message = "\x43\x2D\x0A";
	
	return sendMessageWithVerification(message);
}
bool TestSerialCommunication::messagePowerPlus()
{
	string message = "\x50\x2B\x0A";

	return sendMessageWithVerification(message);
}
bool TestSerialCommunication::messagePowerMinus()
{
	string message = "\x50\x2D\x0A";
	
	return sendMessageWithVerification(message);
}

/// Ask information about the current state of the Light.
// \TODO S'il on en a besoin, tester avec une communication serie fonctionelle !
bool TestSerialCommunication::askInformation(int &status,int &focus,int &color,int &power, int timeout, int delay)
{
    int n = 0;
    int cpt =0;
    char buff[50];
    
    string message;
    string answer;   
    
    int size_msg_info = 6; // 7 normalement
      
    while( ((cpt < timeout)) && ( (n < size_msg_info) /*|| (answer.c_str()[0] != 'A')*/ )  )   
	{	
	    message = "\x56\x0A";
		//--send message "V" to Test in order to ask current status
		n = sendMessageWithVerification(message, delay);
		tcflush (serialHandle, TCIOFLUSH);  
		cpt ++;	 
	}
	
	if(n > 0)
	{
	    cout << "TestSerialCommunication::askInformation : message recu ";
	    cout << message << endl;
	
	}
		
	if((n >= size_msg_info) && (message.c_str()[0] == 'A' ))
	{
		//cout << "infos" << endl;
		//--get information
		status = message.c_str()[2] & 0x01; //--1 == ON, 0 == OFF
		focus = message.c_str()[3] - '0';  //--focus: 1,3,5,7
		color = message.c_str()[4] - '0';  //--color: 1,2,3,4
		power = message.c_str()[5] - '0';  //--power: 1,2,3,4,5,6,7		
		       
		return true;                      
	} 
	 
	
	return false;	
}      

/// Send a message and check for acknowledgment
int TestSerialCommunication::sendMessageWithVerification(string& message, int delay)
{
    int n = 0;
    char buff[50]; 
    
    int size_msg_ack = 1; // 2 normalement
    
    writeWithDelay(message, 15);
    
    string answer;
    
    while( ((clock()-tLastMessage) <= ((delay*CLOCKS_PER_SEC)/1000)) && ( (n < size_msg_ack) /*|| (answer.c_str()[0] != 'A')*/ )  )   
	{	
	    n = readMessage(answer);	    
	}
	
	
	if (n < 0)
	{
	    cerr << "TestSerialCommunication::sendMessageWithVerification : \
	             Pas de message recus" << endl;
		n = -1;//return n;	
	}
	else if (answer.c_str()[0] != 'A') // 'N'
	{
	    cerr << "TestSerialCommunication::sendMessageWithVerification : \
	             Mauvais message recu" << endl;
	    
		n = -1;//return n;	
	}
	/*
	perror ("TestSerialCommunication::sendMessageWithVerification : \
	        Acknowledgment not present\n");
	*/
	
	// Message en out
	// ne le changer que si quelque chose est lu.
	message = answer;
	    
	cerr << "reponse : " << answer << " taille : " << n << endl;
	return n;
	
}

/// Open and initilise the serial communication.
int TestSerialCommunication::initCommunication()
{
    struct termios opt;
    
	cout << "INIT " << serialHandle << endl;
	
	if ((serialHandle = open (portName.c_str(), O_RDWR | O_NONBLOCK | O_NOCTTY)) < 0)
	{
	    perror ("TestSerialCommunication::initCommunication : open");
	    return false;
	}	
	
	cout << "opening succesful " << serialHandle << endl;
	
	//--set Baud Rate
	tcflush (serialHandle, TCIOFLUSH);  
	cfsetispeed(&opt, B57600);    
    cfsetospeed(&opt, B57600);  
    
    if (tcsetattr(serialHandle, TCSANOW, &opt) != 0)  
    {  
        perror ("TestSerialCommunication::initCommunication : tcsetattr speed");
        return false;  
    }  
    tcflush (serialHandle, TCIOFLUSH);  
        
    //--configure 8bits data, no parity, 1bit stop
    opt.c_cflag&=~CSIZE;
    opt.c_cflag|=CS8;
    opt.c_cflag&=~PARENB;
    opt.c_cflag &= ~CSTOPB; 
    tcflush(serialHandle,TCIFLUSH); 
    
    if (tcsetattr(serialHandle, TCSANOW, &opt) != 0)  
    {  
        perror ("TestSerialCommunication::initCommunication : tcsetattr conf");
        return false;
    }
    
    return true;
}
