# IMU LIB

## 简介

**如果觉得本项目好用不妨点下右上角的小星星哈 ; )**

翻出了这个被搁置半年的坑，硬啃了一周完成了，顺便弄了几个滤波器和数学通用库。

起因是因为MPU6050的姿态解算，用DMP的满街都是，但要换个没硬解的陀螺仪就不行了。

按本人一贯的强迫症风格，程序排版、注释比以往做得更详细了些。

**主要特点：**

- 高可移植性，纯C编写；
- 纯软件解算，兼容Cotex-M4矩阵库；
- 只需输入六轴数据，不需要硬件解算。

这东西只是个方便开发的通用库，效果不一定有硬件解算好，优点就是任何单片机任何陀螺仪都能跑，满足本人的奇怪要求。



## 程序说明

看document文件夹。



## 其他

当前版本已通过测试并达到预期效果，单MPU6050对比实验室旧版程序+正交双MPU9250仍有一定的优势，同等环境下理论精度也要更高。

考虑到可能会将此程序逐步替换旧程序，程序结构一定程度上参考了旧程序，并编写了当时可直接移植使用的示例程序用于过渡，放于NCIST文件夹中，具体看说明文档。

若本仓库有后期更新将只更新通用版本，example和library文件夹中NCIST部分的程序若无重大bug将不再进行更新，请自行对比版本号并移植。
