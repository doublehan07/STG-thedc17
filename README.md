<<<<<<< HEAD
#STG_thedc17

Updated by haldak, 10/30/2015, 4:55am
大家加油呀~

.md文件支持Markdown语法，有需要可以参考这个：http://wowubuntu.com/markdown/#list

记得在账单中添加各种支出~

## 比赛时间
- 预审 11.8(第8周周日)
- 初赛 11.22(第10周周日)
- 决赛 12.06(第12周周日)

## 任务清单
- [ ] 11.07(Sat), 参加预审，目标A档前两名
- [ ] 11.06(Fri), 测试通信逻辑是否可用，完成预审要求
- [ ] 11.04(Wed), 完善小车运行和道具寻找、道具使用逻辑
- [ ] 11.04(Wed), 测试红外功能
- [x] 11.02(Mon), 测试Vpropi是否好用
- [x] 11.01(Sun), 通信logic完善接收（传输校验），发送
- [x] 11.01(Sun), 调出红外循迹
- [x] 10.30(Fri)，测试18650电池是否好用
- [x] 10.30(Fri)，分发DISCOVERY板子，入新的传感器
- [x] 10.30(Fri)，焊接两块板子，测试电路板是否可以使用


## 疑惑交流

## 预审方案
1. 使用伯文画的大板子
2. 红外循迹，暂时不考虑获得地图数据并储存
3. 小车的速度尽可能快
4. 实现防御功能

## 未来任务
//【暂时不考虑】1. 电机驱动小板子，使用DRV881，有电压环吗？
2. 地图信息处理AI（i.e. 储存地图信息，获取道具策略，进攻对方策略）
3. 进攻AI与防御AI
4. 黑科技干扰对方
5. ！【重要】！摄像头代替红外方案
=======

This repository contains source code for the articles
* [STM32 Discovery Development on Linux](http://www.wolinlabs.com/blog/linux.stm32.discovery.gcc.html) 
* [STM32F4 Discovery Semihosting with the GNU Tools for Embedded ARM Processors Toolchain](http://www.wolinlabs.com/blog/stm32f4.semihosting.html)
* [STM32F4 Discovery Virtual COM/Serial Port](http://www.wolinlabs.com/blog/stm32f4.virtual.com.port.html)



<br>
Contents:

1. LED blinker sample code (buildable)
2. USB virtual serial port code (buildable)
2. Semihosting Hello World sample code (buildable)
3. STM sample code (buildable)
4. STM libraries and headers


<br>
STM libraries, headers, and samples have been modified to work with GNU Tools for ARM Embedded Processors toolchain and STM32F4 Discovery EVB.   Generally this just involved adding Makefiles and a slightly modified version of the TrueStudio linker file.   In the Project directory, everything under Demonstration, FW\_upgrade, and Peripheral\_Examples is buildable from the command line by running 'make'

STM's approach toward 'library code' is somewhat non-traditional.  Rather than building libraries up front and linking them with your source, STM appears to intend for you to include their individual source files in your code's build.   Some of their source files include local headers and modules (i.e. stm32f4xx\_conf.h, stm32f4xx\_it.c/h, system\_stm32f4xx.c, etc) that you modify/store in your source tree.  Because of this, building the library up front wouldn't work properly, as the configuration for some library modules could vary from project to project.





>>>>>>> origin/master
