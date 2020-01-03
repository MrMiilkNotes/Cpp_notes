### 对象设计

```c++
// 类设计应该思考的问题 - 条款19
//	类的语义可能是RAII类，仿函数，或者是某种type
//	三五法则对类的使用影响巨大，比如：拷贝 -- 赋值，参数传递。移动 -- 想将对象作为返回值传出
//	数据成员作为private，表明了封装性，如果暴露在外面则破坏了类本身封装性的重要特性
```

#### 对象中的const

```C++
// Primer P209, Effe P24
const T& operator<(const T& lhs, const T& rhs);
T& operator<(T& lhs, T& rhs);
// 这两个func经常一起出现，如果只有第一个func，非const版本调用后会返回一个const T&
// 因此需要重载，这时候可以考虑使用const_cast让第二个func调用第一个func

// 成员函数也是如此，当写了如下函数的时候
const M& T::func(...) const;// <- 思考是否需要const
// 思考返回类型为val, const，const &, &&
// 调用对象的const是否会影响返回值的属性？

// Effe P23
// 即使是在const T的成员函数中可能也会有想更改的“状态变量”
mutable bool valid;
// 在const T的成员函数中,不可更改成员变量不是由变量本身的const导致的,而是类本身的const
// 当成员是ref or ptr，可以使用const_cast使得成员可修改，但不应该这么做
// 而即使不用const_cast，对pointer所指的对象也可以进行修改 -> Effe P21
```

#### 析构函数和异常

```C++
// 析构函数不应该抛出异常
// 通常析构函数是返还资源，因此出现异常的概率相对构造函数会比较低
// 如果真的发生异常：
// - abort， 避免不明确行为
// - 吞下异常
```

#### 构造和析构时不用virtual function

```C++
// 当继承体系对象在逐层构造的时候，对每层的构造函数来说，其执行期间类型是正在构造的类型，因此虚函数这时候无法实现“动态绑定”，当然如果这种行为就是你想要的其实也行。
// Effe P51
// 当我们想在每个子类构造函数最后执行一个依类别而不同的操作的时候，如果操作较为类似，可能会想要把这个操作放在基类，此时可能需要“付出一定代价”，即，将来自子类的相关信息传到基类。
```

#### 用对象管理资源

```C++
// 用RAII(资源获取即初始化)手法管理资源，为资源上的操作提供API
//	设计这种类的时候，智能指针是绝佳的参考对象

// P67
// 1. 设计资源管理类的时候小心copying行为，如果是唯一性的资源，可以考虑禁止拷贝，
class A{
    A(const A&) = delete;
    A& operator=(const A&) = delete;
}
//	或者使用引用计数，或者在类内直接用智能指针管理(记得shared_ptr可以指定删除器)
// P69
// 2.有些时候可能会有些函数要求传入这些资源，这时候可以参考sharted_ptr提供get，提供类型转化或许也可行，但是多多少少算是留下隐患。
// 编写一个类的时候应当清楚其定位，如果他表现得像一个type，那标准库是绝佳的参考(一致性)

```

#### 成员函数的策略

```C++
// P99
// A.h
class A{
    void a();
    void b();
    void c();
}
// A_tools.h
void func() {	// non-member non-friend function
    a();b();c();
}
// 类及其member func应当提供“核心机制”，这些应当尽可能少，然后基于这些核心机制去
// 封装“策略”，这些再统一用一个namespace管理
// P105
// 当实现一个数值类的时候通常会默许一些类型转化，还会提供一些运算符，这些运算符应当实现为非成员函数(交换律，对等性)，但至于是否是friend则有待商榷，因为可能通过类本身提供的接口来避免访问私有数据成员从而避免其成为friend

// 一个成员一种职能
```

#### swap

​	如果标准库的swap提供的swap不满足设计的类的需求，比如RAII类，应该提供相应的swap，对于非模板类，可以在std中直接特化swap(P108)，对于模板类，可以在类所在的命名空间中写non-member版本。

​	通常来说自定义的swap是对付成员变量有诸如指针类型的类，比如pimpl类型的类，内置类型的swap是不会抛出异常的。

​	如果能提供一个高效且绝不会抛出异常的swap，就有可能基于这个swap来为类的某些操作提供较强的异常安全性

