// Copyright 2020 Petr Portnov me@progrm-jarvis.ru

#include <http_client_lib.hpp>
#include <iostream>
#include <string>

int main(int arguments_count, char const* const arguments[]) {
    if (arguments_count < 2) {
        ::std::cout << "Endpoint was not specified";

        return 1;
    }

    try {
        auto client = http_client_lib::SimpleHttpClient::from(arguments[1], arguments_count >= 3 ? arguments[2] : "80");

        ::std::string input;
        while (true) {
            ::std::getline(::std::cin, input);
            if (input.empty()) break;

            ::std::cout << client.request("{\"input\"L " + input + "\"}") << ::std::endl;
        }
    } catch (::std::invalid_argument const& error) {
        ::std::cerr << "An exception occurred: " << error.what() << ::std::endl;
        return 1;
    }
}
