# STM32开发方案

#### 介绍
一些日常工作做项目中用到的一些例程和方案，以便于快速做出我想要的产品功能。

#### 使用说明

- 01.USB升级

日常工作中，有的产品有USB接口，一般用来像获取U盘一样获取设备里的数据，但它还有一个功能，可以用来做USB升级，USB_DFU是个不错的选择，可以做两个分区，bootloader+app。

- 02.Fatfs存储

大部分电子产品都希望有记录查询功能，那么就少不了文件系统，在文件系统里做一些对文件的处理也是非常有必要的。

- 03.软件定时器

一般编写单片机应用程序的时候为了精准都会采用硬定时，但是在一些不用特别精准的场合下，multi_timer可以无限拓展定时器，用于解决硬件定时器接口较少的设备。

- 04.数据结构

一般在单片机上会使用的一些数据结构，诸如链表、队列、栈、哈希表等等。

#### 参与贡献

1.  Fork 本仓库
2.  新建 Feat_xxx 分支
3.  提交代码
4.  新建 Pull Request


#### 特技

1.  使用 Readme\_XXX.md 来支持不同的语言，例如 Readme\_en.md, Readme\_zh.md
2.  Gitee 官方博客 [blog.gitee.com](https://blog.gitee.com)
3.  你可以 [https://gitee.com/explore](https://gitee.com/explore) 这个地址来了解 Gitee 上的优秀开源项目
4.  [GVP](https://gitee.com/gvp) 全称是 Gitee 最有价值开源项目，是综合评定出的优秀开源项目
5.  Gitee 官方提供的使用手册 [https://gitee.com/help](https://gitee.com/help)
6.  Gitee 封面人物是一档用来展示 Gitee 会员风采的栏目 [https://gitee.com/gitee-stars/](https://gitee.com/gitee-stars/)
