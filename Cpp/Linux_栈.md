[原文](https://blog.csdn.net/yangkuanqaz85988/article/details/52403726)

### 任务

​	在汇编中已经讲过函数调用的过程，OS利用硬件提供的栈实现，Linux下函数调用使用的栈进行参数传递，对于部分可以放到寄存器的参数也会用寄存器传递。8086中用stack传递参数的时候会使用到BP，同样的，在更高级的cpu中，由于架构相同，其实现方式类似，可以看下图，ESP指示栈顶，EBP用于栈内元素的访问。

![函数调用栈的典型内存布局](Linux_栈.assets/20160901214853559)

函数的嵌套调用意味着stack中函数栈帧的堆积，同时也能看到，一个函数(任务)可以表示为{函数代码，stack信息，寄存器信息}，这也是OS能够实现多任务的基础，即只要保存和恢复这些信息，就能够实现任务的切换。

>  任务是一个抽象的概念，即指软件完成的一个活动；而线程则是完成任务所需的动作；进程则指的是完成此动作所需资源的统称 

#### 进程栈

​	经常看到进程栈的图示，但是都没有记录下来

- 程序段 (Text Segment)：可执行文件代码的内存映射

- 数据段 (Data Segment)：可执行文件的已初始化全局变量的内存映射
- BSS段 (BSS Segment)：未初始化的全局变量或者静态变量（用零页初始化）
- 堆区 (Heap) : 存储动态内存分配，匿名的内存映射
- 栈区 (Stack) : 进程用户空间栈，由编译器自动分配释放，存放函数的参数值、局部变量的值等
- 映射段(Memory Mapping Segment)：任何内存映射文件

<img src="Linux_栈.assets/20160901214948512" alt="Linux标准进程内存段布局" style="zoom:80%;" />

原文在这里利用栈溢出来测试进程栈的大小，约为8M。

Linux内核使用**内存描述符**(mm_struct)表示进程的地址空间

```c
struct mm_struct {
    struct vm_area_struct *mmap;           /* 内存区域链表 */
    struct rb_root mm_rb;                  /* VMA 形成的红黑树 */
    ...
    struct list_head mmlist;               /* 所有 mm_struct 形成的链表 */
    ...
    unsigned long total_vm;                /* 全部页面数目 */
    unsigned long locked_vm;               /* 上锁的页面数据 */
    unsigned long pinned_vm;               /* Refcount permanently increased */
    unsigned long shared_vm;               /* 共享页面数目 Shared pages (files) */
    unsigned long exec_vm;                 /* 可执行页面数目 VM_EXEC & ~VM_WRITE */
    unsigned long stack_vm;                /* 栈区页面数目 VM_GROWSUP/DOWN */
    unsigned long def_flags;
    unsigned long start_code, end_code, start_data, end_data;    /* 代码段、数据段 起始地址和结束地址 */
    unsigned long start_brk, brk, start_stack;                   /* 栈区 的起始地址，堆区 起始地址和结束地址 */
    unsigned long arg_start, arg_end, env_start, env_end;        /* 命令行参数 和 环境变量的 起始地址和结束地址 */
    ...
    /* Architecture-specific MM context */
    mm_context_t context;                  /* 体系结构特殊数据 */

    /* Must use atomic bitops to access the bits */
    unsigned long flags;                   /* 状态标志位 */
    ...
    /* Coredumping and NUMA and HugePage 相关结构体 */
};
```

