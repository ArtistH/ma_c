#head 堆

### int (*compare)(const void *key1, const void *key2);

1. 最大值堆时
```
    key1>key2, return 1
    key1=key2, return 0
    key1<key2, return -1
```
2. 最小值堆时，与上方相反


### void (*destory)(void *data);

当一个结构化数据包含若干动态分配内存的数据结构成员时，
destory应当指向一个用户定义的函数来释放数据成员或结构本身的内存空间。

如果堆中的数据不需要释放，那么destory应该指向NULL.
