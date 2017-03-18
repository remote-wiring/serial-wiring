/*

UartSerial Echo Sample
======================

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

g++ uart_echo.cpp -o uart_echo -std=c++11 -Wall -Werror -I../include -L../bin -lserial_wiring -lpthread -g

*/

#include <serial_wiring>

#include <string.h>
#include <unistd.h>

#include <iostream>

void onSerialEvent (void *context_) {
    // Cast the context to be a UartSerial object so we can read
    // characters from it
    Stream * s = reinterpret_cast<Stream *>(context_);
    while(s->available())
    {
        char inChar = (char)s->read();
        std::cout << inChar << std::flush;
    }
}

int main (int argc, char * argv[]) {
    if ( argc < 2 ) { std::cout << "Usage: " << argv[0] << " <serial device descriptor>" << std::endl; return -1; }

    std::cout << "*****************************************************" << std::endl;
    std::cout << "** The \"Examples > 04.Communication > SerialEvent\" **" << std::endl;
    std::cout << "** sketch must be deployed to the Arduino in order **" << std::endl;
    std::cout << "** for the sample to work correctly.               **" << std::endl;
    std::cout << "*****************************************************" << std::endl;

    // Buffer variables used to receive user input and send to Arduino
    char * user_input_buffer = NULL;
    size_t user_input_buffer_length = 0;

    // Create a new UartSerial object that uses the path to the Arduino serial
    // device. On Mac OSX the path is /dev/cu.usbmodem1411. On most Linux
    // platforms including the Raspberry Pi the path needs to be changed to
    // /dev/ttyACM0
    serial_wiring::UartSerial usb(argv[1]);

    // The sample Arduino sketch is expecting a baud rate of 9600
    usb.begin(9600);

    // Register our serial_event function to be called when the UartSerial
    // object receives new serial data. Pass the UartSerial object as the
    // context which will be available when serial_event is called
    usb.registerSerialEventCallback(onSerialEvent, &usb);

    // By default Arduinos go through a hard reset when they receive a new
    // serial connection. The following sleep() is to allow it time to
    // boot up before sending serial data.
    ::sleep(2);

    // Get user input
    std::cout << "Type a message to send to the Arduino, then press <Enter>: ";
    getline(&user_input_buffer, &user_input_buffer_length, stdin);
    std::cout << std::endl;

    // user_input_buffer_length is set to the size of the allocated memory, and
    // now we need to trim that down the the length of string the user typed
    user_input_buffer_length = ::strnlen(user_input_buffer, user_input_buffer_length);

    // Send the user input to the Arduino sketch to echo back
    for (size_t i = 0 ; i < user_input_buffer_length ; ++i) {
        usb.write(user_input_buffer[i]);
    }
    free(user_input_buffer);

    std::cout << "Arduino says..." << std::endl;

    // Give the Arduino time to send the string back over the serial line
    sleep(1);

    // Close the serial connection and cleanup the UartSerial object
    usb.end();

    return 0;
}
