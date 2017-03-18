/* Created and copyrighted by Kevin D. Sidwar. Offered as open source under the MIT License (MIT). */

#include "UartSerial.h"

#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>

#include <chrono>
#include <climits>
#include <cstdlib>
#include <cstring>

#define SERIAL_CONFIG_INVALID 0x0001

using namespace serial_wiring;

UartSerial::UartSerial (
    const char * device_
) :
    _bytesAvailableCallback(nullptr),
    _bytes_available_context(nullptr),
    _poll_thread(),
    _polling(false),
    _polling_file_descriptor{0},
    _serial_device_path(nullptr),
    _serial_file_descriptor(-1),
    _tio_config{0},
    _tio_config_original{0}
{
    int len = ::strnlen(device_, PATH_MAX);

    if ( 0 == len ) {
        ::perror("UartSerial::UartSerial - Invalid file serial device path");
    } else if ( NULL == (_serial_device_path = (char *)::malloc(len + 1))) {
        ::perror("UartSerial::UartSerial - Unable to allocate memory to store serial device path");
    } else {
        ::strncpy(_serial_device_path, device_, len);
        _serial_device_path[len] = '\0';
    }
}

UartSerial::~UartSerial (
    void
) {
    //TODO: Allow `end()` to be called more than once.
    ::free(_serial_device_path);
}

size_t
UartSerial::_available (
    void
) {
    size_t num_bytes_available = 0;

    if ( -1 == _serial_file_descriptor ) {
        ::perror("UartSerial::available - Invalid file descriptor");
    } else if ( -1 == ::ioctl(_serial_file_descriptor, FIONREAD, &num_bytes_available) ) {
        ::perror("UartSerial::available - `ioctl()` request failed");
        num_bytes_available = 0;
    }

    return num_bytes_available;
}

void
UartSerial::_end (
    void
) {
    if ( -1 == _serial_file_descriptor ) {
        // `end()` has already been called, no action required
    } else {
        flush();

        // Empty the I/O buffers
        if ( 0 != ::tcflush(_serial_file_descriptor, TCIOFLUSH) ) {
            ::perror("UartSerial::flush - Unable to flush the serial buffer");
        }

        // Shut down the poll() thread
        _polling = false;
        _poll_thread.join();

        // Restore the original settings and close the descriptor
        if ( 0 != ::tcsetattr(_serial_file_descriptor, TCSANOW, &_tio_config_original) ) {
            ::perror("UartSerial::end - Unable to restore term attributes");
        }

        if ( 0 != cleanupSerialFileDescriptor() ) {
            ::perror("UartSerial::end - Unable to close serial file descriptor");
        }
    }
}

void
UartSerial::_flush (
    void
) {
    if ( -1 == _serial_file_descriptor ) {
        ::perror("UartSerial::flush - Invalid file descriptor");
    } else if ( 0 != ::tcdrain(_serial_file_descriptor) ) {
        ::perror("UartSerial::flush - Unable to transmit data in the serial buffer");
    } else {
    }
}

int
UartSerial::_read (
    void
) {
    int buffer;

    if ( -1 == _serial_file_descriptor ) {
        ::perror("UartSerial::read - Invalid file descriptor");
        buffer = -1;
    } else if ( 0 == ::read(_serial_file_descriptor, &buffer, 1) ) {
        buffer = -1;
    } else {
        buffer &= 0xFF;
    }

    return buffer;
}

void
UartSerial::_registerSerialEventCallback (
    serialEvent upon_bytes_available_,
    void * context_
) {
    if ( -1 == _serial_file_descriptor ) {
        ::perror("UartSerial::registerSerialEventCallback - Invalid file descriptor");
    } else {
        _bytesAvailableCallback = upon_bytes_available_;
        _bytes_available_context = context_;

        if ( _bytesAvailableCallback && !_polling ) {
            _poll_thread = std::thread(&UartSerial::pollForSerialData, this);
        }
    }
}

