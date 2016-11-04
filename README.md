serial-wiring
==============

## Library details:

The `serial-wiring` library is the transport layer designed for the `remote-wiring` library, and defines the `Stream` interface. The `Stream` interface is a compatible subset of the Arduino `Stream` interface. The `serial-wiring` library also provides the serial implementations for USB, Bluetooth, Bluetooth LE and Network serial streams under the `remote-wiring::transport` namespace.

Technical documentation, requirements and diagrams will be stored in the `docs/` folder. Sample programs are located in the `samples/` folder. Google Mock unit-tests are located in the `tests/` folder.

## Software License:
This project uses the ***MIT license***, please consult the license file for details.

## References:
[Serial - Wiring.org.co](http://wiring.org.co/reference/Serial.html)  
[Serial - Arduino.cc](https://www.arduino.cc/en/Reference/Serial)  
[Serial - Particle.io](https://docs.particle.io/reference/firmware/photon/#serial)  

## Example code:

#### Using `UartSerial()` with one parameter

```c++
#include <UartSerial.h>

using namespace remote_wiring::transport;

int main (int argc, char * argv[]) {
    Stream * stream = NULL;

    stream = new UartSerial("/dev/ttyO0");
    stream->begin();

    // Read and write bytes

    stream->end();
    delete stream;
    return 0;
}

```
