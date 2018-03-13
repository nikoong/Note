#在ubuntu下，亲手做一个 tiny shell
　　上一篇博客我们讲到了异常控制流。从底层来看，异常就是程序计数器序列的突变。正是有了异常控制流机制的存在，我们的计算机才能实现进程之间的切换。进程之间可以相互通过**信号**进行通信工程，信号处理程序和主程序并发运行，他们共享全局变量。其中还可能遇到所谓**竞争**的经典问题。
　　使用linux的人都知道，每当我们打开终端，就自动进入了shell程序。shell有很多内置的命令，更可以直接运行可执行文件。我们大多知道shell中命令的含义，但是对shell的实现原理不甚了解。
　　纸上得来终觉浅，绝知此事要躬行。正好我们学习到了异常控制流，我们就有足够的知识来做一个简单的shell程序了。这篇博客主要实现CSAPP3e的shell lab。实验给出了我们整体框架和一些方便的函数。所以我们的任务就聚焦到shell的关键部分：信号处理和前后台任务切换。


####第零步：了解相关函数

```cpp
/*******************进程控制函数*********************/

//---- 创建子进程&获取进程ID ----
pid_t getpid(void); //返回进程的PID
pid_t getppid(void); //返回父进程的PID
pid_t fork(void);  //创建子进程，子进程返回0，父进程返回子进程的PID，如果出错，则返回-1；

//---- 回收子进程 ----
pid_t waitpid(pid_t pid, int *statusp, int options);  
//默认情况options=0，waitpid挂起调用进程，直到等待集合中有进程终止才返回，如果成功返回值为0。options还有3个选项，主要区别在于什么时候返回。

pid_t wait(int *statusp);  //wait(&status)等价于调用waitpid(-1,&status,0);

//---- 加载并运行程序 ----
int execve(const char *filename, const char *argv[], const char*envp[]); //在当前的上下文中加载并运行一个新程序，如果成功，则不返回，如果错误，则返回-1；





/*******************信号相关函数*********************/

//---- 发送信号 ----
int kill(pid_t pid, int sig)//向pid进程，发送号码为sig的信号

//---- 接收信号 ----
#include <signal.h>
typedef void (*sighandler_t) (int);
sighandler_t signal(int signum, sighandler_t handler);
// sighandler_t是自定义的函数指针类型。重要的事signal函数可以接受signum信号的默认行为，变成执行信号处理函数handler。
// 一部分工作就是对不同的信号，定义不同的信号处理函数。

//---- 阻塞信号 ----
//屏蔽全部信号
#include <signal.h>
sigset_t mask_all, prev_all;
sigfillset(&mask_all);
sigprocmask(SIG_BLOCK, &mask_all, &prev_all);
/*在这里做一些涉及到全局变量读写的操作*/
sigprocmask(SIG_SETMASK, &prev_all, NULL); //解除屏蔽。

//屏蔽单一信号
sigset_t mask_one, prev_all;
sigaddset(&mask_one , int signum);
sigprocmask(SIG_BLOCK, &mask_all, &prev_all);
/*在这里做一些涉及到全局变量读写的操作*/
sigprocmask(SIG_SETMASK, &prev_all, NULL); //解除屏蔽。

```
到此为止，需要了解的函数都过了一遍。其实函数真的很简单，但要写出扎实老练的多进程代码，不是一件容易的事。
####第一步：确定功能
- 每次输出“tsh> ”
- 命令行中如果输入的是内置命令，立即执行；否则当做可执行文件的路径，在当前上下文中开启一个子进程执行。
- 如果命令行中最后一个参数是 &，则该命令在后台执行，否则在前台执行。shell必须等前台任务执行完，再输出tsh>。
- ctrl-c(ctrl-z)默认产生SIGINT（SIGTSTP）到前台的进程。
- 我们的tiny shell需要支持基本的内置命令：
  - quit：结束tiny shell
  - jobs：打印多有的后台进程
  - bg <job>: 通过发送一个SIGCONT信号重启<job>，然后在后台运行。<job>可以是pid也可以是jid。
  - fg <job>: 通过发送一个SIGCONT信号重启<job>，然后在台运行。<job>可以是pid也可以是jid。

未完待续，持续更新。




