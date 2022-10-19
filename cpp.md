# online reference

在线参考文档， https://cplusplus.com/

# hello world

C++编译需要有编译器， 本文使用的是 g++, 环境如下

```shell
g++ -v
Thread model: posix
gcc version 9.4.0 (Ubuntu 9.4.0-1ubuntu1~20.04.1)
```

源代码文件

```sh
vi main.cpp
```

内容如下

```cpp
#include <iostream>
using namespace std;
int main()
{
    cout << "Hello, world!" << endl;   // 不使用 “using namespace std” 就得写成 std::cout <<
    return 0;
}
```

使用 `g++`编译

```sh
g++ main.cpp -o main
gcc main.cpp -lstdc++ -o main
```

运行可执行文件

```cpp
./main
```

# 编译参数

g++ 常用命令选项

| 选项         | 解释                                                         |
| :----------- | :----------------------------------------------------------- |
| -ansi        | 只支持 ANSI 标准的 C 语法。这一选项将禁止 GNU C 的某些特色， 例如 asm 或 typeof 关键词。 |
| -c           | 只编译并生成目标文件。                                       |
| -DMACRO      | 以字符串"1"定义 MACRO 宏。                                   |
| -DMACRO=DEFN | 以字符串"DEFN"定义 MACRO 宏。                                |
| -E           | 只运行 C 预编译器。                                          |
| -g           | 生成调试信息。GNU 调试器可利用该信息。                       |
| -IDIRECTORY  | 指定额外的头文件搜索路径DIRECTORY。                          |
| -LDIRECTORY  | 指定额外的函数库搜索路径DIRECTORY。                          |
| -lLIBRARY    | 连接时搜索指定的函数库LIBRARY。                              |
| -m486        | 针对 486 进行代码优化。                                      |
| -o           | FILE 生成指定的输出文件。用在生成可执行文件时。              |
| -O0          | 不进行优化处理。                                             |
| -O           | 或 -O1 优化生成代码。                                        |
| -O2          | 进一步优化。                                                 |
| -O3          | 比 -O2 更进一步优化，包括 inline 函数。                      |
| -shared      | 生成共享目标文件。通常用在建立共享库时。                     |
| -std=        | Determine the language standard. <br/>g++ 有些系统默认是使用 C++98，我们可以指定使用 C++11 来编译<br/>g++ -g -Wall -std=c++11 main.cpp |
| -static      | 禁止使用共享连接。                                           |
| -UMACRO      | 取消对 MACRO 宏的定义。                                      |
| -w           | 不生成任何警告信息。                                         |
| -Wall        | 生成所有警告信息。                                           |

# 数据类型

##  基本的内置类型

C++ 为程序员提供了种类丰富的内置数据类型和用户自定义的数据类型。下表列出了七种基本的 C++ 数据类型。

| 类型     | 关键字  |
| :------- | :------ |
| 布尔型   | bool    |
| 字符型   | char    |
| 整型     | int     |
| 浮点型   | float   |
| 双浮点型 | double  |
| 无类型   | void    |
| 宽字符型 | wchar_t |

其实 wchar_t 是这样来的：

```cpp
typedef short int wchar_t;
```

所以 wchar_t 实际上的空间是和 short int 一样 

一些基本类型可以使用一个或多个类型修饰符进行修饰：

- signed
- unsigned
- short
- long

##  数据类型字节长度及取值范围

下表显示了各种变量类型在内存中存储值时需要占用的内存，以及该类型的变量所能存储的最大值和最小值。

**注意：**不同系统会有所差异，一字节为 8 位。

**注意：**默认情况下，int、short、long都是带符号的，即 signed。

**注意：**long int 8 个字节，int 都是 4 个字节，早期的 C 编译器定义了 long int 占用 4 个字节，int 占用 2 个字节，新版的 C/C++ 标准兼容了早期的这一设定

