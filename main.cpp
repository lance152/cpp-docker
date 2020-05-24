#include "docker.hpp"
#include <iostream>

int main(int argc, char** argv){
  std::cout<<"...start container"<<std::endl;
  docker::container_config config;

  //配置容器
  config.host_name = "lfrz";
  config.root_dir = "./dock";

  //配置网络参数
  config.ip = "192.168.0.100"; // 容器ip
  config.bridge_name = "docker0"; //宿主机网桥名
  config.bridge_ip = "192.168.0.1"; //宿主机网桥ip

  docker::container container(config);//构造容器
  container.start();
  std::cout<<"stop container..."<<std::endl;
  return 0;
}
