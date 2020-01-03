> 文章有很多部分摘抄自 https://www.zhihu.com/people/oven-yang/posts 

# 对象模型

## C++对象模型

### 内存模型

**简单对象模型**

​	使用表格存储指针，指针指向对象的成员。

**表格驱动对象模型**

​	简单对象所有成员都存储在一张表中，表格驱动对象模型则将数据成员和成员函数分别放到两张表，而对象本身含有两个指针分别指向这两个表

> 从命名的区别看，应该就是指对象含有的指针指向成员还是表格

**C++的对象模型**(某个模型)

​	非静态数据成员存放在对象内，虚函数则是通过类似上面表格驱动模型的方式，使用虚函数表存放，在对象中使用*vptr*指向这个表。

**常见继承关系下的内存布局**

查看代码文件，用*gdb*查看内存，[参考](https://blog.csdn.net/tmhanks/article/details/89110833 )

```shell
set print object on
set print vtbl on
set print pretty on

p <obj>
info vtbl <obj>
```

### class和struct

​	[from](https://zhuanlan.zhihu.com/p/60162109) C++引入class关键字, 保留struct的一个原因是为了体现OO, 并且兼容C, 而C中不需要模板, 也就不需要保证struct可以用于模板. 

### 指针的类型

​	指向不同类型的指针的差异，即不在指针表示法不同，也不在其内容不同，而在于其寻址出来的object类型不同，即指导编译器如何解释某个特定地址中的内存内容以及大小。

​	可以结合：当出现一个带有*vptr*的继承体系的子类对象的指针被赋值给基类对象的时候,,,理解一下。

## 构造函数

> 这部分相关的内容 C++ reference应该是最好的参考资料。了解一些编译器的行为，从而知道你的代码会被如何执行，从而优化代码。

### 一. Default Constructor

>  "只有一个参数的构造函数可以被编译器作为类型转换函数"从C++11起被废止, 新标准规定具有多个参数的构造函数也可以作为转换构造函数, 新的标准是"没有被声明为*explicit*的构造函数就可以作为转换构造函数(*converting constructor*)". 	

​	关于POD对象，在*C_to_Cpp.md*中。

​	C++ reference 中说到，如果用户没有提供构造函数，编译器会默认合成一个(在合法的情况下)。但是这个合成的构造函数是“**编译器需要**”的构造函数，可能是个[trival default constructor](https://en.cppreference.com/w/cpp/language/default_constructor)，即什么都不做的构造函数，比如POD对象的默认构造函数即是。

#### nontrival default constructor

​	在什么时候，编译器会合成nontrival default constructor。

**1. 带有default constructor的member class object**

​	必须调用成员的默认构造函数，因此编译器合成了默认构造函数(编译器需要)，当用户的构造函数没有显式初始化这些成员的时候，编译器也会“安插”相应的代码。

> 不同编译模块中如何避免合成出多个默认构造函数？
>
> ​	以inline完成构造函数，对于复杂的构造函数，合成explicit non-inline static实例(**???**)

**2. 带有default constructor的Base Class**

**3. 带有Virtual Function的Class**

​	带有虚函数的类会有一个vptr指针指向虚函数表，虚函数表会在编译期间确定，关于[vptr的初始化](https://zhuanlan.zhihu.com/p/61585306)：

-  对于静态初始化的对象, vptr由编译器初始化 
-  对于动态初始化的对象, **vptr由构造函数初始化**. 因此编译器会在所有的构造函数中插入一些代码来完成这个任务  

**4. 带有一个Virtual Base Class的Class**

### 二. Copy Constructor

> In many situations, copy constructors are optimized out even if they would produce observable side-effects, see [copy elision](https://en.cppreference.com/w/cpp/language/copy_elision).  即使是移动也可能被优化掉。
>
> RVO(return value optimization)从C++17开始为强制性的 
>
> NRVO(named return value optimization).
>
> 关于rvo(和move)可以查看[文章](https://www.ibm.com/developerworks/community/blogs/5894415f-be62-4bc0-81c5-3956e82276f3/entry/RVO_V_S_std_move?lang=en)或保存的*RVO V.S. std__move .mhtml*

​	通常情况下，显式地使用同类的对象初始化新对象的时候会调用拷贝构造，而函数传入和传出由于优化的原因可能不会用到拷贝构造函数，这部分[参考](https://zh.cppreference.com/w/cpp/language/copy_elision)中的例子能解释。

编译器**默认(合成)**的两种拷贝方式

- bitwise copy :  利用memcpy等, 因此效率更高, 复制出的对象和原对象完全相同. 

-  default memberwise copy :  递归调用其默认复制构造函数来初始化 

以下情况下编译器会使用default memberwise copy：

- 类中的某成员有copy ctor的时候

- 其基类含有copy ctor的时候

  > 这两种情况可以认为成员或者基类需要bitwise copy做不到的行为，因此必须使用default memberwise copy

- 类含有vptr的时候

  > 回顾一下vtbl的样子，显然子类和父类的vtbl不一定相同，因此bitwise copy无法处理子类赋值给基类的情况。P56

- 类的基类中含有virtual base class的时候

  >  内存布局可能存在区别 

### 三. 程序转化语意学

​	这部份内容应当关注一下 copy elision ，你定义的copy ctor是提供给编译器使用的，但编译器不一定会使用，当有具体工作要做的时候应该用自己的函数确保，而不是靠“编译器应当做什么”。

​	 

## DATA 语义学

### 0大小的对象

​	对象的大小至少是1 byte, 以保证变量有唯一的地址，不过当这个对象作为基类的时候，如果子类有了数据成员，这个多余的1bit会被优化掉，因为可以用子类的地址作为父类部分的地址(即可以标识出这个子类)。

```C++
class Base {};

class Derived : public Base {
  int64_t i;
};
Base b;
Derived d;
std::cout << "sizeof(b) = " << sizeof(b) << std::endl;
std::cout << "sizeof(d) = " << sizeof(d) << std::endl;
// sizeof(b) = 1
// sizeof(d) = 8
```

​	从优化的原因可以看出，如果无法区别多个不占空间的对象，每个对象还是得占用一个字节。比如：

- 子类的第一个非静态数据成员的类型和空基类相同
- 子类的第一个非静态数据成员的基类类型和空基类相同
- 声明多个不占空间的对象

> 变量模板：以模板的方式声明变量
>
> ```C++
> template<typename T>
> T var;
> 
> var<int> = 5;
> ```

### 数据成员的存取

#### 静态数据成员

​	类的静态成员变量存储于全局数据段，为了保证不同类可以具有相同的静态成员，编译器会对每个静态数据成员编码(*name-mangling*)。

#### 非静态数据成员

​	 不存在虚基类时, 通过对象名或对象指针访问非静态数据成员没有区别. 存在虚基类时, 通过对象指针访问非静态数据成员需要在运行时才能确定, 因为无法确定指针所指对象的实际类型, 也就不能判断对象的内存布局, 也就不知道对象中该数据成员的偏移. 普通继承和虚继承的这个区别的原因在于, 普通继承的类对象的内存布局在编译时就可以决定, 而存在虚继承时则需要在运行时决定 

## Function语义学

### 成员函数

#### 1. 普通非静态成员函数

​	为了保证类的*nonnstatic member function*有着至少和一般*nonmember function*一样的效率，C++会“对将普通的*nonstatic member function*的调用转化为对*nonmember function*的调用”，步骤：

- 修改函数签名，添加一个额外的第一参数，即*this*，从而和类绑定
- 将对类中非静态成员的访问改为经*this*的访问
- 将成员函数重写为一个外部函数，生成一个独一无二的名字(*name-mangling*)

#### 2. 静态成员函数

​	同样会被转化为对*nonmember function*的调用，但是不需要添加*this*指针。

> 考虑以下情况：某函数的调用返回了一个对象，之后通过这个对象调用了对应类的静态成员函数
>
> ```C++
> Obj func();
> 
> func().static_func();
> // 虽然看起来func()没必要被调用，但是编译器会确保调用func(),以免未执行用户期望的操作
> ```

#### 3. 虚函数

​	编译器会将对类的虚函数的调用转化为通过*vptr*的调用，由于虚函数在虚表中的偏移是确定的，因此编译器会将其转化为通过索引的调用，这种方式同时也会增加一次内存访问

```C++
p->func(param);
// 编译器转化为
(*(p->vptr)[idx])(p, param)
```

### 虚函数的实现

#### 1. 单继承

​	子类继承父类的时候，对数据成员的继承体现在对象布局上，而对虚函数的继承体现在虚表上。单继承下的虚表构造步骤可能如下：

- 将父类虚表中的指针拷贝到子类虚表的相同下标位置
- 若子类重写了父类的虚函数则修改虚表中对应的指针
- 如果子类加入了新的虚函数，就增加虚表容量，在后面添加新的指针

#### 2. 多继承

​	多继承的复杂性在于：

- 通过多个父类的指针访问子类的虚函数
- 