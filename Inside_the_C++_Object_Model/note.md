##C++面向对象模型

###第一章 关于对象
* C++对象模型
	1.非静态数据成员放在每个类对象中，静态数据成员、静态和非静态成员函数放在所有类对象之外。
	2.每个class产生一对指向虚函数的指针，放在表格之中。这个表格称为virtual tabel（vtbl）。
	3.每个class对象被安插一个指向virtual tabel的指针，这个指针称为vptr。

###第三章 Data语意学
######Data Member的存取
* 静态数据对象，将被放在程序的data segment里。如果有两个class声明了一个同名静态数据成员，编译器会暗中对每个成员编码得到不同的名字。
* 单继承、多态、多继承、虚继承的布局方式。
######指向Data Members的指针
```cpp
class Point3d{
public:
	virtual ～point3d();
protected:
	static Point3d origin;
    float x, y, z;
};
Point3d origin;
float Point3d:: *a = & Point3d::z;//指向data member的指针；将得到z在类中的偏移位置；
float* b = & origin.z;//指向已绑定的Member的指针；将得到该member在内存中的真实地址。 
```

###第四章 Function 语意学
*这本书有一大半的精华就在这里了：C++编译器究竟如何实现多态的？*
参考博客：http://www.roading.org/develop/cpp/c之虚函数virtual-member-functions.html
######单继承
当一个类单继承自有虚函数的基类的时候，将按如下步骤构建虚函数表：
* 继承基类中声明的虚函数——这些虚函数的实体地址被拷贝到继承类中的虚函数表中对于的slot中。
* 如果有改写（override）基类的虚函数，那么在1中应将改写（override）的函数实体的地址放入对应的slot中而不是拷贝基类的。
* 如果有定义新的虚函数，那么将虚函数表扩大一个slot以存放新的函数实体地址。
######多重继承
编译器要处理一个棘手的问题，将子类转换成第二个基类。根据成员布局方式我们知道这里需要一个额外的偏移量。
主要看书上的图。
######*虚继承*