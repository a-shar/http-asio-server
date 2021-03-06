//
// server.hpp
// ~~~~~~~~~~
#ifndef HTTP_SERVER3_SERVER_HPP
#define HTTP_SERVER3_SERVER_HPP

#include <boost/asio.hpp>
#include <string>
#include <vector>
#include <boost/noncopyable.hpp>
#include "connection.hpp"
#include "request_handler.hpp"

namespace bao = boost::asio;

namespace http {

class server
    : private boost::noncopyable {
public:
    /// Construct the server to listen on the specified TCP address and port, and
    /// serve up files from the given directory.
    explicit server(const std::string& address, const std::string& port,
        const std::string& doc_root, std::size_t thread_pool_size);

    /// Run the server's io_service loop.
    void run();

private:
    /// Initiate an asynchronous accept operation.
    void start_accept();

    /// Handle completion of an asynchronous accept operation.
    void handle_accept(const boost::system::error_code& e);

    /// Handle a request to stop the server.
    void handle_stop();

    /// The number of threads that will call io_service::run().
    std::size_t thread_pool_size_;

    /// The io_service used to perform asynchronous operations.
    bao::io_service io_service_;

    /// The signal_set is used to register for process termination notifications.
    bao::signal_set signals_;

    /// Acceptor used to listen for incoming connections.
    bao::ip::tcp::acceptor acceptor_;

    /// The next connection to be accepted.
    connection::ptr new_connection_;

    /// The handler for all incoming requests.
    request_handler request_handler_;
};

} // namespace http
#endif // HTTP_SERVER3_SERVER_HPP
