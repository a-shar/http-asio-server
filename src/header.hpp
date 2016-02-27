#ifndef HTTP_SERVER3_HEADER_HPP
#define HTTP_SERVER3_HEADER_HPP

#include <string>

namespace http {

struct header
{
  std::string name;
  std::string value;
};

} // namespace http

#endif // HTTP_SERVER3_HEADER_HPP