| 类型               | 位            | 范围                                                         |
| :----------------- | :------------ | :----------------------------------------------------------- |
| char               | 1 个字节      | -128 到 127 或者 0 到 255                                    |
| unsigned char      | 1 个字节      | 0 到 255                                                     |
| signed char        | 1 个字节      | -128 到 127                                                  |
| int                | 4 个字节      | -2147483648 到 2147483647                                    |
| unsigned int       | 4 个字节      | 0 到 4294967295                                              |
| signed int         | 4 个字节      | -2147483648 到 2147483647                                    |
| short int          | 2 个字节      | -32768 到 32767                                              |
| unsigned short int | 2 个字节      | 0 到 65,535                                                  |
| signed short int   | 2 个字节      | -32768 到 32767                                              |
| long int           | 8 个字节      | -9,223,372,036,854,775,808 到 9,223,372,036,854,775,807      |
| signed long int    | 8 个字节      | -9,223,372,036,854,775,808 到 9,223,372,036,854,775,807      |
| unsigned long int  | 8 个字节      | 0 到 18,446,744,073,709,551,615                              |
| float              | 4 个字节      | 精度型占4个字节（32位）内存空间，+/- 3.4e +/- 38 (~7 个数字) |
| double             | 8 个字节      | 双精度型占8 个字节（64位）内存空间，+/- 1.7e +/- 308 (~15 个数字) |
| long double        | 16 个字节     | 长双精度型 16 个字节（128位）内存空间，可提供18-19位有效数字。 |
| wchar_t            | 2 或 4 个字节 | 1 个宽字符                                                   |

在具体环境中每中类型所占用的字节数及其取值范围可以通过  `sizeof` 关键字来查看，如下所示:

```cpp
#include<iostream>  
#include <limits>
 
using namespace std;  
  
int main()  
{  
    cout << "type: \t\t" << "************size**************"<< endl;  
    cout << "bool: \t\t" << "所占字节数：" << sizeof(bool);  
    cout << "\t最大值：" << (numeric_limits<bool>::max)();  
    cout << "\t\t最小值：" << (numeric_limits<bool>::min)() << endl;  
}
```

## 自定义类型 typedef

typedef 为一个已有的类型取一个新的名字

```cpp
typedef int feet;			// 告诉编译器，feet 是 int 的另一个名称
feet distance;				// 创建了一个整型变量 distance
```

##  枚举 enum

```cpp
enum color { red, green, blue } c;
c = blue;
```

默认情况下，第一个名称的值为 0，第二个名称的值为 1，第三个名称的值为 2，以此类推。但是，您也可以给名称赋予一个特殊的值，只需要添加一个初始值即可。例如，在下面的枚举中，**green** 的值为 5。

```cpp
enum color { red, green=5, blue };
```

**blue** 的值为 6，因为默认情况下，每个名称都会比它前面一个名称大 1，但 red 的值依然为 0

# 变量类型

##  全局变量(extern)

有两个以上文件都需要使用共同的变量，我们将这些变量定义为全局变量。

a.cpp

```cpp
int i;                // 全局变量声明并定义i
int j = 1;            // 声明并定义j
double max(double d1,double d2)
{
    return 0.0;
} //定义
```

b.cpp

```cpp
#include <iostream>
using namespace std;

extern int i;          	// 声明 i 而非定义
extern int j;     		// 定义 j 而非声明，会报错，多重定义
//int j;                // 错误，重定义，注意这里的 j 是在全局范围内声明
extern double max(double d1,double d2); //声明

int main()
{
    i = 10;
    j = 20;
    cout << i << ", " << j << endl;
}
```

运行

```cpp
g++ a.cpp b.cpp
./a.out
```

与include相比，extern引用另一个文件的范围小，include可以引用另一个文件的全部内容。extern的引用方式比包含头文件要更简洁。extern的使用方法是直接了当的，想引用哪个函数就用extern声明哪个函数。这样做的一个明显的好处是，会加速程序的编译（确切的说是预处理）的过程，节省时间。在大型C程序编译过程中，这种差异是非常明显的。

##  函数定义

func1.cpp

```cpp
int func1() 		//delcare and define
{
    int a=1;
    int b=2;
    return a+b;
}
```

func2.cpp

```cpp
#include <iostream>
using namespace std;
 
// declare func1(), defined in fun1.cpp
int func1();
 
int main()
{
    int a = func1();
   cout << a << endl; // 输出 Hello World
   return 0;
}
```

compile

```sh
 g++ func1.cpp func2.cpp -o func
 ./func
```

##  常量

###  常量表示

0x 或 0X 表示十六进制，0 表示八进制，不带前缀则默认表示十进制。整数常量也可以带一个后缀，后缀是 U 和 L 的组合，U 表示无符号整数（unsigned），L 表示长整数（long）。后缀可以是大写，也可以是小写，U 和 L 的顺序任意。

