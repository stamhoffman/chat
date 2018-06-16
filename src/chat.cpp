#include "config.h"

void *read_socket_data(void *param);
void *write_socket_data(void *param);

int main(int argc, char const *argv[]) try {

  int dsock;
  pthread_t read_pth;
  pthread_attr_t r_attr_pth;
  pthread_attr_init(&r_attr_pth);

  pthread_t write_pth;
  pthread_attr_t w_attr_pth;
  pthread_attr_init(&w_attr_pth);

  //start thread read
  pthread_create(&read_pth, &r_attr_pth, read_socket_data, (void *)&dsock);
  //start thread wride
  pthread_create(&write_pth, &w_attr_pth, write_socket_data, (void *)&dsock);

  pthread_join(read_pth, NULL);
  pthread_join(write_pth,NULL);

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

  if((dsock = socket(AF_INET, SOCK_DGRAM, 0))== -1) throw "Error open socket";
  if(bind(dsock, (struct sockaddr *)&in_addr, sizeof(in_addr))== -1) throw "Error bind socket";

  struct sockaddr addr;
  socklen_t saddr = sizeof(addr);
  std::string str;

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
  std::cout << "start write" << '\n';
  int dsock;
  struct sockaddr_in in_addr;
  std::string str;

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(1, 100);
  int port = dis(gen) + PORT;
  std::cout << "write:" << port <<'\n';

  in_addr.sin_family = AF_INET;
  in_addr.sin_port = htons(port);
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
