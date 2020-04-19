#ifndef WASMABI_OSTREAM_MODIFIERS_HPP
#define WASMABI_OSTREAM_MODIFIERS_HPP
#include <iostream>

namespace wasmabi {

class OstreamModifier {
public:
  enum Code {
    RED = 31,
    GREEN = 32,
    BLUE = 34,
    DEFAULT = 39,
    BG_RED = 41,
    BG_GREEN = 42,
    BG_BLUE = 44,
    BG_DEFAULT = 49,
    BOLD = 1,
    END = 0,
  };

  friend std::ostream &operator<<(std::ostream &os,
                                  const OstreamModifier::Code &code) {
    os << "\033["
       << static_cast<std::underlying_type<OstreamModifier::Code>::type>(code)
       << "m";
    return os;
  }
};

} // namespace wasmabi

#endif // WASMABI_OSTREAM_MODIFIERS_HPP