```cpp
212         // 合法的
215u        // 合法的
0xFeeL      // 合法的
078         // 非法的：8 不是八进制的数字
032UU       // 非法的：不能重复后缀
3.14159       // 合法的 
314159E-5L    // 合法的 
510E          // 非法的：不完整的指数
210f          // 非法的：没有小数或指数
.e55          // 非法的：缺少整数或分数
```

数字常量的实例：

```cpp
85         // 十进制
0213       // 八进制 
0x4b       // 十六进制 
30         // 整数 
30u        // 无符号整数 
30l        // 长整数 
30ul       // 无符号长整数
```

布尔常量

```cpp
true		// 真
false		// 假
```

 字符串常量

```cpp
#include <iostream>
#include <string>
using namespace std;

int main() {
    string greeting = "hello, world";
    cout << greeting;
    cout << "\n";     // 换行符
    string greeting2 = "hello, \
                       world";
    cout << greeting2;
    return 0;
}
```

###  常量定义

\#define 预处理器定义常量

```cpp
#define identifier value
```

test

```cpp
#include <iostream>
using namespace std;
 
#define LENGTH 10   
#define WIDTH  5
#define NEWLINE '\n'
 
int main()
{
 
   int area;  
   area = LENGTH * WIDTH;
   cout << area;
   cout << NEWLINE;
   return 0;
}
```

# 数据结构

##  Array(数组)

```cpp
 int n[ 10 ]; 			// n 是一个包含 10 个整数的数组
```

##  string(字符串)

（1）C 风格的字符串

```cpp
char a1[7] = {'H', 'E', 'L', 'L', 'O', '\0'};
char a2[] = "HELLO";
```

示范

```cpp
#include <iostream>

using namespace std;
int main() 
{
    char a1[7] = {'H', 'E', 'L', 'L', 'O', '\0'};
	char a2[] = "HELLO";
    cout << a1 << endl;
    cout << a2 << endl;
}
```

（2）c++ 中的字符串

```cpp
string str1 = "hello";
```

示范

```cpp
#include <iostream>

using namespace std;
int main() 
{
    string a="hello";
    cout << a << endl;
}
```

##  STL 容器

STL(Standard Template Library，标准模板库)，是惠普实验室开发的一系列软件的统称。现在主要出现在 C++中，但是在引入 C++之前该技术已经存在很长时间了。

STL 从广义上分为:容器(container) 、算法(algorithm) 和迭代器(iterator)。STL 的 13个头文件 

```cpp
<algorithm>、<deque>、<functional>、<iterator>、<vector>、<list>、<map>、<memory>、<numeric>、<queue>、<set>、<stack>和<utility>
```

###  通用方法

```cpp
.size();               // 容器内元素个数
.empty();              // 判断容器是否为空, 返回值为 bool 型
.front();               // 返回容器第一个元素
.back();               // 返回容器最后一个元素
.begin();              // 返回容器第一个元素的指针
.end();                // 返回容器最后一个元素的指针
.swap(b);              // 交换两个容器的内容
:: iterator;           // 迭代器
```

### array（数组）

```cpp
.at(index);					// 读写某个元素
.back();					// 读取 array 中的最后一个元素
```

示例

```cpp
#include <iostream>
#include <array>

int main ()
{
  std::array<int,10> myarray;

  // assign some values:
  for (int i=0; i<10; i++) myarray.at(i) = i+1;

  // print content:
  std::cout << "myarray contains:";
  for (int i=0; i<10; i++)
    std::cout << ' ' << myarray.at(i);
  std::cout << '\n';
  std::cout << "myarray.back()=" << myarray.back() << std::endl;  

  return 0;
}
```





### list（列表）

`list` 是数据结构中的双向链表， 它的内存空间是不连续的，通过指针进行数据的访问。由于链表的特点，`lis`t可以在任意位置进行插入和删除的序列式容器，并且该容器可以前后双向迭代，但不支持随机访问，即`[i]`不可用，因为随机存取效率非常差。

