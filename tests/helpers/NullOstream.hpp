#ifndef WASMABI_NULL_OSTREAM_HPP
#define WASMABI_NULL_OSTREAM_HPP

#define PREVENT_PRINTING

#ifdef PREVENT_PRINTING
#define OSTREAM_DEFINITION NullOstream nullOstream;
#define TEST_OSTREAM nullOstream

#endif // PREVENT_PRINTING

#ifndef PREVENT_PRINTING
#define OSTREAM_DEFINITION
#define TEST_OSTREAM std::cout
#endif // !PREVENT_PRINTING

#include <iostream>

class NullBuffer : public std::streambuf {
public:
  int overflow(int c) { return c; }
};

class NullOstream : public std::ostream {
public:
  NullOstream() : std::ostream(&m_sb) {}

private:
  NullBuffer m_sb;
};

#endif // WASMABI_NULL_OSTREAM_HPP
