#include "server.hpp"
#include <boost/bind.hpp>
#include <memory>
#include <vector>
#include <thread>

namespace bao = boost::asio;

namespace http {

server::server(const std::string& address, const std::string& port,
    const std::string& doc_root, std::size_t thread_pool_size)
    : thread_pool_size_(thread_pool_size)
    , signals_(io_service_)
    , acceptor_(io_service_)
    , new_connection_()
    , request_handler_(doc_root)
{
    // Register to handle the signals that indicate when the server should exit.
    // It is safe to register for the same signal multiple times in a program,
    // provided all registration for the specified signal is made through Asio.
    signals_.add(SIGINT);
    signals_.add(SIGTERM);
#if defined(SIGQUIT)
    signals_.add(SIGQUIT);
#endif // defined(SIGQUIT)
    signals_.async_wait(boost::bind(&server::handle_stop, this));

    // Open the acceptor with the option to reuse the address (i.e. SO_REUSEADDR).
    bao::ip::tcp::resolver resolver(io_service_);
    bao::ip::tcp::resolver::query query(address, port);
    bao::ip::tcp::endpoint endpoint = *resolver.resolve(query);
    acceptor_.open(endpoint.protocol());
    acceptor_.set_option(bao::ip::tcp::acceptor::reuse_address(true));
    acceptor_.bind(endpoint);
    acceptor_.listen();

    start_accept();
}

void server::run()
{
    // Create a pool of threads to run all of the io_services.
    std::vector<std::shared_ptr<std::thread> > threads;
    for (std::size_t i = 0; i < thread_pool_size_; ++i) {
        std::shared_ptr<std::thread> thread(new std::thread(
            boost::bind(&bao::io_service::run, &io_service_)));
        threads.push_back(thread);
    }

    // Wait for all threads in the pool to exit.
    for (std::size_t i = 0; i < threads.size(); ++i)
        threads[i]->join();
}

void server::start_accept()
{
    new_connection_.reset(new http::connection(io_service_, request_handler_));
    acceptor_.async_accept(new_connection_->socket(),
        boost::bind(&server::handle_accept, this,
                               bao::placeholders::error));
}

void server::handle_accept(const boost::system::error_code& e)
{
    if (!e) {
        new_connection_->start();
    }

    start_accept();
}

void server::handle_stop()
{
    io_service_.stop();
}

} //namespace http
