#include <boost/asio.hpp>
#include <boost/log/trivial.hpp>
#include <boost/thread.hpp>
#include <chrono>
#include <iostream>
#include <string>

using boost::asio::buffer;
using boost::asio::ip::tcp;

boost::asio::io_service service;

class talk_to_srv {
  tcp::socket sock;
  int already_read;
  char buff[1024];
  bool started;
  std::string name;

 public:
  talk_to_srv(const std::string& username)
      : sock(service), started(true), name(username) {}
  talk_to_srv() : sock(service), started(true) {}

  void connect(tcp::endpoint endp) { sock.connect(endp); }

  void write_to_srv(const std::string& msg) { sock.write_some(buffer(msg)); }

  std::string username() { return name; }

  void write_request() { write_to_srv("ping/n"); }

  size_t read_complete(const boost::system::error_code& err, size_t bytes) {
    if (err) return 0;
    bool find = (std::find(buff, buff + bytes, '\n') < buff + bytes);
    if (find)
      return 0;
    else
      return 1;
  }
  void process_msg() {
    std::string msg(buff, already_read);
    if (msg.find("login ") == 0)
      on_login();
    else if (msg.find("ping") == 0)
      on_ping(msg);
    else if (msg.find("clients ") == 0)
      on_clients(msg);
    else
      std::cerr << "invalid message " << msg << std::endl;
  }

  void read_answer() {
    already_read = 0;
    read(sock, buffer(buff),
         boost::bind(&talk_to_srv::read_complete, this, _1, _2));
    process_msg();
  }

  void do_ask_clients() {
    write_to_srv("ask_clients\n");
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
    std::cout << name << ", new client list: " << clients;
  }

  void loop() {
    write_to_srv("login " + name + "\n");
    read_answer();
    while (started) {
      write_request();
      read_answer();
      boost::this_thread::sleep(boost::posix_time::millisec(rand() % 7000));
    }
  }
};

static talk_to_srv client;

void start_client() {
  std::cout << "Enter username " << std::endl;
  std::string username;
  std::cin >> username;
  tcp::endpoint ep(boost::asio::ip::address::from_string("127.0.0.1"), 8001);
  client.connect(ep);
  client.write_to_srv(username);

  while (true) {
    std::string cmd;
    std::cout << "Enter request" << std::endl;
    std::cin >> cmd;
    client.write_to_srv(cmd);
  }
}
int main() {
  boost::thread_group pool_thr;
  pool_thr.create_thread(start_client);
  pool_thr.join_all();
  return 0;
}
