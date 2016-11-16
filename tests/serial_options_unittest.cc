#include <limits.h>
#include "UartSerial.h"
#include "gtest/gtest.h"

TEST(SerialOptions, SERIAL_5E1) {

  // Ensure the flags are correct
  ASSERT_EQ(remote_wiring::SERIAL_5E1, CS5 | PARENB);

  remote_wiring::transport::SerialOptions opts;
  opts.config = remote_wiring::SERIAL_5E1;

  EXPECT_EQ(opts.flags.char_count, (5 - 5));
  EXPECT_EQ(opts.flags.parity_enable, 1);
  EXPECT_EQ(opts.flags.parity_odd, 0);
  EXPECT_EQ(opts.flags.stop_bit_2, 0);
}

TEST(SerialOptions, SERIAL_5E2) {

  // Ensure the flags are correct
  ASSERT_EQ(remote_wiring::SERIAL_5E2, CS5 | PARENB | CSTOPB);

  remote_wiring::transport::SerialOptions opts;
  opts.config = remote_wiring::SERIAL_5E2;

  EXPECT_EQ(opts.flags.char_count, (5 - 5));
  EXPECT_EQ(opts.flags.parity_enable, 1);
  EXPECT_EQ(opts.flags.parity_odd, 0);
  EXPECT_EQ(opts.flags.stop_bit_2, 1);
}

TEST(SerialOptions, SERIAL_5N1) {

  // Ensure the flags are correct
  ASSERT_EQ(remote_wiring::SERIAL_5N1, CS5);

  remote_wiring::transport::SerialOptions opts;
  opts.config = remote_wiring::SERIAL_5N1;

  EXPECT_EQ(opts.flags.char_count, (5 - 5));
  EXPECT_EQ(opts.flags.parity_enable, 0);
  EXPECT_EQ(opts.flags.parity_odd, 0);
  EXPECT_EQ(opts.flags.stop_bit_2, 0);
}

TEST(SerialOptions, SERIAL_5N2) {

  // Ensure the flags are correct
  ASSERT_EQ(remote_wiring::SERIAL_5N2, CS5 | CSTOPB);

  remote_wiring::transport::SerialOptions opts;
  opts.config = remote_wiring::SERIAL_5N2;

  EXPECT_EQ(opts.flags.char_count, (5 - 5));
  EXPECT_EQ(opts.flags.parity_enable, 0);
  EXPECT_EQ(opts.flags.parity_odd, 0);
  EXPECT_EQ(opts.flags.stop_bit_2, 1);
}

TEST(SerialOptions, SERIAL_5O1) {

  // Ensure the flags are correct
  ASSERT_EQ(remote_wiring::SERIAL_5O1, CS5 | PARENB | PARODD);

  remote_wiring::transport::SerialOptions opts;
  opts.config = remote_wiring::SERIAL_5O1;

  EXPECT_EQ(opts.flags.char_count, (5 - 5));
  EXPECT_EQ(opts.flags.parity_enable, 1);
  EXPECT_EQ(opts.flags.parity_odd, 1);
  EXPECT_EQ(opts.flags.stop_bit_2, 0);
}

TEST(SerialOptions, SERIAL_5O2) {

  // Ensure the flags are correct
  ASSERT_EQ(remote_wiring::SERIAL_5O2, CS5 | PARENB | PARODD | CSTOPB);

  remote_wiring::transport::SerialOptions opts;
  opts.config = remote_wiring::SERIAL_5O2;

  EXPECT_EQ(opts.flags.char_count, (5 - 5));
  EXPECT_EQ(opts.flags.parity_enable, 1);
  EXPECT_EQ(opts.flags.parity_odd, 1);
  EXPECT_EQ(opts.flags.stop_bit_2, 1);
}

TEST(SerialOptions, SERIAL_6E1) {

  // Ensure the flags are correct
  ASSERT_EQ(remote_wiring::SERIAL_6E1, CS6 | PARENB);

  remote_wiring::transport::SerialOptions opts;
  opts.config = remote_wiring::SERIAL_6E1;

  EXPECT_EQ(opts.flags.char_count, (6 - 5));
  EXPECT_EQ(opts.flags.parity_enable, 1);
  EXPECT_EQ(opts.flags.parity_odd, 0);
  EXPECT_EQ(opts.flags.stop_bit_2, 0);
}

TEST(SerialOptions, SERIAL_6E2) {

  // Ensure the flags are correct
  ASSERT_EQ(remote_wiring::SERIAL_6E2, CS6 | PARENB | CSTOPB);

  remote_wiring::transport::SerialOptions opts;
  opts.config = remote_wiring::SERIAL_6E2;

  EXPECT_EQ(opts.flags.char_count, (6 - 5));
  EXPECT_EQ(opts.flags.parity_enable, 1);
  EXPECT_EQ(opts.flags.parity_odd, 0);
  EXPECT_EQ(opts.flags.stop_bit_2, 1);
}

