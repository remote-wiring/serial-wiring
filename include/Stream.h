/* Created and copyrighted by Zachary J. Fields. Offered as open source under the MIT License (MIT). */

#ifndef STREAM_H
#define STREAM_H

#include <cstddef>
#include <cstdint>

/*!
 * \brief Callback supplied to `Stream::registerSerialEventCallback`,
 *        and invoked when data is available.
 *
 * \param [in] Optional context to use in the callback. This value is stored
 *             and then supplied to the callback function later.
 *
 * \note Use `Stream.read()` to capture this data.
 *
 * <a href="https://www.arduino.cc/en/Reference/SerialEvent">serialEvent (Arduino.cc)</a>
 */
typedef void(*serialEvent)(void *context_);

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
struct Stream {
    virtual
    ~Stream (
        void
    ) {

    }

    /*!
     * \brief Checks the number of bytes available in the buffer
     *
     * \return The number of bytes available
     *
     * \sa <a href="https://www.arduino.cc/en/Reference/StreamAvailable">Stream::available (Arduino.cc)</a>
     */
    virtual
    size_t
    available (
        void
    ) = 0;

    /*!
     * \brief Initializes the underlying serial channel
     */
    virtual
    void
    begin (
        void
    ) = 0;

    /*!
     * \brief Closes the underlying serial channel
     *
     * \sa <a href="https://www.arduino.cc/en/Reference/Serial/End">Serial::end (Arduino.cc)</a>
     */
    virtual
    void
    end (
        void
    ) = 0;

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
    virtual
    void
    flush (
        void
    ) = 0;

    /*!
     * \brief Fetches the next byte from the buffer
     *
     * \return The first byte of incoming data available
     *
     * \note Returns -1 if there is no byte, although this should
     *       be avoided by first checking `available()` to see if
     *       data is available.
     *
     * \sa <a href="https://www.arduino.cc/en/Reference/StreamRead">Stream::read (Arduino.cc)</a>
     */
    virtual
    int
    read (
        void
    ) = 0;

    /*!
     * \brief Registers a callback to be called when serial data is
     *        available to read from the buffer
     *
     * \param [in] bytes_available_ Callback invoked when byte(s) are
     *                              available in the buffer
     *
     * \param [in] context_ Optional context to send back to bytes_available_
     *                      when called.
     */
    virtual
    void
    registerSerialEventCallback (
        serialEvent bytes_available_,
        void *context_ = nullptr
    ) = 0;

    /*!
     * \brief Writes a byte to the stream
     *
     * \param [in] byte_ The byte of data to write into the stream
     *
     * \sa <a href="https://www.arduino.cc/en/Reference/Serial/Write">Serial::write (Arduino.cc)</a>
     */
    virtual
    void
    write (
        uint8_t byte_
    ) = 0;
};

#endif // STREAM_H

/* Created and copyrighted by Zachary J. Fields. Offered as open source under the MIT License (MIT). */

