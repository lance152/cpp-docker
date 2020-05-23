#include <sys/wait.h> //waitpid
#include <sys/mount.h> //mount
#include <fcntl.h> //open
#include <unistd.h> //execv,sethostname,chroot,fchdir
#include <sched.h> //clone

#include <cstring> //C标准库

#include <string> //C++标准库

#define STACK_SIZE (512*512) //定义子进程空间大小

namespace docker{
  typedef int proc_stat;
  proc_stat proc_err = -1;
  proc_stat proc_exit = 0;
  proc_stat proc_wait = 1;

  //docker容器配置
  struct container_config{
    std::string host_name; //主机名
    std::string root_dir; //根目录
  };

  class container{
  private:
    //增强可读性
    typedef int process_pid;

    //子进程栈
    char child_stack[STACK_SIZE];

    //容器配置
    container_config config;

    //docker run -it ubuntu:14.04 /bin/bash 可以让一个容器保持运行状态
    //我们在这里定义一个start_bash函数来执行这个命令
    void start_bash(){
      //将C++ std：：string安全的转换为C风格的字符串
      std::string bash = "/bin/bash";
      char* c_bash = new char[bash.length()+1];//+1 用于存放 ‘\0’
      strcpy(c_bash,bash.c_str());

      char* const child_args[] = {c_bash,NULL};
      execv(child_args[0],child_args);
      delete []c_bash;
    }

  public:
    //构造函数
    container(container_config &config){
      this->config = config;
    }

    void start(){
      auto setup = [](void *args)->int{
        auto _this = reinterpret_cast<container*>(args);

        //调用start_bash
        _this->start_bash();

        return proc_wait;
      };

      process_pid child_pid = clone(setup,child_stack+STACK_SIZE,SIGCHLD,this);
      //首先child_stack+STACK_SIZE会移动到栈底，SIGCHLD会在子进程退出时发出信号给父进程
      //这里传入了this指针是为了让etup函数获得contaier对象
      //这里setup函数未lambda函数，C++中捕获列表为空的lambda函数可以作为函数指针传递
      waitpid(child_pid,NULL,0); //等待子进程退出
    }
  }
}