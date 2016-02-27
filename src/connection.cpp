#include "connection.hpp"
#include <vector>
#include <boost/bind.hpp>
#include "request_handler.hpp"
#include <iostream>
#include <thread>

namespace bao = boost::asio;

namespace http {

connection::connection(bao::io_service& io_service,
    request_handler& handler)
    : strand_(io_service)
    , socket_(io_service)
    , request_handler_(handler)
{
}

bao::ip::tcp::socket& connection::socket()
{
    return socket_;
}

void connection::start()
{
    //std::cout << "connection::start with pid " << std::this_thread::get_id() << "\n";
    socket_.async_read_some(bao::buffer(buffer_),
        strand_.wrap(
            boost::bind(&connection::handle_read, shared_from_this(),
                bao::placeholders::error,
                bao::placeholders::bytes_transferred)));
}

void connection::handle_read(const boost::system::error_code& e, std::size_t bytes_transferred)
{
    if (!e) {
        boost::tribool result = request_parser_.parse(request_, buffer_.data(), buffer_.data() + bytes_transferred);

        if (result) {
            request_handler_.handle_request(request_, reply_);
            bao::async_write(socket_, reply_.to_buffers(),
                strand_.wrap(
                    boost::bind(&connection::handle_write, shared_from_this(),
                        bao::placeholders::error)));
        }
        else if (!result) {
            reply_ = reply::stock_reply(reply::bad_request);
            bao::async_write(socket_, reply_.to_buffers(),
                strand_.wrap(
                    boost::bind(&connection::handle_write, shared_from_this(),
                        bao::placeholders::error)));
        }
        else {
            socket_.async_read_some(bao::buffer(buffer_),
                strand_.wrap(
                    boost::bind(&connection::handle_read, shared_from_this(),
                        bao::placeholders::error,
                        bao::placeholders::bytes_transferred)));
        }
    }

    // If an error occurs then no new asynchronous operations are started. This
    // means that all shared_ptr references to the connection object will
    // disappear and the object will be destroyed automatically after this
    // handler returns. The connection class's destructor closes the socket.
}

void connection::handle_write(const boost::system::error_code& e)
{
    if (!e) {
        // Initiate graceful connection closure.
        boost::system::error_code ignored_ec;
        socket_.shutdown(bao::ip::tcp::socket::shutdown_both, ignored_ec);
    }

    // No new asynchronous operations are started. This means that all shared_ptr
    // references to the connection object will disappear and the object will be
    // destroyed automatically after this handler returns. The connection class's
    // destructor closes the socket.
}

} // namespace http
