#ifndef HTTP_SERVER3_MIME_TYPES_HPP
#define HTTP_SERVER3_MIME_TYPES_HPP

#include <string>

namespace http {
namespace mime_types {

    /// Convert a file extension into a MIME type.
    std::string extension_to_type(const std::string& extension);

} // namespace mime_types
} // namespace http

#endif // HTTP_SERVER3_MIME_TYPES_HPP