TEST(SerialOptions, SERIAL_6N1) {

  // Ensure the flags are correct
  ASSERT_EQ(remote_wiring::SERIAL_6N1, CS6);

  remote_wiring::transport::SerialOptions opts;
  opts.config = remote_wiring::SERIAL_6N1;

  EXPECT_EQ(opts.flags.char_count, (6 - 5));
  EXPECT_EQ(opts.flags.parity_enable, 0);
  EXPECT_EQ(opts.flags.parity_odd, 0);
  EXPECT_EQ(opts.flags.stop_bit_2, 0);
}

TEST(SerialOptions, SERIAL_6N2) {

  // Ensure the flags are correct
  ASSERT_EQ(remote_wiring::SERIAL_6N2, CS6 | CSTOPB);

  remote_wiring::transport::SerialOptions opts;
  opts.config = remote_wiring::SERIAL_6N2;

  EXPECT_EQ(opts.flags.char_count, (6 - 5));
  EXPECT_EQ(opts.flags.parity_enable, 0);
  EXPECT_EQ(opts.flags.parity_odd, 0);
  EXPECT_EQ(opts.flags.stop_bit_2, 1);
}

TEST(SerialOptions, SERIAL_6O1) {

  // Ensure the flags are correct
  ASSERT_EQ(remote_wiring::SERIAL_6O1, CS6 | PARENB | PARODD);

  remote_wiring::transport::SerialOptions opts;
  opts.config = remote_wiring::SERIAL_6O1;

  EXPECT_EQ(opts.flags.char_count, (6 - 5));
  EXPECT_EQ(opts.flags.parity_enable, 1);
  EXPECT_EQ(opts.flags.parity_odd, 1);
  EXPECT_EQ(opts.flags.stop_bit_2, 0);
}

TEST(SerialOptions, SERIAL_6O2) {

  // Ensure the flags are correct
  ASSERT_EQ(remote_wiring::SERIAL_6O2, CS6 | PARENB | PARODD | CSTOPB);

  remote_wiring::transport::SerialOptions opts;
  opts.config = remote_wiring::SERIAL_6O2;

  EXPECT_EQ(opts.flags.char_count, (6 - 5));
  EXPECT_EQ(opts.flags.parity_enable, 1);
  EXPECT_EQ(opts.flags.parity_odd, 1);
  EXPECT_EQ(opts.flags.stop_bit_2, 1);
}

TEST(SerialOptions, SERIAL_7E1) {

  // Ensure the flags are correct
  ASSERT_EQ(remote_wiring::SERIAL_7E1, CS7 | PARENB);

  remote_wiring::transport::SerialOptions opts;
  opts.config = remote_wiring::SERIAL_7E1;

  EXPECT_EQ(opts.flags.char_count, (7 - 5));
  EXPECT_EQ(opts.flags.parity_enable, 1);
  EXPECT_EQ(opts.flags.parity_odd, 0);
  EXPECT_EQ(opts.flags.stop_bit_2, 0);
}

TEST(SerialOptions, SERIAL_7E2) {

  // Ensure the flags are correct
  ASSERT_EQ(remote_wiring::SERIAL_7E2, CS7 | PARENB | CSTOPB);

  remote_wiring::transport::SerialOptions opts;
  opts.config = remote_wiring::SERIAL_7E2;

  EXPECT_EQ(opts.flags.char_count, (7 - 5));
  EXPECT_EQ(opts.flags.parity_enable, 1);
  EXPECT_EQ(opts.flags.parity_odd, 0);
  EXPECT_EQ(opts.flags.stop_bit_2, 1);
}

TEST(SerialOptions, SERIAL_7N1) {

  // Ensure the flags are correct
  ASSERT_EQ(remote_wiring::SERIAL_7N1, CS7);

  remote_wiring::transport::SerialOptions opts;
  opts.config = remote_wiring::SERIAL_7N1;

  EXPECT_EQ(opts.flags.char_count, (7 - 5));
  EXPECT_EQ(opts.flags.parity_enable, 0);
  EXPECT_EQ(opts.flags.parity_odd, 0);
  EXPECT_EQ(opts.flags.stop_bit_2, 0);
}

TEST(SerialOptions, SERIAL_7N2) {

  // Ensure the flags are correct
  ASSERT_EQ(remote_wiring::SERIAL_7N2, CS7 | CSTOPB);

  remote_wiring::transport::SerialOptions opts;
  opts.config = remote_wiring::SERIAL_7N2;

  EXPECT_EQ(opts.flags.char_count, (7 - 5));
  EXPECT_EQ(opts.flags.parity_enable, 0);
  EXPECT_EQ(opts.flags.parity_odd, 0);
  EXPECT_EQ(opts.flags.stop_bit_2, 1);
}

