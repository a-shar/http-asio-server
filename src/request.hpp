#ifndef HTTP_SERVER3_REQUEST_HPP
#define HTTP_SERVER3_REQUEST_HPP

#include <string>
#include <vector>
#include "header.hpp"

namespace http {

    /// A request received from a client.
    struct request {
        std::string method;
        std::string uri;
        int http_version_major;
        int http_version_minor;
        std::string resource(void) const
        {
            if (!cached_resource_.empty())
                return cached_resource_;
            size_t pos = uri.find_first_of('?');

            cached_resource_ = uri.substr(0, pos);

            return cached_resource_;
        }
        std::vector<header> headers;

    private:
        mutable std::string cached_resource_;
    };

} // namespace http

#endif // HTTP_SERVER3_REQUEST_HPP
