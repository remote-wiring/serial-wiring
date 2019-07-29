/* Created and copyrighted by Kevin Sidwar. Offered as open source under the MIT License (MIT). */

#ifndef SERIAL_WIRING_STREAM_H
#define SERIAL_WIRING_STREAM_H

#include <cerrno>
#include <cstddef>
#include <cstdint>
#include <stdarg.h>
#ifdef LOG_ERRORS
#include <cstdio>
#endif

namespace serial_wiring {

/*!
 * \brief Callback supplied to `Stream::registerSerialEventCallback`,
 *        and invoked when data is available.
 *
 * \param [in] context_ Context to use in the callback
 *
 * \note Use `Stream.read()` to capture this data.
 *
 * <a href="https://www.arduino.cc/en/Reference/SerialEvent">serialEvent (Arduino.cc)</a>
 */
typedef void(*serial_event_t)(void * context_);

/*!
 * \brief Stream is the base class for character and binary based streams
 *
 * Stream defines the reading functions in Arduino. It is not called
 * directly, but invoked whenever you use a function that relies on
 * it. When using any core functionality that uses a read() or similar
 * method, you can safely assume it calls on the Stream class.
 *
 * \sa <a href="https://www.arduino.cc/en/Reference/Stream">Stream (Arduino.cc)</a>
 */
template<typename T>
class Stream {
  public:
    /*!
     * \brief Checks the number of bytes available in the buffer
     *
     * \return The number of bytes available
     *
     * \sa <a href="https://www.arduino.cc/en/Reference/StreamAvailable">Stream::available (Arduino.cc)</a>
     */
    size_t available (void) {
        return static_cast<T&>(*this).available_impl();
    }

    /*!
     * \brief Initializes the underlying serial channel
     */
    void begin (void) 
    {
        int error;

        if ( 0 != (error = static_cast<T&>(*this).begin_impl()) ) {
            errno = error;
#ifdef LOG_ERRORS
            ::perror("ERROR: Stream::begin - Underlying implementation encountered error!");
#endif
        }
    }

    /*!
     * \brief Closes the underlying serial channel
     *
     * \sa <a href="https://www.arduino.cc/en/Reference/Serial/End">Serial::end (Arduino.cc)</a>
     */
    void end (void) 
    {
        int error;

        if ( 0 != (error = static_cast<T&>(*this).end_impl()) ) {
            errno = error;
#ifdef LOG_ERRORS
            ::perror("ERROR: Stream::end - Underlying implementation encountered error!");
#endif
        }
    }

    /*!
     * \brief Clears the transmit buffer once all outgoing characters
     *        have been sent
     *
     * \warning Documentation descrepincy: The documentation states a
     *          boolean value is returned, but the actual source shows
     *          the return value to be of type `void`.
     *
     * \sa <a href="https://github.com/arduino/Arduino/blob/master/hardware/arduino/avr/cores/arduino/Stream.h">GitHub: Arduino/.../Stream.h</a>
     * \sa <a href="https://www.arduino.cc/en/Reference/StreamFlush">Stream::flush (Arduino.cc)</a>
     */
    void flush (void) 
    {
        int error;

        if ( 0 != (error = static_cast<T&>(*this).flush_impl()) ) {
            errno = error;
#ifdef LOG_ERRORS
            ::perror("ERROR: Stream::flush - Underlying implementation encountered error!");
#endif
        }
    }

    /*!
     * \brief Fetches the next byte from the buffer
     *
     * \return The first byte of incoming data available
     *
     * \note Returns -1 if there is no byte, although this should
     *       be avoided by first checking `available()` to see if
     *       data is available.
     *
     * \sa Stream::available
     * \sa <a href="https://www.arduino.cc/en/Reference/StreamRead">Stream::read (Arduino.cc)</a>
     */
    int read (void) 
    {
        int result = static_cast<T&>(*this).read_impl();

        if ( result < -1 || result > 255 ) {
            errno = ERANGE;
#ifdef LOG_ERRORS
            ::perror("ERROR: Stream::read - Underlying implementation result out of range!");
#endif
        }

        return result;
    }

    /*!
     * \brief Register a serial data ready callback
     *
     * This callback shall be invoked when serial data has become available
     *
     * \param [in] uponBytesAvailable_ Callback invoked when byte(s) are
     *                                 available in the buffer
     *
     * \param [in] context_ Optional context provided to `uponBytesAvailable_`
     *                      via the `context_` parameter
     */
    void registerSerialEventCallback (serial_event_t uponBytesAvailable_,
                                      void * context_ = nullptr) 
    {
        int error;

        if ( !uponBytesAvailable_ ) { context_ = nullptr; }

        if ( 0 != (error = static_cast<T&>(*this).registerSerialEventCallback_impl(uponBytesAvailable_, context_)) ) {
            errno = error;
#ifdef LOG_ERRORS
            ::perror("ERROR: Stream::registerSerialEventCallback - Underlying implementation encountered error!");
#endif
        }
    }

    /*!
     * \brief Writes a byte to the stream
     *
     * \param [in] byte_ The byte of data to write into the stream
     *
     * \sa <a href="https://www.arduino.cc/en/Reference/Serial/Write">Serial::write (Arduino.cc)</a>
     */
    void write (uint8_t byte_) 
    {
        int error;

        if ( 0 != (error = static_cast<T&>(*this).write_impl(byte_)) ) {
            errno = error;
#ifdef LOG_ERRORS
            ::perror("ERROR: Stream::write - Underlying implementation encountered error!");
#endif
        }
    }

    void print (const char *fmt, ...){
        va_list args;
        va_start(args, fmt);
        static_cast<T&>(*this).print_impl(fmt, args);
        va_end(args);
    }

    bool is_connected(){
        return static_cast<T&>(*this).is_connected_impl();
    }

  protected:
    ~Stream (void) {}
};

}  // namespace serial_wiring

#endif // SERIAL_WIRING_STREAM_H