```cpp
list<string> l;				// 定义
.begin(); 					// 返回第一个元素的迭代器
.end(); 					// 返回最后一个元素下一个位置的迭代器
.rbegin(); 					// 返回第一个元素的reverse_iterator,即end位置
.rend(); 					// 返回最后一个元素下一个位置的reverse_iterator,即begin位;
.empty();					// 判断是否为空
.size();					// 返回list中有效节点的个数
.push_front(val);			// 在list首元素前插入值为 val 的元素
.pop_front();				// 删除list中第一个元素，返回 void
.push_back (val);			// 在list尾部插入值为val的元素
.pop_back();				// 删除list中最后一个元素，返回 void
.insert(pos, elem);			// 在位置 pos 处插入元素 elem
iterator .erase (iterator position)    			// 删除list position位置的 元素
iterator .erase (iterator ﬁrst, iterator last)   // 删除list中[ﬁrst, last)区 间中的元素
.clear() 					// 清空list中的有效元素
.sort() 					//给list排序 
.splice() 					// 合并两个list 
.swap() 					// 交换两个list 
.unique() 					// 删除list中重复的元素
find(it1,it2,elem);			// 在迭代器 it1 和 it2 之间查找值为 elem的元素, 需要 #include<algorithm>
.reverse()					// 逆序
.merge(another_list)		// 与 another_list 合并， 合并后 another_list 为空
```

示例

```cpp
#include <iostream>
#include <list>
#include <string>
#include <algorithm>

using namespace std;
int main()
{
    list<string> l;
    l.push_back("hello");
    l.push_back("world");
    l.push_back("who");
    l.push_back("are");
    l.push_back("you");
     l.push_back("who");
    l.push_back("OK?");
    list<string>::iterator it0 = l.begin();
    list<string>::iterator it1 = l.end();
    it1--;
    list<string>::iterator itp = find(l.begin(), l.end(), "world");
    cout << *it0 << "\t" << *it1 << "\t" << *itp << endl;
    list<string>::iterator i;
    cout << "befor sort and unique" << endl;
  	for(i = l.begin(); i !=l.end(); ++i) {
        cout << *i << endl;
    }
    l.sort();
    l.unique();
    cout << "after sort and unique" << endl;
    for(i = l.begin(); i !=l.end(); ++i) {
        cout << *i << endl;
    }
    l.reverse();
    cout << "after reverse" << endl;
    for(i = l.begin(); i != l.end(); ++i) {
        cout << *i << endl;
    }
    
    list<string> l2;
    l2.push_back("hello l2");
    l2.push_back("world l2");
    l2.push_back("you l2");
    l.merge(l2);
    cout << "after merge" << endl;
    for(i = l.begin(); i != l.end(); ++i) {
        cout << *i << endl;
    }
    cout <<"l2.size()=" << l2.size() << "l2.empty()=" << l2.empty() << endl;
    return 0;
}
```

###   vector（向量）

`vector`和`built-in`数组类似，拥有一段连续的内存空间，支持随即存取，即`[]`操作符,可以理解为动态数组，但由于它的内存空间是连续的，所以在中间进行插入和删除会造成内存块的拷贝，另外，当插入较多的元素后，预留内存空间可能不够，需要重新申请一块足够大的内存并把原来的数据拷贝到新的内存空间。这些影响了`vector`的效率，但是实际上用的最多的还是`vector`容器，建议大多数时候使用`vector`效率一般是不错的，其基本操作如下所示。

```cpp
.push_back(element)			// 顺序添加元素
.insert(pos, element)		// 在某个位置添加元素
.pop_back()					// 取元素
.empty()					// 判断是否为空，为空返回真，反之返回假
.erase(first,last)			// 删除[first,last)的所有元素
.resize(n,v)				// 改变数组大小为n, n个空间数值赋为v，如果没有默认赋值为0
 [i]						// 下标访问
```

示例

```cpp
#include <iostream>
#include <vector>

using std::vector;
using namespace std;
int main(void)
{
    vector<int> v;
    cout<<"v.size() == " << v.size() << " v.capacity() = " << v.capacity() << endl;
    v.reserve(10); 		//
    std::cout<<"v.size() == " << v.size() << " v.capacity() = " << v.capacity() << endl;
    v.resize(10);
    v.push_back(111);
    cout<<"v.size() == " << v.size() << " v.capacity() = " << v.capacity() << endl;
    cout << v.back() << endl; 
    v.insert(v.begin(), 12);
    cout << v[0] << endl;
    cout << v.front() << endl;
    cout << v.empty() << endl;
    v.erase(v.begin(), v.end());
    cout << v.empty() << endl;
    return 0;
}
```

