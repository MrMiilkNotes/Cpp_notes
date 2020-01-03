## [POD](https://zhuanlan.zhihu.com/p/45545035)

### POD

​	POD 是 Plain Old Data 的缩写，是 C++ 定义的一类数据结构概念，比如 int、float 等都是 POD 类型的。Plain 代表它是一个普通类型，Old 代表它是旧的，与几十年前的 C 语言兼容，那么就意味着可以使用 memcpy() 这种最原始的函数进行操作。两个系统进行交换数据，如果没有办法对数据进行语义检查和解释，那就只能以非常底层的数据形式进行交互，而拥有 POD 特征的类或者结构体通过二进制拷贝后依然能保持数据结构不变。**也就是说，能用 C 的 memcpy() 等函数进行操作的类、结构体就是 POD 类型的数据**。 对应的，如果在C++中需要仿造C中的*struct*可以使用POD类型，使用*reinterpret_cast<A\*>*。

### 要求

#### 1. trival

​	就是特别平凡。

- 没有构造/析构/拷贝/移动函数。当类为空类或者数据成员只有内置类型的时候，呈现*bitewise copy semantics*，[see](https://www.zhihu.com/question/302938908).

```C++
class A{
    A() {};	//不是POD
};
class B{};
class C{
    C() = default;
};

//====================
// 使用is_trival<T>::value能判断是不是POD类型
```

- 不能有虚函数和虚基类

#### 2. 布局有序

​	《C++对象模型》：(P20)，凡处于同一个*access session*的数据，必定保证其以声明顺序出现在内存中，如果是不同的*access session*就不一定了。

```C++
class A{
  public:
    int a;
  private:
    int b;
}
is_trival<A>::value // -> false
```

​	C中没有继承体系，所以如果存在继承关系，所有非静态成员需要放在同一个*class*中。