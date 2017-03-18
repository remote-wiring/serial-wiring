Serial Wiring
==============

## Library details:

The `serial_wiring` library is designed to support the `wiring` library, and defines the `Stream` interface. The `Stream` interface is a compatible subset of the `Arduino::Stream` interface. The `serial_wiring` library also provides the serial implementations for USB, Bluetooth, Bluetooth LE and Network serial streams under the `serial_wiring` namespace.

Technical documentation, requirements and diagrams will be stored in the `docs/` folder. Sample programs are located in the `samples/` folder. Google Mock unit-tests are located in the `tests/` folder.

## Software License:
This project uses the ***MIT license***, please consult the license file for details.

## References:
[Serial - Wiring.org.co](http://wiring.org.co/reference/Serial.html)
[Serial - Arduino.cc](https://www.arduino.cc/en/Reference/Serial)
[Serial - Particle.io](https://docs.particle.io/reference/firmware/photon/#serial)

## Example code:

#### Creating `UartSerial()` with one parameter

```c++
#include <UartSerial.h>

using namespace serial_wiring;

int main (int argc, char * argv[]) {
    UartSerial usb("/dev/ttyACM0");
    usb.begin();

    // Read and write bytes using the Arduino::Stream interface

    usb.end();
    return 0;
}

```
