221900180 田永铭

本人此次实验全部采用matlab实现，全程独立实现无参考。

关于代码的说明：
代码在code文件夹中，需要注意的是，现在默认都是运行第一个积分，要运行其他的修改一下注释就行，特别注意第三个积分要单独处理0处的数值，要修改两处。

关于结果的说明：
结果均保证了误差在1e-7以内且三种方法高度统一，excel中有结果的汇总。梯形和simpson公式的分点我按照2的幂次来取，误差通过误差公式来求二阶或者四阶导函数的最大值来估计，从而区间数依从取1，2，4，8...直到满足精度要求.Romberg的误差直接使用公式即可。

如有问题，请联系QQ：1532606676。