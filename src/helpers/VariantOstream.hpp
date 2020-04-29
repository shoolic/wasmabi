#ifndef WASMABI_VARIANT_OSTREAM_HPP
#define WASMABI_VARIANT_OSTREAM_HPP
#include <iostream>
#include <variant>

template <typename T, typename... Ts>
std::ostream &operator<<(std::ostream &os, const std::variant<T, Ts...> &v) {
  std::visit([&os](auto &&arg) { os << arg; }, v);
  return os;
}

#endif // WASMABI_VARIANT_OSTREAM_HPP