void
UartSerial::_write (
    uint8_t byte_
) {
    if ( -1 == _serial_file_descriptor ) {
        ::perror("UartSerial::write - Invalid file descriptor");
    } else if ( 0 > ::write(_serial_file_descriptor, &byte_, 1) ) {
        ::perror("UartSerial::write - Unable to write data");
    }
}

void
UartSerial::begin (
    const size_t speed_,
    const size_t config_
) {
    // Scrubbed parameters
    speed_t baud_rate = B0;
    size_t c_cflags = 0;

    // Interpret common baud rates
    switch (speed_) {
      case 300: baud_rate = B300; break;
      case 600: baud_rate = B600; break;
      case 1200: baud_rate = B1200; break;
      case 2400: baud_rate = B2400; break;
      case 4800: baud_rate = B4800; break;
      case 9600: baud_rate = B9600; break;
      case 14400: break;
      case 19200: baud_rate = B19200; break;
      case 28800: break;
      case 38400: baud_rate = B38400; break;
      case 57600: baud_rate = B57600; break;
      case 115200: baud_rate = B115200; break;
      default: break;
    }

    // Initialize the value for c_cflag with the options needed for the
    // character size, parity and stop bits.
    switch (config_) {
      case wiring::SERIAL_5E1: c_cflags = (CS5|PARENB); break;
      case wiring::SERIAL_5E2: c_cflags = (CS5|PARENB|CSTOPB); break;
      case wiring::SERIAL_5N1: c_cflags = CS5; break;
      case wiring::SERIAL_5N2: c_cflags = (CS5|CSTOPB); break;
      case wiring::SERIAL_5O1: c_cflags = (CS5|PARENB|PARODD); break;
      case wiring::SERIAL_5O2: c_cflags = (CS5|PARENB|PARODD|CSTOPB); break;
      case wiring::SERIAL_6E1: c_cflags = (CS6|PARENB); break;
      case wiring::SERIAL_6E2: c_cflags = (CS6|PARENB|CSTOPB); break;
      case wiring::SERIAL_6N1: c_cflags = CS6; break;
      case wiring::SERIAL_6N2: c_cflags = (CS6|CSTOPB); break;
      case wiring::SERIAL_6O1: c_cflags = (CS6|PARENB|PARODD); break;
      case wiring::SERIAL_6O2: c_cflags = (CS6|PARENB|PARODD|CSTOPB); break;
      case wiring::SERIAL_7E1: c_cflags = (CS7|PARENB); break;
      case wiring::SERIAL_7E2: c_cflags = (CS7|PARENB|CSTOPB); break;
      case wiring::SERIAL_7N1: c_cflags = CS7; break;
      case wiring::SERIAL_7N2: c_cflags = (CS7|CSTOPB); break;
      case wiring::SERIAL_7O1: c_cflags = (CS7|PARENB|PARODD); break;
      case wiring::SERIAL_7O2: c_cflags = (CS7|PARENB|PARODD|CSTOPB); break;
      case wiring::SERIAL_8E1: c_cflags = (CS8|PARENB); break;
      case wiring::SERIAL_8E2: c_cflags = (CS8|PARENB|CSTOPB); break;
      case wiring::SERIAL_8N1: c_cflags = CS8; break;
      case wiring::SERIAL_8N2: c_cflags = (CS8|CSTOPB); break;
      case wiring::SERIAL_8O1: c_cflags = (CS8|PARENB|PARODD); break;
      case wiring::SERIAL_8O2: c_cflags = (CS8|PARENB|PARODD|CSTOPB); break;
      default: c_cflags = SERIAL_CONFIG_INVALID; break;
    }

    // Validate baud rate
    if ( B0 == baud_rate ) {
        ::perror("UartSerial::begin - Unsupported baud rate");
    // Validate configuration flags
    } else if ( SERIAL_CONFIG_INVALID == c_cflags ) {
        ::perror("UartSerial::begin - Unsupported configuration");
    // Attempt to open the device
    } else if ( 0 > (_serial_file_descriptor = ::open(_serial_device_path, (O_RDWR | O_NOCTTY | O_NONBLOCK))) ) {
        ::perror(_serial_device_path);
    // Confirm file descriptor is a TTY device
    } else if ( 0 == ::isatty(_serial_file_descriptor) ) {
        ::perror("UartSerial::begin - File descriptor is not a TTY device");
        if ( 0 != cleanupSerialFileDescriptor() ) {
            ::perror("UartSerial::begin - Unable to close serial file descriptor");
        }
    // Save current device settings
    } else if ( 0 != ::tcgetattr(_serial_file_descriptor, &_tio_config_original) ) {
        ::perror("UartSerial::begin - Unable to save current term attributes");
        if ( 0 != cleanupSerialFileDescriptor() ) {
          ::perror("UartSerial::begin - Unable to close serial file descriptor");
        }
    // Flush all current i/o data before enabling the new configuration
    } else if ( 0 != ::tcflush(_serial_file_descriptor, TCIOFLUSH) ) {
        ::perror("UartSerial::begin - Unable to flush file descriptor");
        if ( 0 != cleanupSerialFileDescriptor() ) {
            ::perror("UartSerial::begin - Unable to close serial file descriptor");
        }
    } else {
        // Configure the termios structure. See termios man page for further info
        // http://man7.org/linux/man-pages/man3/termios.3.html

        // c_iflag - input modes
        // Leave all input flags unset

        // c_oflag - output modes
        // Leave all output flags unset

        // c_cflag - control modes
        _tio_config.c_cflag = c_cflags;
        _tio_config.c_cflag |= CREAD;   // Enable receiver
        _tio_config.c_cflag |= CLOCAL;  // Ignore modem control lines
        _tio_config.c_cflag |= HUPCL;  // Enable hang-up on close

        // c_lflag - local modes
        // Leave all local mode flags unset. This enables noncanonical mode input.

        // c_cc - special characters
        // The following combination of VTIME and VMIN will result in a polling
        // read. If data is available a call to read() returns immediately. If
        // no data is available a call to read() returns 0.
        _tio_config.c_cc[VTIME] = 0;
        _tio_config.c_cc[VMIN] = 0;

        // Update configuration
        if ( 0 != ::cfsetspeed(&_tio_config, baud_rate) ) {
            ::perror("UartSerial::begin - Unable to set baud rate");
            if ( 0 != cleanupSerialFileDescriptor() ) {
                ::perror("UartSerial::begin - Unable to close serial file descriptor");
            }
        // Enable new term configuration
        } else if ( 0 != ::tcsetattr(_serial_file_descriptor, TCSANOW, &_tio_config) ) {
            ::perror("UartSerial::begin - Unable to set term attributes");
            if ( 0 != cleanupSerialFileDescriptor() ) {
                ::perror("UartSerial::begin - Unable to close serial file descriptor");
            }
        }
    }
}

