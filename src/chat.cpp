#include "config.h"

// Что еще нужно сделать
// Использование mutex
// Возврат значений из потока

void *read_socket_data(void *param);
void *write_socket_data(void *param);
void *server_chat(void *param);

int main(int argc, char const *argv[]) try {

  int dsock;
  pthread_t read_pth;
  pthread_attr_t r_attr_pth;
  pthread_attr_init(&r_attr_pth);

  pthread_t write_pth;
  pthread_attr_t w_attr_pth;
  pthread_attr_init(&w_attr_pth);

  pthread_t server_pth;
  pthread_attr_t server_attr_pth;
  pthread_attr_init(&server_attr_pth);

  //start server
  pthread_create(&server_pth, &server_attr_pth, server_chat, (void *)&dsock);
  //start thread read
  pthread_create(&read_pth, &r_attr_pth, read_socket_data, (void *)&dsock);
  //start thread wride
  pthread_create(&write_pth, &w_attr_pth, write_socket_data, (void *)&dsock);

  pthread_join(read_pth, NULL);
  pthread_join(write_pth,NULL);
  pthread_join(server_pth,NULL);

} catch(const char *str)
  {
    std::cout << str << '\n';
    return -1;
  }
  catch(...)
  {
    return -1;
  }

void *read_socket_data(void *param) try
{
  std::cout << "start read" << '\n';
  int dsock;

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(1, 100);
  int port = dis(gen) + PORT;
  std::cout << "read:" << port <<'\n';

  struct sockaddr_in in_addr;
  in_addr.sin_family = AF_INET;
  in_addr.sin_port = htons(port);
  inet_pton(AF_INET, IP, &in_addr.sin_addr);

  int out_sock;
  struct sockaddr_in out_addr;
  out_addr.sin_family = AF_INET;
  out_addr.sin_port = htons(PORT);
  out_addr.sin_addr.s_addr = htonl(INADDR_ANY);

  if((out_sock = socket(AF_INET, SOCK_DGRAM, 0))== -1) throw "Error open out_socket";
  std::string str;
  str.erase();
  str = "my_port";
  sendto(out_sock, str.data(), str.size(), 0, (sockaddr *)&out_addr, sizeof(out_addr));
  str.erase();
  str = std::to_string(port);
  sendto(out_sock, str.data(), str.size(), 0, (sockaddr *)&out_addr, sizeof(out_addr));
  str.erase();

  if((dsock = socket(AF_INET, SOCK_DGRAM, 0))== -1) throw "Error open socket";
  if(bind(dsock, (struct sockaddr *)&in_addr, sizeof(in_addr))== -1) throw "Error bind socket";

  struct sockaddr addr;
  socklen_t saddr = sizeof(addr);

  while(1)
  {
    if(recvfrom(dsock, &str, sizeof(str), 0, &addr, &saddr) > 0)
    {
      std::cout << str << '\n';
    }
    if(str == "exit")
    {
      pthread_exit(0);
    }
  }
} catch(const char *str)
{
  std::cout << str << '\n';
  pthread_exit(0);
}


void *write_socket_data(void *param) try
{
  int dsock;
  struct sockaddr_in in_addr;
  std::string str;

  in_addr.sin_family = AF_INET;
  in_addr.sin_port = htons(PORT);
  inet_pton(AF_INET, IP, &in_addr.sin_addr);

  if((dsock = socket(AF_INET, SOCK_DGRAM, 0))== -1) throw "Error open socket";

  while(1)
  {
    std::cin >> str;
    if(str == "exit")
    {
      pthread_exit(0);
    }
    sendto(dsock, str.data(), str.size(), 0, (sockaddr *)&in_addr, sizeof(in_addr));
  }

} catch(const char *str)
{
  std::cout << str << '\n';
  pthread_exit(0);
}

void *server_chat(void *param) try
{
  int in_dsock;
  int out_dsock;
  struct sockaddr_in in_addr;
  struct sockaddr_in out_addr;
  std::string str;

  out_addr.sin_port = htons(PORT);
  out_addr.sin_family = AF_INET;
  out_addr.sin_addr.s_addr = htonl(INADDR_ANY);

  in_addr.sin_family = AF_INET;
  in_addr.sin_port = htons(PORT);
  in_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  socklen_t size_in = sizeof(in_addr);

  if((in_dsock = socket(AF_INET, SOCK_DGRAM, 0))== -1) throw "Error open in_socket";
  if(bind(in_dsock, (struct sockaddr *)&in_addr, sizeof(in_addr))== -1) throw "Error bind socket";

  if((out_dsock = socket(AF_INET, SOCK_DGRAM, 0))== -1) throw "Error open out_socket";

  std::vector<int> d_sockets;

  while(1)
  {
    str.erase();
    auto ret = recvfrom(in_dsock, &str, str.size(), 0, (struct sockaddr *)&in_addr, &size_in);
    if(str == "my_port")
    {
      while(1)
      {
        str.erase();
        auto ret = recvfrom(in_dsock, &str, str.size(), 0, (struct sockaddr *)&in_addr, &size_in);
        if(ret > 0)
        {
          d_sockets.push_back(std::stoi(str));
          std::cout << "port push server = " << str << '\n';
        }
      }
    }
    if(ret > 0)
    {
      for(auto unit: d_sockets)
      {
        out_addr.sin_port = htons(unit);
        sendto(out_dsock, &str, str.size(), 0, (struct sockaddr*)&out_addr, sizeof(out_addr));
      }
    }
  }


} catch(...)
  {
    pthread_exit(0);
  }
