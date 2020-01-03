## Smart pointer parameters

#### [整理自]( https://herbsutter.com/2013/06/05/gotw-91-solution-smart-pointer-parameters/ )

>  *avoid copying it unless you really mean to add a new reference* 

考虑如下使用

```C++
void func(std::shared_ptr<Foo>);
```

​	首先需要意识到的是，传入的指针生命周期为这个函数，需要的执行拷贝构造和析构都涉及引用计数(共享区域)的写。

>  the called function’s lifetime is a strict subset of the calling function’s call expression 

​	如果这个函数有意保留一个指针，使用值传递是可以的，否则应该考虑使用引用，基于上面这个解释，不需要担心shared_ptr指向无效对象。

​	更多的时候，考虑引用。

```C++
void func(const std::shared_ptr<Foo>&);
void func(std::shared_ptr<Foo>&);
```



>  *Express a “sink” function using a by-value* **unique_ptr** *parameter.* 

```C++
void func(std::unique_ptr<Foo>);
```

​	当使用如上的方式的时候，语义是这个对象将被“交付”给这个函数。

#### pair

```C++
// FIXME: use unique_ptr<Timer> instead of raw pointers.
// This requires heterogeneous comparison lookup (N3465) from C++14
// so that we can find an T* in a set<unique_ptr<T>>.
typedef std::pair<Timestamp, Timer*> Entry;

// pair在构造的时候对输入对象执行拷贝构造，所以unique_ptr不能放入pair中
```

