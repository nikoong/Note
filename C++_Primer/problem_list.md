- [x] [ ]操作符重载,为何需要两个格式
	* type& operator[ ] (int index)
	* type operator[ ] (int index) const
- [x] constexper
	* constexper变量，可以由编译器来验证变量的值是否是一个常量表达式。
	* 函数 能拥有的唯一可执行语句就是返回语句
	* 构造函数 。函数体一般是空的
- [ ] 构造函数创建临时变量
	```cpp
    f(vector< int>(10));
    ```