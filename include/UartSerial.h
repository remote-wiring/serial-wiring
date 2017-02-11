/* Created and copyrighted by Kevin D. Sidwar. Offered as open source under the MIT License (MIT). */

#ifndef UartSerial_H
#define UartSerial_H

#include <sys/poll.h>
#include <termios.h>

#include <atomic>
#include <thread>

#include "Stream.h"

namespace remote_wiring {

namespace wiring {

static const size_t SERIAL_5E1 = 0x1000;
static const size_t SERIAL_5E2 = 0x1400;
static const size_t SERIAL_5N1 = 0x0000;
static const size_t SERIAL_5N2 = 0x0400;
static const size_t SERIAL_5O1 = 0x3000;
static const size_t SERIAL_5O2 = 0x3400;
static const size_t SERIAL_6E1 = 0x1100;
static const size_t SERIAL_6E2 = 0x1500;
static const size_t SERIAL_6N1 = 0x0100;
static const size_t SERIAL_6N2 = 0x0500;
static const size_t SERIAL_6O1 = 0x3100;
static const size_t SERIAL_6O2 = 0x3500;
static const size_t SERIAL_7E1 = 0x1200;
static const size_t SERIAL_7E2 = 0x1600;
static const size_t SERIAL_7N1 = 0x0200;
static const size_t SERIAL_7N2 = 0x0600;
static const size_t SERIAL_7O1 = 0x3200;
static const size_t SERIAL_7O2 = 0x3600;
static const size_t SERIAL_8E1 = 0x1300;
static const size_t SERIAL_8E2 = 0x1700;
static const size_t SERIAL_8N1 = 0x0300;
static const size_t SERIAL_8N2 = 0x0700;
static const size_t SERIAL_8O1 = 0x3300;
static const size_t SERIAL_8O2 = 0x3700;

}  // wiring

/*!
* \brief SerialFlags describes the flags specified by the serial configuration
*        constants
*
* The serial configuration flags are designed to provide a level of
* granularity that would enable a user to interpret the serial configuration
* constants and configure any system. The flags are directly compatible
* with `termios_p->c_cflag` flags.
*
* \sa <a href="https://github.com/torvalds/linux/blob/master/include/uapi/asm-generic/termbits.h">termbits.h (Linux)</a>
*/
typedef struct SerialFlags {
  uint16_t reserved_1:8;
  uint16_t char_count:2;
  uint16_t stop_bit_2:1;
  uint16_t reserved_2:1;
  uint16_t parity_enable:1;
  uint16_t parity_odd:1;
  uint16_t reserved_3:2;
} SerialFlags;

/*!
* \brief SerialOptions is a union of a serial configuration constant
*        and SerialFlags
*
* This is used to generate bit fields to allow for inspection of
* the serial configuration flags.
*
* \sa SerialFlags
*/
union SerialOptions {
  size_t config;
  SerialFlags flags;
};

class UartSerial : public Stream {
  private:
    serialEvent _bytesAvailableCallback;
    void * _bytes_available_context;
    std::thread _poll_thread;
    std::atomic<bool> _polling;
    struct pollfd _polling_file_descriptor;
    char * _serial_device_path;
    int _serial_file_descriptor;
    struct termios _tio_config;
    struct termios _tio_config_original;

  public:
    UartSerial(const char *device);
    virtual ~UartSerial(void);
    size_t available (void);
    inline void begin(void) { begin(57600, wiring::SERIAL_8N1); }
    void end (void);
    void flush (void);
    int read (void);
    void write (uint8_t byte_);
    void registerSerialEventCallback (serialEvent bytes_available_, void *context_ = nullptr);

    /*!
    * \brief Sets the data rate in bits per second (baud) for
    *        serial data transmission
    *
    * For communicating with the computer, use one of these rates:
    * 300, 600, 1200, 2400, 4800, 9600, 14400, 19200, 28800, 38400,
    * 57600, or 115200. You can, however, specify other rates - for
    * example, to communicate over pins 0 and 1 with a component
    * that requires a particular baud rate.
    *
    * \param [in] speed_ Bits per second (baud)
    * \param [in] config_ Configures the data, parity, and stop bits.
    *                     The default is 8 data bits, no parity, one
    *                     stop bit.
    *
    * \sa <a href="https://www.arduino.cc/en/Reference/Serial/Begin">Serial::begin (Arduino.cc)</a>
    */
    void
    begin (
      const size_t speed_,
      const size_t config_ = wiring::SERIAL_8N1
    );

  private:
    int cleanupSerialFileDescriptor(void);
    void pollForSerialData(void);
};

} // namespace remote_wiring

#endif // UartSerial_H

/* Created and copyrighted by Kevin D. Sidwar. Offered as open source under the MIT License (MIT). */
