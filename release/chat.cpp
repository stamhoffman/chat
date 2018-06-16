#include "config.h"

void *read_socket_data(void *param);
void *write_socket_data(void *param);

int main(int argc, char const *argv[]) try {

  int dsock;
  struct sockaddr_in in_addr;
  in_addr.sin_family = AF_INET;
  in_addr.sin_port = htons(PORT);
  inet_pton(AF_INET, IP, &in_addr.sin_addr);

  if((dsock = socket(AF_INET, SOCK_DGRAM, 0))== -1) throw "Error open socket";
  if(bind(dsock, (struct sockaddr *)&in_addr, sizeof(in_addr))== -1) throw "Error bind socket";

  pthread_t read_pth;
  pthread_attr_t r_attr_pth;
  pthread_attr_init(&r_attr_pth);

  pthread_t read_pth_send;
  pthread_attr_t w_attr_pth;
  pthread_attr_init(&w_attr_pth);

  //start thread read
  pthread_create(&read_pth, &r_attr_pth, read_socket_data, (void *)&dsock);
  pthread_create(&read_pth_send, &w_attr_pth, write_socket_data, (void *)&dsock);

} catch(const char *str)
  {
    std::cout << str << '\n';
    return -1;
  }
  catch(...)
  {
    return -1;
  }

void *read_socket_data(void *param)
{
  struct sockaddr addr;
  socklen_t saddr = sizeof(addr);
  std::string str;

  while(1)
  {
    if(recvfrom(*((int *)param), &str, sizeof(str), 0, &addr, &saddr) > 0)
    {
      std::cout << str << '\n';
    }
    if(str == "exit")
    {
      pthread_exit(0);
    }
  }
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
    sendto(dsock, str.data(), str.size(), 0, (sockaddr *)&in_addr, sizeof(in_addr));
  }

} catch(const char *str)
{
  std::cout << str << '\n';
  pthread_exit(0);
}