###  stack（栈）

基本操作

```cpp
stack<T> s;				// 定义， T 为容器中元素的类型
.push(x);				// 压栈，增加元素 x
.pop();					// 仅用于从堆栈中移除元素，并且没有返回值
.top();					// 取得栈顶元素
.empty()				// 检测栈内是否为空，空为真
.size()					// 返回stack内元素的个数
```

示例

```cpp
#include <iostream>
#include <stack>

using namespace std;
int main(void)
{
    stack<int> si;
    stack<string> ss;
    si.push(100);
    si.push(200);
    ss.push("hi,100");
    ss.push("hi 200");
    si.pop();
    ss.pop();
    cout << si.top() << "\t"<< ss.empty() << endl;
    cout << ss.size() << endl;
    return 0;
}
```

###  queue（队列）

基本操作如下，除了queue外， 还有优先队列(priority_queue)。

```cpp
queue<T> q;			// 定义， T为元素类型
.push(x);			// 尾部添加一个元素副本
.pop();				// 删除第一个元素,返回 void
.front();			// 返回队列首元素的值，但不删除该元素
.back();			// 返回队列尾的值，但不删除该元素
.empty();			// 判断是否为空，队列为空，返回true
.size();			// 返回队列中元素个数，返回值类型unsigned int
```

示例

```cpp
#include <iostream>
#include <queue>

using namespace std;
int main(void)
{
    queue<int> qi;
    queue<string> qs;
    qi.push(100);
    qi.push(200);
    qs.push("hi,100");
    qs.push("hi,200");
    qs.push("hi,300");
    qi.pop();
    qs.pop();
    cout << qi.front() << "\t" << qi.front() << "\t" << qi.empty() << endl;
    cout << qs.back() << "\t" << qs.size() << endl;
    cout << qs.back() << "\t" << qs.size() << endl;
    return 0;
}
```

###  set（集合）

 `set`基于红黑树实现，容器内部不允许出现重复的元素，基本操作如下。 multiset（多重集合）允许出现重复的元素

```cpp
set<int> s;			//定义
.begin();			// 返回set容器的第一个元素
.end();				// 返回set容器的最后一个元素
.rbegin();			// 返回逆序迭代器，指向容器元素最后一个位置
.insert(x);			//
.erase();			// 
.find(e)			// 查找set中的某一元素，有则返回该元素对应的迭代器，无则返回结束迭代器
.empty(iterator)	// 删除定位器iterator指向的值
.erase(first,second）// 	删除定位器first和second之间的值
.erase(key_value)	// 删除键值key_value的值
.count()			// set中某个某个键值出现的次数, 0, 1, 因为set 中的元素不能重复
.clear()			// 删除set容器中的所有的元素,返回unsigned int类型O(N)
.lower_bound(k) 	// 返回大于等于k的第一个元素的迭代器
.upper_bound(k)		// 返回大于k的第一个元素的迭代器

```

示例

```cpp
#include <iostream>
#include <set>

using namespace std;
int main(void)
{
    set<int> si;
    si.insert(100);
    si.insert(200);
    si.insert(200);					// 无法插入重复元素
    si.insert(300);
    si.insert(300);					// 无法插入重复元素
    cout << "set element" << endl;
    set<int>::iterator i;
    for(i = si.begin(); i !=si.end(); ++i) {
        cout << *i << endl;
    }
    
    multiset<int> msi;
    msi.insert(100);
    msi.insert(200);
    msi.insert(200);					// 可以插入重复元素
    msi.insert(300);
    msi.insert(300);					// 可以插入重复元素
    cout << "multiset element" << endl;
    multiset<int>::iterator j;
    for(j = msi.begin(); j !=msi.end(); ++j) {
        cout << *j << endl;
    }
    
    set<string> ss;
    ss.insert("hi,100");
    ss.insert("hi,200");
    ss.insert("hi,300");
    si.erase(si.begin());
    set<string>::iterator sit = ss.begin();
    sit++;
    ss.erase(ss.begin(), sit);
    set<int>::iterator it0 = si.begin();
    cout << "another test" << endl;
    cout << *it0 << "\t" << *it0 << "\t" << si.empty() << endl;
    set<int>::iterator it1 = si.find(100) ;
    set<string>::iterator it2 = ss.find("hi,300") ;
    cout << *it1<< "\t" << ss.size() << endl;
    cout << *it2 << "\t" << ss.size() << endl;
    return 0;
}
```

