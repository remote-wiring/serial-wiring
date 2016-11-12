
#include "UartSerial.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include "sys/ioctl.h"
#include <string.h>


using namespace remote_wiring::transport;
using namespace std;

UartSerial::UartSerial(const char *device) : _fd(-1), _bytes_available_callback(nullptr), _callbackContext(nullptr), _polling(true)
{
  bzero(&_tio_config, sizeof(_tio_config));
  strcpy(_device, device);
}

UartSerial::~UartSerial()
{
  end();
}

size_t UartSerial::available()
{
  if (_fd == -1) return 0;

  int numBytesAvailable;
  ioctl(_fd, FIONREAD, &numBytesAvailable);
  return numBytesAvailable;
}

void UartSerial::begin(uint32_t speed_, size_t config_)
{
  _fd = open(_device, O_RDWR | O_NOCTTY | O_NONBLOCK);
  
  // Unable to open the device
  if(_fd < 0) {perror(_device); exit(-1);}

  // File descriptor is not a TTY device
  if(!isatty(_fd)) {perror("UartSerial::begin - file descriptor is not a TTY device"); exit(-1);}

  // save current device settings
  if (tcgetattr(_fd, &_tio_original_config) < 0)
  {
    perror("UartSerial::begin - Unable to save current term attributes");
    exit(-1);
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
  }

  cfsetispeed(&_tio_config, baud_rate);
  cfsetospeed(&_tio_config, baud_rate);

  // Configure the termios structure. See termios man page for further info
  // http://man7.org/linux/man-pages/man3/termios.3.html

  // c_iflag - input modes
  // Leave all input flags unset

  // c_oflag - output modes
  // Leave all output flags unset

  // c_cflag - control modes
  _tio_config.c_cflag |= CS8;   // 8 bit characters
  _tio_config.c_cflag |= CREAD; // Enable receiver
  _tio_config.c_cflag |= CLOCAL;// Ignore modem control lines

  // c_lflag - local modes
  // Leave all local mode flags unset. This enables noncanonical mode input.

  // c_cc - special characters
  // The following combination of VTIME and VMIN will result in a polling
  // read. If data is available a call to read() returns immediately. If
  // no data is available a call to read() returns 0. 
  _tio_config.c_cc[VTIME]    = 0;
  _tio_config.c_cc[VMIN]     = 0;
  
  // Flush all current i/o data before enabling the new configuration
  tcflush(_fd, TCIOFLUSH);

  // Enable new term configuration
  if(tcsetattr(_fd, TCSANOW, &_tio_config) < 0)
  {
    perror("UartSerial::begin - Unable to set term attributes");
    exit(-1);
  }
}

void UartSerial::end()
{
  if(_fd == -1) return;
  
  flush();

  // Shut down the poll() thread
  _polling = false;
  _pollThread.join();

  // Restore the original settings
  tcsetattr(_fd, TCSANOW, &_tio_original_config);
  close(_fd);
  _fd = -1;
}

void UartSerial::flush()
{
  if(_fd != -1)
    tcdrain(_fd);
}

int UartSerial::read()
{
  if(_fd == -1) return -1;

  int buffer[1];
  int res = ::read(_fd, buffer, 1);
  if (res == 0) 
    return -1;
  else
    return buffer[0];
}

void UartSerial::write(uint8_t byte_)
{
  if(_fd != -1)
    ::write(_fd, &byte_, 1);
}

void UartSerial::registerSerialEventCallback (serialEvent bytes_available_, void *context_)
{
  if(_fd == -1) return;

  // For now only allow a single callback to be assigned
  if(_bytes_available_callback == nullptr)
  {
    _bytes_available_callback = bytes_available_;
    _callbackContext = context_;

    // Setup poll struct
    _fds[0].fd = _fd;
    _fds[0].events = POLLIN;

    _polling = true;
    _pollThread = std::thread(&UartSerial::pollForSerialData, this);
  }
}

void UartSerial::pollForSerialData()
{
  int pollrc = 0;
  while(_polling)
  {
    pollrc = poll(_fds, 1, 5); // timeout after 5ms
    if (pollrc > 0 && (_fds[0].revents & POLLIN))
    {
      _bytes_available_callback(_callbackContext);
    }
    else
    {
      // Release control back to the CPU
      this_thread::sleep_for(std::chrono::seconds(0));
    }
  }
}