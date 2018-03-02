
* 空stack调用top()或者pop()，会产生段错误，所以要先进行判空，如：
```cpp
if(!st.empty() )
```


#####queue
- empty()
- front() //即将离开队列的元素
- pop()
- push()


#####vector
- 构造函数
	1. std::vector<int> first;                                // empty vector of ints
	2. std::vector<int> second (4,100);                       // four ints with value 100
	3. std::vector<int> third (second.begin(),second.end());  // iterating through second
	4. std::vector<int> fourth (third);                       // a copy of third
- push_back( ele )
- pop_back()



#####map
* find(key_type k) //是否有这个key，如果有返回指向对应值的迭代器；否则返回map::end
