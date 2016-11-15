/*
  UartSerial Echo Sample

This sample demonstrates the serialEvent callback ability of serial wiring
as well as basic transmit and receive. It is meant to be run with an 
Arduino attached to your machine via USB cable. 

The Arduino is programmed with the SerialEvent sample sketch which reads 
characters in on the serialport until a newline is encountered. Once the 
newline is received it transmits the entire string back out over serial. 
That transmission triggers the serial_event() callback in the code below.

Take note on line 50 that the path to the serial device is different based
on the platform on which you are compiling.

To build this sample ensure you have the C++ dev tools installed on your
platform and run the following command from the root folder of the repo

g++ samples/uart_echo.cpp src/UartSerial.cpp -I include -pthread -std=c++11 -Wall -g -Werror -o uart_echo

*/

#include <unistd.h>
#include <iostream>
#include "Stream.h"
#include "UartSerial.h"

using namespace std;
using namespace remote_wiring::transport;


void serial_event(void *context)
{
    // Cast the context to be a UartSerial object so we can read
    // characters from it
    Stream *serial = reinterpret_cast<UartSerial*>(context);
    while(serial->available())
    {
        char inChar = (char)serial->read();
        cout << inChar;
    }
    fflush(stdout);
}

int main (int argc, char * argv[]) {
    // Create a new UartSerial object that uses the path to the Arduino serial
    // device. On Mac OSX the path is /dev/cu.usbmodem1411. On most Linux
    // platforms including the Raspberry Pi the path needs to be changed to
    // /dev/ttyACM0
    UartSerial *serial = new UartSerial("/dev/cu.usbmodem1411");

    // The sample Arduino sketch is expecting a baud rate of 9600
    serial->begin(9600);

    // Register our serial_event function to be called when the UartSerial
    // object receives new serial data. Pass the UartSerial object as the 
    // context which will be available when serial_event is called
    serial->registerSerialEventCallback(&serial_event, serial);
    
    // By default Arduinos go through a hard reset when they receive a new
    // serial connection. The following sleep() is to allow it time to
    // boot up before sending serial data.
    sleep(2);

    // Write data to the Arduino over the serial port. Try changing these
    // lines of code to see what happens when you write different values
    serial->write('H');
    serial->write('e');
    serial->write('l');
    serial->write('l');
    serial->write('o');

    cout << "Press enter to send the newline character to the Arduino: ";
    getchar();
    // Send the newline which tells the Arduino sketch to echo back all of the
    // characters received so far
    serial->write('\n');
    
    // Give the Arduino time to send the string back over the serial line
    sleep(1);

    // Close the serial connection and cleanup the UartSerial object
    serial->end();
    delete serial;
    return 0;
}