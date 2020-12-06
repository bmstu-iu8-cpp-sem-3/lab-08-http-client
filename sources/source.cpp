// Copyright 2020 Merzlov Nikolay merzlovnik@mail.ru

#include <header.hpp>

using boost::asio::ip::tcp;
using boost::asio::buffer;

boost::asio::io_service service;

class talk_to_svr {
  tcp::socket sock_;
  enum { max_msg = 1024 };
  int already_read_;
  char buff_[max_msg];
  bool started_;
  std::string username_;

 public:
  talk_to_svr(const std::string& username)
      : sock_(service), started_(true), username_(username) {}

  void connect(tcp::endpoint ep) { sock_.connect(ep); }

  void write(const std::string& msg) { sock_.write_some(buffer(msg)); }

  size_t read_complete(const boost::system::error_code& err, size_t bytes) {
    if (err) return 0;
    bool found = std::find(buff_, buff_ + bytes, '\n') < buff_ + bytes;
    return found ? 0 : 1;
  }

  void do_ask_clients() {
    write("ask_clients\n");
    read_answer();
  }

  void on_login() { do_ask_clients(); }

  void on_ping(const std::string& msg) {
    std::istringstream in(msg);
    std::string answer;
    in >> answer >> answer;
    if (answer == "client_list_changed") do_ask_clients();
  }

  void on_clients(const std::string& msg) {
    std::string clients = msg.substr(8);
    std::cout << username_ << ", new client list:" << clients;
  }

  void process_msg() {
    std::string msg(buff_, already_read_);
    if (msg.find("login ") == 0)
      on_login();
    else if (msg.find("ping") == 0)
      on_ping(msg);
    else if (msg.find("clients ") == 0)
      on_clients(msg);
    else
      std::cerr << "invalid msg " << msg << std::endl;
  }

  void read_answer() {
    already_read_ = 0;
    read(sock_, buffer(buff_),
         boost::bind(&talk_to_svr::read_complete, this, _1, _2));
    process_msg();
  }

  void write_request() { write("ping\n"); }

  void loop() {
    write("login " + username_ + "\n");
    read_answer();
    while (started_) {
      write_request();
      read_answer();
      boost::this_thread::sleep(boost::posix_time::millisec(rand() % 7000));
    }
  }

  std::string username() const { return username_; }
};

int main() { return 0; }
