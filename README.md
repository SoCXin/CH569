# [CH569](https://github.com/SoCXin/CH569)

[![sites](http://182.61.61.133/link/resources/SoC.png)](http://www.SoC.Xin)

#### [Vendor](https://github.com/SoCXin/Vendor)：[WCH](https://github.com/SoCXin/WCH)
#### [Core](https://github.com/SoCXin/RISC-V)：[RISC-V](https://github.com/SoCXin/RISC-V)
#### [Level](https://github.com/SoCXin/Level)：120MHz

### [CH569简介](https://github.com/SoCXin/CH569/wiki)

[CH569](https://github.com/SoCXin/CH569)

```
RISC-V内核，120MHz系统主频，支持单周期乘法和硬件除法、可编程中断控制器、低功耗两级流水线
448KB CodeFlash，32KB DataFlash，16KB的32位宽SRAM，32/64/96KB可配置的128位宽SRAM
内置超高速USB3.0控制及收发器（内置PHY），支持USB3.0 主机Host/设备Device模式、OTG功能，支持USB3.0 HUB
内置高速USB2.0控制及收发器（内置PHY），支持USB2.0 主机Host/设备Device模式，支持控制/批量/中断/同步传输
内置千兆以太网控制器（Ethernet），提供RGMII和RMII PHY接口，支持10/100/1000Mbps的传输速率
内置数字视频接口DVP，可配置8/10/12位数据宽度，支持YUV、RGB、JPEG压缩数据
内置高速并行接口HSPI，可配置8/16/32位数据宽度，内置FIFO，支持DMA，最快传输速度约为3.8Gbps
内置SerDes控制及收发器（内置PHY，可直接驱动光纤），支持网线（仅使用1组差分线）传输90米，支持1.25Gbps高速差分信号通讯
内置EMMC控制器，支持单线、4线、8线数据通讯模式，符合EMMC卡4.4和4.5.1规范，兼容5.0规范
支持AES/SM4算法，8种组合加解密模式，支持SRAM/EMMC/HSPI外设接口数据加解密
4组UART，最高波特率6Mbps，兼容16C550，内置FIFO，多个触发级
2组SPI接口，支持主从(Master/Slave)模式，内置FIFO，支持DMA
主动并口：8位数据，15位地址总线
3组26位定时器，支持定时、计数、信号捕捉、PWM调制输出，4组扩展PWM输出，占空比可调
49个通用IO，8个可设置电平/边沿中断，部分引脚具有复用及映射功能
内置看门狗，集成2线调试接口，支持在线仿真
支持低功耗模式，支持部分GPIO、USB、以太网信号唤醒
芯片ID号：唯一64bit ID识别号
封装：QFN68,QFN40
```

### [选型建议](https://github.com/SoCXin)

[CH569](https://github.com/SoCXin/CH569) 可看作在[CH552](https://github.com/SoCXin/CH552) 基础上添加了USB主机功能，代码兼容，对于不需要主机功能的场景下使用性价比不高。

###  [SoC芯平台](http://www.SoC.Xin)