int
UartSerial::cleanupSerialFileDescriptor (
    void
) {
    int error;

    if ( 0 != ::close(_serial_file_descriptor) ) {
        error = __LINE__;
    } else {
        _serial_file_descriptor = -1;
        error = 0;
    }

    return error;
}

void
UartSerial::pollForSerialData (
    void
) {
    const int timeout_ms = 0;

    // Setup poll struct
    _polling_file_descriptor.fd = _serial_file_descriptor;
    _polling_file_descriptor.events = POLLIN;

    // Begin polling loop
    for ( _polling = true ; _polling ; ) {
        switch ( ::poll(&_polling_file_descriptor, 1, timeout_ms) ) {
          case -1:  // error
            ::perror("UartSerial::pollForSerialData - Polling error occurred");
            break;
          case 0:  // timeout
            // Release control back to the CPU
            std::this_thread::sleep_for(std::chrono::seconds(0));
            break;
          default:
            if ( (_polling_file_descriptor.revents & POLLIN) && _bytesAvailableCallback ) {
                _bytesAvailableCallback(_bytes_available_context);
            } else {
                // Release control back to the CPU
                std::this_thread::sleep_for(std::chrono::seconds(0));
            }
        }
    }
}

/* Created and copyrighted by Kevin D. Sidwar. Offered as open source under the MIT License (MIT). */
