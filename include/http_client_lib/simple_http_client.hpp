// Copyright 2020 Petr Portnov me@progrm-jarvis.ru

#ifndef INCLUDE_SIMPLE_HTTP_CLIENT_HPP_
#define INCLUDE_SIMPLE_HTTP_CLIENT_HPP_

#include <http_client_lib/definitions.hpp>
#include <string>

namespace http_client_lib {

    /**
     * @brief Simple implementation of an HTTP-client
     */
    class SimpleHttpClient final {
    public:
        /// Type of common exception which may be thrown.
        using exception_t = boost::system::system_error;
        /// Request type.
        using request_t = http::request<http::string_body>;
        /// Response type.
        using response_t = http::response<http::dynamic_body>;

    public:
        /**
         * @brief Creates a new simple HTTP-client from the given host and port strings.
         *
         * @param host target host
         * @param port target port
         * @return created HTTP-server
         *
         * @throw {@refitem exception_t} if an error occurs while creating as server
         */
        static SimpleHttpClient from(::std::string const& host, ::std::string const& port);

        /**
         * @brief Performs an HTTP-request to this client's target.
         * @param request request to send to the server
         * @return response to the given request
         *
         * @throw {@refitem exception_t} if an error occurs while performing the request
         */
        response_t request(::std::string&& request);

        /**
         * @brief Safely shuts this HTTP-client down.
         *
         * @throw {@refitem exception_t} if an error occurs while shutting down
         */
        ~SimpleHttpClient();

    private:
        asio::io_context context_;
        tcp::resolver::results_type endpoints_;
        ::std::string host_;

        SimpleHttpClient(::std::string&& host, ::std::string&& port);
    };
} // namespace http_client_lib

#endif // INCLUDE_SIMPLE_HTTP_CLIENT_HPP_