> 异常安全的承诺：
>
> - 基本承诺：异常如果被抛出，程序的任何事物都保持在有效状态下
> - 强烈保证：如果抛出异常，程序的状态不变
> - 不抛出保证：承诺绝不抛出异常，比如移动构造和移动赋值

为了保障异常安全，通常是需要付出较大代价的，比如使用swap实现“copy and swap”，即，将需要修改的对象进行拷贝，然后修改副本，然后进行交换(这个手法有时候会在拷贝函数中遇到)



#### 引用

```C++
// 应当使用C++提供的pass by reference to const 以及右值引用
// 对象的大小不是其pass by reference to const 或 pass by value的依据 P89
//	当函数返回局部对象的时候就不要想着reference了，用值传递的方式其实是可用move的

// 另外，“从对象中返回对象的成员变量或者是this(条款28,handler)”是一个可能产生危险的做法，
//	返回reference，则封装性就破坏了
//	返回const reference, iterator,传出this，则就潜在地假设了对象的生命期会比暴露的handler生命期更长
```



### 编码策略

```C++
// 和C中不太一样的，变量的构造应当尽量延后到需要为其赋值
// 不要为了表示某件事发生而改变对象，除非那件事真的发生了，即，将表示状态变化的操作留到最后。像是条件变量通常都是在状态改变(资源可用)后才进行通知
```

#### inline

​	inline的实际操作是将函数调用展开为实际代码，这也是它需要定义在头文件的原因(编译期间展开)，所以它会带来“代码膨胀”，通常也无法取得这个函数的地址，编译器也有可能在inline的时候生成一个outline版本，使得指针能正确使用，但是通过指针的调用不会被展开

> virtual的语义是“等待，直到运行期才确定”，所以virtual调用注定无法inline

​	另外，inline无法随着程序库的升级而升级。如果说inline是为了避免函数调用的开销，那inline应在在优化阶段再进行，而不是编码阶段

#### 降低编译依赖

​	编译器必须在声明一个对象的时候看到类的定义(内存分配)，或者使用对象成员的时候看到类的定义。所以，如果一个类直接定义了某些自定义的类型作为成员，则难免会在头文件中导入包含这些自定义类的定义的头文件

```C++
#include"Addr.h"

class Person {
  private:
    Addr addr_;
}
```

这时候如果自定义类的定义发生了改动，则全部包含了Person的代码都要重新编译，但如果使用指针，则只需要前置声明，（作为函数参数也只需要声明）

```C++
class Addr;

class Person {
  private:
    std::unique_ptr<Addr> addr_;
}
```

这种用法要求Person使用这些自定义类型的时候都得用指针的方式，每个自定义类都会多用去一个指针的大小，有一种做法是对整个Person引入一层间接性，即“pimp idiom”，将Person分为handler class 和 implementation class

```C++
// 考虑为声明式和定义式提供不同头文件
class PersonImpl;

class Person {
  // 成员函数不能inline
  private:
    std::unique_ptr<PersonImpl> pImpl_;
}
// 在对应的.cpp中include"PersonImpl.h"
```

除了在实现细目部分降低编译依赖，另一部分是类的拓展，比如为基类添加子类

```C++
class Person{
    virtual ~PersonInt();
    virtual void func() = 0;
    
    static std::shared_ptr<Person> create(...);
    // 定义放于.cpp中，static的工厂方法
}
```

这两种方式都是为了解除接口和实现的耦合，也都得付出一定代价。第一种方法，首先是多出一个指针，其次，每个函数的调用都必须经过handler从而增加间接性，还有就是引入了动态内存。第二种方式也差不多，一个虚函数表，每个函数访问的间接跳跃。

### 继承体系

#### is-a：public

​	public继承塑造is-a的关系，这个所谓is-a包含了“所有base class支持的操作，derived class都必须支持”，这里的"is-a"和日常经验中的可能不同，书中用bird的fly属性和企鹅举了例子，子类继承后可能存在无法实现父类某些方法的情况，这种时候虽然可用利用异常在运行期避免错误调用，但是实际上建模已经出了问题，违反了"is-a"语义。

​	public继承是多态的基础，或者说多态需要由public才能实现，private继承显然会违反“访问权限”的限制

#### has-a, is-implemented-in-terms-of

​	has-a属于应用域(application domain)

​	is-implemented-in-terms-of属于实现域(implementation domain)，比如基于vector实现stack，（而不是stack is a vector）

**private**继承