### map（映射）

基本操作如下，除了map外，还有 multimap（多重映射）

```cpp
map<int, string> m;			//定义
.insert(pair<int, string>(1, "hello"));	//添加元素
.earase(key);				// 删除元素
m[key];						// 获取 key 对应的 value 值
m[key]=value;				// 修改key 对应的value值
.find(key)->first;			// 根据 key 值查找对应的 pair， first 输出 key
.find(key)->second;			// 根据 key 值查找对应的 pair， second 输出 value
.clear();					// 清除所有元素
.empty();					// 是否为空，空返回1, 非空返回0
.size();					// 返回元素的个数
.max_size();				// 返回可以容纳的元素最大个数
```

示例

```cpp
#include <iostream>
#include <map>
#include <string>

using namespace std;
int main(void)
{
    map<int, string> m;
    m.insert(pair<int, string>(1, "hello1"));
    m.insert(pair<int, string>(3, "hello3"));
     m.insert(pair<int, string>(10, "hello10"));
    m[3]="hello33";
    cout << 3 << "\t" << m[3] << endl;
    cout << m.empty() << "\t" << m.size() << "\t" << m.max_size() << endl;
    cout << m.find(3)->first << "\t" << m.find(3)->second << endl;
}
```

###  deque（双端队列）

[];//支持随机选取，但其效率较低

```
push_back();//在队尾插入元素
pop_back();//弹出队尾元素
push_front();//在队头插入元素
pop_front();//弹出队头元素
size();
empty();
clear();
```

### Red Black Tree（红黑树）

基本操作

```cpp
//RB树的起头为最左节点（指向数值最小的节点），类里面含有一个起始迭代器和结束结束迭代器，用来管理树
iterator begin() { return leftmost(); }
//RB树的终点，最右边节点（指向数值最大的节点），头结点使用，使其实现左闭右开的用法。
iterator end() { return header; }
//将x插入到RB-tree中（保持节点key值独一无二），返回一个pair对象，里面存储对应迭代器和成功与否。
 pair<iterator,bool> insert_unique(const value_type& x);
//将x插入到RB-tree中（允许节点值重复），返回插入的新节点的迭代器。
 iterator insert_equal(const value_type& x);
```

示例

```cpp
#include <bits/stl_tree.h>
#include <iostream>

using namespace std;

int main(void)
{
    _Rb_tree<int, int, _Identity<int>, less<int>> itree;
    cout << "itree.empty()=" << itree.empty() << endl;
    cout << "itree.size()=" << itree.size() << endl;

    itree._M_insert_unique(3);
    itree._M_insert_unique(8);
    itree._M_insert_unique(5);
    itree._M_insert_unique(9);
    itree._M_insert_unique(13);
    itree._M_insert_unique(5);								//没有作用，因为unique插入

    cout << "itree.empty()=" << itree.empty() << endl;
    cout << "itree.size()=" << itree.size() << endl;
    cout << "itree.count(5)=" << itree.count(5) << endl;	//1 找出红黑树中节点为5的个数

    itree._M_insert_equal(5);
    itree._M_insert_equal(5);

    cout << "itree.size()=" << itree.size() << endl;		//7
    cout << "itree.count(5)=" << itree.count(5) << endl;		//3 找出红黑树中节点为5的个数
}
```

## Tree(自定义树)

Tree  是一个具有递归结构的数据结构（a recursive data structures），可以通过结构体的定义，实现树

```cpp
struct Node {
	int data;
	Node* left;
	Node* right;
}
```

示例

```cpp
#include <iostream>

using namespace std;
int main(void)
{
	struct Node {
        int data;
        Node* left;
        Node* right;
    };
    struct Node n0, n1, n2, n3;
    n0.data = 0;
    n0.left = &n1;
    n0.right =&n2;
    n1.data=1;
    n2.data=2;
    cout << "n0.data=" << n0.data << endl;
    cout << "n0.left.data=" << n0.left->data << endl;
    cout << "n0.right.data=" << n0.right->data << endl;
    return 0;
}
```
