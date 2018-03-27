* 先写测试案例，再写算法；
* 如果题目要求是按照一定要求摆放若干数字，我们可以先求出这些数字的所有排列，然后再一一判断每个排列是不是满足题目给定的要求。
* 二分查找：
	* 使用 Start代表序列第一个数， End代表序列最后一个元素之后的一个位置；
	* mid = (Start + End -1) >>2 //-1是因为不能让mid等于End；
* 对于序列左旋转问题，可以使用两次翻转解决。
	* 左旋转就是循环左移，翻转是首尾交换。右旋转等价于一种左旋转。
		* ab|cde -> ba|edc -> cdeab /左旋转两位
	* 对于总体翻转，局部顺序不变，也可以使用两次翻转方法。总体翻转->局部翻转
		* ab cd ef -> fe dc ba -> ef cd ab

* int 类型最大值 0x7FFFFFFF,最小值(int)0x80000000;
* 中位数问题，用一个最大堆，一个最小堆；

* 对类似需要用到滑动窗口，可以储存从第一个元素加到第n个元素的和，然后记录在位置n上。


###正则表达式问题:
匹配字符串和模式，模式可以有'.'和'*'两种特殊字符。
* 对'*'字符的求解是问题核心
* 每次先比较下一个字符是不是'*'
```cpp
bool match(char* str, char* pattern){
    if (str == NULL || pattern == NULL)
        return false;
    return matchCore(str, pattern);
}
bool matchCore(char* str, char* pattern){
    if(*str == '\0' && *pattern == '\0')
        return true;
    if(*str != '\0' && *pattern == '\0')
        return false;
    //每次先比较下一个字符是不是'*',如果不是就是简单的比较问题
    if( *(pattern+1) != '*' ){
    	//'.'字符要和'\0'比较
        if(*pattern == *str || (*pattern == '.' && *str != '\0'))
            return matchCore(str+1, pattern+1);
        else
            return false;
    //如果下一个字符是'*'
    }else{
    	//且当前字符串中字符和模式匹配
        if ( *pattern == *str ||(*pattern == '.' && *str != '\0') )
            /*可能有三种情况：
              	如果代表零个a：字符串不动，模式前进两格
            	如果代表多个a：字符串前进一格，最终会递归成只剩下一个a的情况
                如果代表一个a：字符串前进一格，模式前进两格
            */
            return matchCore(str, pattern+2) || matchCore(str+1, pattern) || matchCore(str+1, pattern+2);
        else
            return matchCore(str, pattern+2);
            
        }
    }
```