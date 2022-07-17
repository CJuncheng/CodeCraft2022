# 文件介绍
- ```SDK_C++```: SDK文件夹，核心代码区域
- ```dataset```: 用于测试的数据集
- ```CodeCraft2022-benchmark-master```： 开源，判题器
- ```CodeCraft2022-PressureGenerator-master```：开源，生成压测数据集
- 其余为部分官方数据文档

$$T(n)=O(f(n))$$

其中：
- $T(n)$: 程序执行总时间
- $n$: 数据规模大小
- $f(n)$: 程序执行的总次数
- $O$：程序执行总时间$T(n)$与$f(n)$成正比
> 大$O$记号按最坏的情况来估计程序执行时间；大$\Omega$记号按最好情况估计程序执行时间；大$\Theta$记号按平均情况来估计程序执行时间。后文只分析大$O$记号。

1. 常数阶$O(1)$
```
public void sum(int n) {
    int sum = 0; // 执行一次
    sum = n*2; // 执行一次
    System.out.println(sum); // 执行一次
}
```
程序执行常数次，与问题规模$n$无关，复杂度记为$O(1)$

2. 对数阶$O(logn)$
```
public void logarithm(int n) {
    int count = 1; // 执行一次
    while (count <= n) { // 执行logn次
        count = count*2; // 执行logn次
    }
}
```

1. 常数阶$O(1)$
```
public void sum(int n) {
    int sum = 0; // 执行一次
    sum = n*2; // 执行一次
    System.out.println(sum); // 执行一次
}
```
程序执行常数次，与问题规模$n$无关，复杂度记为$O(1)$

2. 对数阶$O(logn)$
```
public void logarithm(int n) {
    int count = 1; // 执行一次
    while (count <= n) { // 执行logn次
        count = count*2; // 执行logn次
    }
}
```
该段代码什么时候会停止执行呢？是当count大于n时。也就是说多少个2相乘后其结果值会大于$n$，即$2^x=n$。由$2^x=n$可以得到$x=logn$，所以这段代码时间复杂度是$O(logn)$。

3. 线性阶$O(n)$
```
public void circle(int n) {
    for(int i = 0; i < n; i++) { // 执行n次
        System.out.println(i); // 执行n次
    }
}
```

4. 线性对数阶 $O(nlogn)$
线性对数阶$O(nlogn)$就是将一段时间复杂度为$O(logn)$的代码执行$n$次
```
public void logarithm(int n) {
    int count = 1;
    for(int i = 0; i < n; i++) { // 执行n次
        while (count <= n) { // 执行logn次
            count = count*2; // 执行nlogn次
        }
    }
}
```

5. 平方阶 $O(n^2)$
双重for循环
```
public void square(int n) {
    for(int i = 0; i < n; i++){ // 执行n次
        for(int j = 0; j <n; j++) { // 执行n次
            System.out.println(i+j); // 执行n方次
        }
    }
}
```

$2^4$

\[jj \]
$$\frac{((1+2+3+\cdots+n)+n)}{(n+1)}=\frac{n(n+3)}{2(n+1)$$