​	通常来讲，这种方式很少出现，书中说明了会使用这种方式的一个情况：“一个意欲成为derived class的类想访问一个意欲成为Base类的protected成分或为了重新定义virtual函数”。这种情况下可能会选择private继承，特别是在**设计库**的时候，可能会有空对象，如果使用private继承，基于编译器的EBO(empty base optimization)，可以使得空类不增加基类的大小。使用public继承再加复合也可以实现，但是相对private会增加一定复杂度。

#### 接口继承和实现继承

​	base中声明函数的时候应当明确各类声明方式的语义

```C++
class Base {
	void func1();				// 子类不应该进行覆盖，必须表现得和Base一致
    virtual void func2();		// 子类可以修改，也可以使用默认方式
    virtual void func3() = 0;	// 子类必须自定义
}
// ------Base.cpp---------
void Base::func2() { ... }
void Base::func3() { ... }
```

在使用纯虚函数的时候，其强调的是“子类必须自定义”，而不是“基类没有默认行为”，实际上，纯虚函数可以有定义，子类如果有需要，可以通过作用域运算符来使用，但毕竟基类给的是纯虚函数，子类在这么做的时候一定得思考清除。

```C++
class Derived: public Base {
    virtual void func3();
}
// --------Derived.cpp------------
void Derived::func3() {
    Base::func3();
}
```

#### 替代virtual

**non-virtual interface**

​	Base calss中的虚函数实现为private(protected)，使用public成员函数调用虚函数，这种做法可以在虚函数执行前后进行一些准备和清理。

**仿函数** -> Stratege模式

​	实现一个继承体系通常是比较费时费力的，有时候需求的是一些实现策略上的“动态绑定”，利用标准库的function对象和bind能满足这种需求

> 在网络编程中写回调函数的时候特别有用

**古典的Stratege**

​	实现一个策略的继承体系。

#### 多重继承

​	C++ Primer有提到了多重继承以及虚继承图象(相比普通的继承，虚继承是需要付出一定代价的)。

​	如果说没有硬性规则要求避免多重继承和private继承，再需要的时候就应当使用。

### 模板

面向对象解决问题的方式的最大特点：显式接口和运行期多态。而模板则是：**隐式接口**和**编译器多态**。隐式接口：当使用一个模板类型参数的时候，往往对这个类型有一定的要求，比如其支持的操作，或是通过隐式转换而支持的操作，编译器多态可以理解成“以不同的模板参数具现化(函数/类)模板”

> 编译器多态和运行期多态的差异有点像“确定调用哪个重载函数”和“确定绑定哪个virtual函数”

#### 模板参数推导

