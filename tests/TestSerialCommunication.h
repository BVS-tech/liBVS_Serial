#ifndef Test_SERIAL_COMMUNICATION_H
#define Test_SERIAL_COMMUNICATION_H

#include "liBVSSerialCommunication.h"


// COM ordi      "/dev/ttyUSB0"
// COM raspberry "/dev/ttyAMA0"

/**
 * \TestSerialCommunication
 * Class to through serial communication using the Test protcol.
 *
 * The user must redefine the function `initCommunication` to open 
 * the serial port and set its communication parameters.
 */
class TestSerialCommunication : public SerialCommunication
{

    public :
        TestSerialCommunication(std::string COMName);
    
        bool messageFocusPlus();
        bool messageFocusMinus();
        bool messageColorPlus();
        bool messageColorMinus();
        bool messagePowerPlus();
        bool messagePowerMinus();
        
        /// Ask information about the current state of the Light.
        /**
         * \param status [out] if the light is on or off
         * \param focus [out] focus level
         * \param color [out] color selected
         * \param power [out] power level
         * \param timeout [in] Number of time the message for information
         * is send when the answer is not directly given
         * \param delay [in], time (in ms) between two conseccutives messages.
         *
         * \return if the communication went right
         */
        bool askInformation(int &status,int &focus,int &color,int &power, int timeout = 5, int delay = 2);
        
    protected :             
        /// Send a message and check for acknowledgment
        /**
         *
         * \param message  [in out] message to send, message received.
         * \param delay [in] time to wait for the acknowledgment (in ms)
         *
         * \return size of the answer
         */ 
        int sendMessageWithVerification(std::string& message, int delay = 2); 
        
        /// Open ans initilise the serial communication.
        int initCommunication();
};

#endif
