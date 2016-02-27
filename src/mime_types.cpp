#include "mime_types.hpp"
#include <unordered_map>

namespace http {
namespace mime_types {

typedef std::unordered_map<std::string, std::string> mime_type_map;

const static mime_type_map mappings = {
    { "gif", "image/gif" },
    { "htm", "text/html" },
    { "html", "text/html" },
    { "jpg", "image/jpeg" },
    { "png", "image/png" },
    { "js", "application/javascript" } };

    std::string extension_to_type(const std::string& extension)
    {
        if(mappings.count(extension) == 0)
            return "text/plain";
        else
            return mappings.at(extension);
    }

} // namespace mime_types
} // namespace http