>  [左值和右值]( https://www.cnblogs.com/catch/p/3500678.html )
>
> 左值指的是可以取地址的变量 

``` C++
int	i = 0;
const int a = 0;

template <typename T> void func1(T& a);
// 可以传递const int,即T可以是底层const
func1(a);	// T: const int
// 从而在函数中a作为引用本身不能修改 而且底层具有const属性也不能修改
func1(0);	// error: &无法绑定到右值

template <typename T> void func2(const T& a);
// 可以传递任意类型的参数
func2(a);	// T: int

template <typename T> void func3(T&& a);
// 可以传递任意类型的参数
func3(0);	// T: int
func3(i);	// T: int&
func3(a);	// T: const int&
```

**引用叠加**

```C++
// 规则： (不能直接使用)
X& &, X& &&, X&& & -> X&
X&& && -> X&&
```

引用叠加导致模板函数的行为“诡异”

```C++
template <typename T>
void func4(T&& a) {
    T c = a;	// T 可能是值或引用 比如int 或 int&
    			// 这样对之后的行为影响会很大
    c = 10;		// 是否改变a?
}

// 实际如果真的要这样用，思考是否可以加个重载
template<typename T>
void func4(const T& a);	// 用于处理左值和const右值	前面的用于处理可修改右值
```

**move**

```C++
template <typename T>
typename remove_reference<T>::type&& move(T&& t) {
    return static_cast<typename remove_reference<T>::type&&>(T);
}
```

**转发**

```C++
// 欲将实参原封不动地转发给别的函数
// 1.
template <typename Func, typename T1, typename T2>
void flip(Func f, T1 t1, T2 t2) {
    f(t1, t2);
}
// usage
int i, j;
void f(int i, int& j);
flip(f, i, j);	// T2: int 

// 2.
template <typename Func, typename T1, typename T2>
void flip(Func f, T1&& t1, T2&& t2) {
    f(t1, t2);
}
// usage
void f(int&& i, int& j);
flip(f, 0, j);	// 0作为右值传入了t1，没问题，
// 但是在flip中t1作为参数传入f是左值，出现错误

//3.使用forward: forward<T>返回 T&&，然后依据法则进行折叠
template <typename Func, typename T1, typename T2>
void flip(Func f, T1&& t1, T2&& t2) {
    f(std::forward<T1>(t1), std::forward<T2>(t2));
}
```



#### 继承模板类

```C++
template<typename T>
class Base {
  public:
    void send();
}
template<typename T>
class Derived: public Base<T> {
    void func() {
        send();	// 虽然模板类中有这个函数，但是我们对这个模板类的特化可能没有这个函数
        // 因此编译器会拒绝在templatized base class内寻找名称
        // 但是我们可以显示告诉编译器，比如：
        // 1.
        this->send();
        // 2.
        using Base<T>::send();
        // 3.下面这种方式，如果send是virtual函数，则会关闭动态绑定(???如何理解，eg??)
        Base<T>::send();
    }
}
```

​	

#### 避免模板导致的膨胀

​	类似函数中的代码重复，通常会抽取为一个新函数，类中的代码重复通常也会抽取为新的类，模板也是一样的，不同之处在于模板只有一个，其抽取应当针对“和参数无关的部分”，从而避免生成的代码的重复。

```C++
// 一种手法，可能不算主流，但是提供了基础的思路
template<typename T>
class Base{
  public:
    void func(std::size_t);//将另一个模板参数当成函数参数
  private:
    T* data_ptr;	// 需要访问数据，因此必须有数据的handler
}
template<typename T, std::size_t n>
class Derived: private Base<T> {	// 选用private表示is...of而非is-a
  private:
    using Base<T>::func(std::size_t);
  public:
  	void func() {func(n);} 
    T[n*n] data;	// 多个指针的开销
}
```

#### 成员函数模板

​	use member function template to accept "all compatible types".

```C++
template<typename T>
class SmartPtr{
  public:
    template<typename U>
    SmartPtr(const SmartPtr<U>& other)
     : data_ptr(other.get()) {}	// 这个用法确保只有兼容类型能通过编译，即兼容的指针转化
    T* get() const { return data_ptr; }
  private:
    T* data_ptr;
}
```

泛化的拷贝构造并不会阻止编译器生成默认拷贝构造。

类模板的成员函数初始化并不是在使用具体的模板类的时候，而是**使用具体的模板成员函数的时候**，这样使得某些模板即使使用了某些类不支持的操作，但只要不使用(用了类不支持的操作的)模板的成员函数即能通过编译。

#### 隐式类型转换

​	模板实参推导从不将隐式类型转化纳入考虑，这个影响的是模板类被当成函数参数的情况。比如operator+之类的。当我们的模板类遇到这种问题，可以使用非成员的友元函数来解决

```C++
template<typename T>
class Rational {
  public:
    // 当模板类被实例化的时候友元函数也会生成(注意这里友元函数不能定义在类外部，不然这种做法就没意义了)
    // 被实例化的函数能够接收隐式类型转化，然后调用模板函数执行
    firend const Rational operator*(const Rational& lhs, const Rational& rhs) {
        doMultiply(lhs, rhs);
    }
}
// 无法进行隐式转换
template<typename T>
const Rational<T> doMultiply(const Rational<T>& lhs, const Rational& rhs) {
    return Rational(lhs.numerator()*rhs.numerator(),
                    lhs.denominator()*rhs.denominator())
}
```

> 模板类的友元不能在外部提供定义

#### traits class

​	一种利用模板表现类型信息的手法，可以理解成编译期以类型为条件的*if...else...*。

> 编译器必须确保所有源码有效，即使是一定不会执行的代码。
>
> 所以这种“表现类型信息”的方式非常有用。

STL中的迭代器类型就是个很好的例子，能和内置类型兼容。

```C++
// 这两个模板主要是为了兼容内置指针类型
template<typename T>	// 用于STL容器的迭代器类型
struct iterator_traits {
    // 要求STL容器定义iterator_category字段
    typedef typename T::iterator_category iterator_category;
}
template<typename T*>	// 偏特化 用于内置类型 即raw pointer
struct iterator_traits {
    typedef std::random_access_iterator_tag iterator_category;
}
// 会调用不同的模板函数，利用模板实现类型的if...else...
template<typename IterT, typename DistT>
void advance(IterT& iter, DistT d) {
    doAdvance(iter, d, typename std::iterator_traits<iterT>::iterator_category);
}
// 
template<typename IterT, typename DistT>
void doAdvance(IterT& iter, DistT d, std::random_access_iterator_tag){...}

template<typename IterT, typename DistT>
void doAdvance(IterT& iter, DistT d, std::input_iterator_tag){...}
....
```

### 接管new和delete

#### 1. Do it for me

​	条款49主要讲的是*new_handler*函数和设置这个函数的*set_new_handler*，但是这个*new_handler*是全局的，使用的时候应当注意，即使在单线程中，如果使用也需要注意恢复，作者的手法和*muduo*中互斥锁的管理手法类似，用RAII手法封装*set_new_handler*的使用，在析构的时候自动注册回原来的*new_handler*

​	*new_handler*是*new*中很重要的一个处理环节，其应当具有的行为：

- 让更多内存可被使用
- 安装另一个*new_handler*：第一条无法实现的时候，尝试使用其他函数
- 卸除*new_handler*：无法完成第一条，从而执行下面一条。
- 抛出*bad_alloc*
- *abort*等直接终止

```C++
class NewHandlerHolder: nocopyable {
  public:
    explicit NewHandlerHolder(std::new_handler nh): nh_(nh) {}
    ~NewHandlerHolder() { std::set_new_handler(nh_); }
  private:
    std::new_handler nh_;
}

// Usage in Class:
void* W::operator new(std::size_t sz) noexcept {
    // 类似MutexLockGuard的用法
    NewHandlerHolder(std::new_handler_holder(currentHandler));
    return ::operator new(sz);
}
// 		这里W是希望定制new_handler的类，除了operator new，他还需要有成员函数
//	能够设置currentHandler(成员变量)
// 
//		enable_shared_from_this<T>的手法：CRTP, do it for me
//	W的需求是比较普遍的，寻找复用的手法会是较为合理的需求
template <typename T>
class NewHandlerSupport {
  public:
    static std::new_handler set_new_handler(std::new_handler);
    static void* operator new(std::size_t sz);
  private:
    // 使用模板是为了让每个不同的Class会有不同的static handler
    static std::new_handler nh_;
}
// 初始化
template <typename T>
std::new_handler NewHandlerSupport<T>::nh_ = nullptr;
```

#### 2. 何时需要替换new和delete

​	自带的new相对中庸，普适各种情况，当程序确实需要在内存分配做优化的时候可以考虑定制。其位问题，为了“相关对象成簇从而降低缺页”。为定制新的行为(薄记)

#### 3. new和delete应当具有的行为

```C++
void* Base::operator new(std::size_t size) {
    if(size != sizeof(Base)) 	// 这个做法是为了避免继承的时候子类调用本函数导致的错误
        // 自动进行分配0byte的行为处理
        return ::operator new(size);	// 使用全局的new
    while(true) {	// 应当不断尝试分配
        // malloc尝试分配
        
        // 分配失败
        new_handler globalHandler = set_new_handler(nullptr);
        set_new_handler(globalHandler);
        // 处理or抛异常
        if(globalHandler) (*globalHandler)();
        else throw std::bad_alloc();
    }
}
```

#### 4. placement handler

​	使用“非常见形式”的*new*，比如，进行薄记的时候可能会想传入*ostream*。当使用*new*的时候

```C++
W* w_ptr = new W;
```

​	会调用*new*和W的构造函数，为了实现强烈的异常安全保证，当构造函数失败的时候，会有相应的*delete*被调用从而避免内存泄漏。当使用自定义的*new*，如果没有对应的*delete*则会出现内存泄漏

```C++
class W {
  public:
    static void* operator new(std::size_t sz, std::ostream& os);
    // 正常使用的delete
    static void operator delete(void* mem_ptr) noexcept;
    // new 调用失败 自动调用的delete
    static void operator delete(void* mem_ptr, std::ostream& os) noexcept;
}

// user：
W* w_ptr = new (std::cout) W;
```

​	为类定义*new*同样需要注意名称掩盖问题，包括类成员*new*掩盖全局的*new*，以及子类掩盖基类的*new*。