TEST(SerialOptions, SERIAL_7O1) {

  // Ensure the flags are correct
  ASSERT_EQ(remote_wiring::SERIAL_7O1, CS7 | PARENB | PARODD);

  remote_wiring::transport::SerialOptions opts;
  opts.config = remote_wiring::SERIAL_7O1;

  EXPECT_EQ(opts.flags.char_count, (7 - 5));
  EXPECT_EQ(opts.flags.parity_enable, 1);
  EXPECT_EQ(opts.flags.parity_odd, 1);
  EXPECT_EQ(opts.flags.stop_bit_2, 0);
}

TEST(SerialOptions, SERIAL_7O2) {

  // Ensure the flags are correct
  ASSERT_EQ(remote_wiring::SERIAL_7O2, CS7 | PARENB | PARODD | CSTOPB);

  remote_wiring::transport::SerialOptions opts;
  opts.config = remote_wiring::SERIAL_7O2;

  EXPECT_EQ(opts.flags.char_count, (7 - 5));
  EXPECT_EQ(opts.flags.parity_enable, 1);
  EXPECT_EQ(opts.flags.parity_odd, 1);
  EXPECT_EQ(opts.flags.stop_bit_2, 1);
}

TEST(SerialOptions, SERIAL_8E1) {

  // Ensure the flags are correct
  ASSERT_EQ(remote_wiring::SERIAL_8E1, CS8 | PARENB);

  remote_wiring::transport::SerialOptions opts;
  opts.config = remote_wiring::SERIAL_8E1;

  EXPECT_EQ(opts.flags.char_count, (8 - 5));
  EXPECT_EQ(opts.flags.parity_enable, 1);
  EXPECT_EQ(opts.flags.parity_odd, 0);
  EXPECT_EQ(opts.flags.stop_bit_2, 0);
}

TEST(SerialOptions, SERIAL_8E2) {

  // Ensure the flags are correct
  ASSERT_EQ(remote_wiring::SERIAL_8E2, CS8 | PARENB | CSTOPB);

  remote_wiring::transport::SerialOptions opts;
  opts.config = remote_wiring::SERIAL_8E2;

  EXPECT_EQ(opts.flags.char_count, (8 - 5));
  EXPECT_EQ(opts.flags.parity_enable, 1);
  EXPECT_EQ(opts.flags.parity_odd, 0);
  EXPECT_EQ(opts.flags.stop_bit_2, 1);
}

TEST(SerialOptions, SERIAL_8N1) {

  // Ensure the flags are correct
  ASSERT_EQ(remote_wiring::SERIAL_8N1, CS8);

  remote_wiring::transport::SerialOptions opts;
  opts.config = remote_wiring::SERIAL_8N1;

  EXPECT_EQ(opts.flags.char_count, (8 - 5));
  EXPECT_EQ(opts.flags.parity_enable, 0);
  EXPECT_EQ(opts.flags.parity_odd, 0);
  EXPECT_EQ(opts.flags.stop_bit_2, 0);
}

TEST(SerialOptions, SERIAL_8N2) {

  // Ensure the flags are correct
  ASSERT_EQ(remote_wiring::SERIAL_8N2, CS8 | CSTOPB);

  remote_wiring::transport::SerialOptions opts;
  opts.config = remote_wiring::SERIAL_8N2;

  EXPECT_EQ(opts.flags.char_count, (8 - 5));
  EXPECT_EQ(opts.flags.parity_enable, 0);
  EXPECT_EQ(opts.flags.parity_odd, 0);
  EXPECT_EQ(opts.flags.stop_bit_2, 1);
}

TEST(SerialOptions, SERIAL_8O1) {

  // Ensure the flags are correct
  ASSERT_EQ(remote_wiring::SERIAL_8O1, CS8 | PARENB | PARODD);

  remote_wiring::transport::SerialOptions opts;
  opts.config = remote_wiring::SERIAL_8O1;

  EXPECT_EQ(opts.flags.char_count, (8 - 5));
  EXPECT_EQ(opts.flags.parity_enable, 1);
  EXPECT_EQ(opts.flags.parity_odd, 1);
  EXPECT_EQ(opts.flags.stop_bit_2, 0);
}

TEST(SerialOptions, SERIAL_8O2) {

  // Ensure the flags are correct
  ASSERT_EQ(remote_wiring::SERIAL_8O2, CS8 | PARENB | PARODD | CSTOPB);

  remote_wiring::transport::SerialOptions opts;
  opts.config = remote_wiring::SERIAL_8O2;

  EXPECT_EQ(opts.flags.char_count, (8 - 5));
  EXPECT_EQ(opts.flags.parity_enable, 1);
  EXPECT_EQ(opts.flags.parity_odd, 1);
  EXPECT_EQ(opts.flags.stop_bit_2, 1);
}
