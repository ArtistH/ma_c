#notes of Mastering Algorithms with c

TP312C/L243

算法精解 C语言描述

由于实现的互包含性，去除目录结构

### sort (比较排序， 线性时间排序)

* qksort.c 快速排序 O(n lg n), 最坏O(n ** 2)
* rxsort.c 基数排序 O(pn + pk) (p元素位数，k基数)(空间 2个n大小, 1个k大小数组)
* ctsort.c 计数排序 O(n + k) (k max+1)(空间 2个n，1个k数组)
* mgsort.c 归并排序 O(n lg n) (空间 2n)
* issort.c 插入排序 O(n ** 2), 在递增序列中O(n)
* 

### other sort 

* 冒泡排序 O(n ** 2)
* 选择排序 O(lg n) three times space
* 堆排序 O(n lg n)
* 内省排序
* 桶排序
* 

### search

* bisearch.c 二分查找 O(lg n)
* 
