// Copyright 2020 Petr Portnov me@progrm-jarvis.ru

#include <boost/beast/version.hpp>
#include <http_client_lib/simple_http_client.hpp>

namespace http_client_lib {
    SimpleHttpClient::SimpleHttpClient(::std::string&& host, ::std::string&& port)
        : context_(), endpoints_(tcp::resolver{context_}.resolve(host, port)), host_(host) {}

    SimpleHttpClient SimpleHttpClient::from(::std::string const& host, ::std::string const& port) {
        return SimpleHttpClient{::std::string(host), ::std::string(port)};
    }

    SimpleHttpClient::response_t SimpleHttpClient::request(::std::string&& request) {
        beast::tcp_stream stream(context_);
        stream.connect(endpoints_);

        request_t http_request{http::verb::post, request, 11 /* HTTP 1.1 */};
        http_request.set(http::field::host, host_);
        http_request.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
        http::write(stream, http_request);

        beast::flat_buffer buffer;
        response_t response;
        http::read(stream, buffer, response);

        return response;
    }

    SimpleHttpClient::~SimpleHttpClient() {}
} // namespace http_client_lib
