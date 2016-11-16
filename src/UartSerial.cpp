
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <string.h>
#include "UartSerial.h"

using namespace remote_wiring::transport;
using namespace std;

UartSerial::UartSerial(const char *device_) : _serial_file_descriptor(-1), _serial_device_path(NULL), _bytes_available_callback(nullptr), _callback_context(nullptr), _polling(false)
{
  bzero(&_tio_config, sizeof(_tio_config));

  int len = strlen(device_);
  if (len > 0)
  {
    _serial_device_path = (char *)malloc(len + 1);
    if (_serial_device_path != NULL)
    {
      strcpy(_serial_device_path, device_);
    }
  }
}

UartSerial::~UartSerial()
{
  end();
  free(_serial_device_path);
}

size_t UartSerial::available()
{
  if (_serial_file_descriptor == -1) { return 0; }

  int num_bytes_available;
  ioctl(_serial_file_descriptor, FIONREAD, &num_bytes_available);
  return num_bytes_available;
}

void UartSerial::begin(uint32_t speed_, size_t config_)
{
  _serial_file_descriptor = open(_serial_device_path, O_RDWR | O_NOCTTY | O_NONBLOCK);
  
  // Unable to open the device
  if(_serial_file_descriptor < 0) { perror(_serial_device_path); return; }

  // File descriptor is not a TTY device
  if(!isatty(_serial_file_descriptor)) 
  {
    perror("UartSerial::begin - file descriptor is not a TTY device"); 
    cleanupSerialFileDescriptor();
    return;
  }

  // save current device settings
  if (tcgetattr(_serial_file_descriptor, &_tio_original_config) < 0)
  {
    perror("UartSerial::begin - Unable to save current term attributes");
    cleanupSerialFileDescriptor();
    return;
  }

  speed_t baud_rate = speed_;

  // Handle common baud rates
  switch(speed_)
  {
    case 2400: baud_rate = B2400; break;
    case 4800: baud_rate = B4800; break;
    case 9600: baud_rate = B9600; break;
    case 19200: baud_rate = B19200; break;
    case 38400: baud_rate = B38400; break;
    case 57600: baud_rate = B57600; break;
    case 115200: baud_rate = B115200; break;
    case 230400: baud_rate = B230400; break;
    default: 
    {
      perror("UartSerial::begin - unsupported baud rate");
      cleanupSerialFileDescriptor();
      return;
    }
  }

  if (cfsetispeed(&_tio_config, baud_rate) != 0)
  {
    perror("UartSerial::begin - unable to set input baud rate");
    cleanupSerialFileDescriptor();
    return;
  }
  
  if (cfsetospeed(&_tio_config, baud_rate ) != 0)
  {
    perror("UartSerial::begin - unable to set output baud rate");
    cleanupSerialFileDescriptor();
    return;
  }

  // Configure the termios structure. See termios man page for further info
  // http://man7.org/linux/man-pages/man3/termios.3.html

  // c_iflag - input modes
  // Leave all input flags unset

  // c_oflag - output modes
  // Leave all output flags unset

  // c_cflag - control modes
  _tio_config.c_cflag |= config_; // Configuration passed by caller
  _tio_config.c_cflag |= CREAD;   // Enable receiver
  _tio_config.c_cflag |= CLOCAL;  // Ignore modem control lines

  // c_lflag - local modes
  // Leave all local mode flags unset. This enables noncanonical mode input.

  // c_cc - special characters
  // The following combination of VTIME and VMIN will result in a polling
  // read. If data is available a call to read() returns immediately. If
  // no data is available a call to read() returns 0. 
  _tio_config.c_cc[VTIME]    = 0;
  _tio_config.c_cc[VMIN]     = 0;
  
  // Flush all current i/o data before enabling the new configuration
  if (tcflush(_serial_file_descriptor, TCIOFLUSH) != 0)
  {
    perror("UartSerial::begin - unable to flush non-transmitted output data");
    cleanupSerialFileDescriptor();
    return;
  }

  // Enable new term configuration
  if(tcsetattr(_serial_file_descriptor, TCSANOW, &_tio_config) < 0)
  {
    perror("UartSerial::begin - Unable to set term attributes");
    cleanupSerialFileDescriptor();
    return;
  }
}

void UartSerial::end()
{
  if(_serial_file_descriptor == -1) { return; }
  
  flush();

  // Shut down the poll() thread
  _polling = false;
  _poll_thread.join();

  // Restore the original settings
  if (tcsetattr(_serial_file_descriptor, TCSANOW, &_tio_original_config))
  {
    perror("UartSerial::end - Unable to restore term attributes");
  }
  cleanupSerialFileDescriptor();
}

void UartSerial::flush()
{
  if(_serial_file_descriptor == -1) { return; }

  if (tcdrain(_serial_file_descriptor) != 0)
  {
    perror("UartSerial::flush - tcdrain failed");
  }
}

int UartSerial::read()
{
  if(_serial_file_descriptor == -1) { return -1; }

  uint8_t buffer;
  int res = ::read(_serial_file_descriptor, &buffer, 1);
  if (res == 0) 
    return -1;
  else
    return (int)buffer;
}

void UartSerial::write(uint8_t byte_)
{
  if(_serial_file_descriptor == -1) { return; }

  ::write(_serial_file_descriptor, &byte_, 1);
}

void UartSerial::registerSerialEventCallback (serialEvent bytes_available_, void *context_)
{
  if(_serial_file_descriptor == -1) { return; }

  // For now only allow a single callback to be assigned
  if(_bytes_available_callback == nullptr)
  {
    _bytes_available_callback = bytes_available_;
    _callback_context = context_;

    // Setup poll struct
    _polling_file_descriptor.fd = _serial_file_descriptor;
    _polling_file_descriptor.events = POLLIN;

    _polling = true;
    _poll_thread = std::thread(&UartSerial::pollForSerialData, this);
  }
}

void UartSerial::pollForSerialData()
{
  int pollrc = 0;
  while(_polling)
  {
    pollrc = poll(&_polling_file_descriptor, 1, 5); // timeout after 5ms
    if (pollrc > 0 && (_polling_file_descriptor.revents & POLLIN))
    {
      _bytes_available_callback(_callback_context);
    }
    else
    {
      // Release control back to the CPU
      this_thread::sleep_for(std::chrono::seconds(0));
    }
  }
}

void UartSerial::cleanupSerialFileDescriptor()
{
  ::close(_serial_file_descriptor);
  _serial_file_descriptor = -1;
}
