#include "docker.hpp"
#include <iostream>

int main(int argc, char** argv){
  std::cout<<"...start container"<<std::endl;
  docker::container_config config;

  //配置容器
  config.host_name = "lfrz";
  config.root_dir = "./shiyanlou";

  docker::container container(config);//构造容器
  container.start();
  std::cout<<"stop container..."<<std::endl;
  return 0;
}
