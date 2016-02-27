#include <iostream>
#include <string>

#include <unistd.h>

#include "server.hpp"

/**
Как-то много споров возникло про контент ответа у коллег. Вот достаточная форма:

static const char* templ = "HTTP/1.0 200 OK\r\n"

                   "Content-length: %d\r\n"

                   "Connection: close\r\n"

                   "Content-Type: text/html\r\n"

                   "\r\n"

                   "%s";

static const char not_found[] = "HTTP/1.0 404 NOT FOUND\r\nContent-Type: text/html\r\n\r\n";
Запросы посылаются только в виде GET с путем, так что всякие cgi-параметры надо отрезать (просто замените '?' на '\0' в пути).

Не понимаю зачем вы делаете какие-то специальные html-файлы :)
*/

const static std::size_t DEFAULT_THREAD_COUNT = 4;

void display_usage(void)
{
    std::cout << "usage:\n"
         << "\t -h host\n\t -p port\n\t -d directory" << std::endl;
}

int main(int argc, char* argv[])
{
    int opt;
    char optString[] = "h:p:d:";
    std::string host;
    std::string dir;
    std::string port;
    opt = getopt(argc, argv, optString);
    while (opt != -1) {
        switch (opt) {
        case 'h':
            host = optarg;
            break;

        case 'p':
            port = optarg;
            break;

        case 'd':
            dir = optarg;
            break;

        case '?':
            display_usage();
            break;

        default:
            break;
        }

        opt = getopt(argc, argv, optString);
    }

    if(daemon(0, 0) != 0) {
        perror("daemon");
        return -1;
    }

    try {

        http::server s(host, port, dir, DEFAULT_THREAD_COUNT);

        // Run the server until stopped.
        s.run();
    }
    catch (std::exception& e) {
        std::cerr << "exception: " << e.what() << "\n";
    }

    return 0;
}
