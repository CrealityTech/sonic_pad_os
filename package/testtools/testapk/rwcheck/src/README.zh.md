# read/write check

rwcheck是一个对嵌入式设备进行读写压测的工具

## 什么是rwcheck

正如其名，rwcheck工具用于读写压测。它是什么工作原理呢？为什么要用rwcheck呢？

rwcheck的初衷是为嵌入式Linux设备提供读写冒烟测试的工具，在大压力的IO测试中，确保存储稳定。rwcheck也被用于读写掉电测试，在读写时掉电，并在上电后对掉电前写入的文件进行检查。

他有以下特点：

* 多种测试模式，覆盖各种尺寸的测试文件
* 读写随机数据，覆盖到各种数据组合
* CRC校验数据，错误即刻退出并保留问题现场
* 支持读写掉电，重启后校验历史数据
* 动态获取系统信息，按存储空间使用比例限制测试总大小
* 支持多线程写，模拟真实使用场景

## 怎么用rwcheck

### 下载与编译

```
git clone https://github.com/gmpy/rwcheck.git rwcheck && make -C rwcheck
```

### 使用说明

**rwcheck -h**可以获取到使用说明

```
  Usage: rwcheck [-h] [-d dir] [-t times] [-b size] [-e size]
		   [-s size] [-u size] [-p percent] [-i input] [-j jobs]

	-h : show this massage and exit
	-d # : the diretory to check [default currect path]
	-t # : check times
	-b # : [up mode] set begin size
	-e # : [up mode] set end size
	-s # : [same mode] set file size
	-u # : set read/write buf size
	-p # : set maximum ratio of total flash size to check. Eg. -p 95
	-i # : input path of file [default <check_dir>/rwcheck.org]
		   if file don't existed, create 64K from /dev/urandom
	-j # : multiple jobs

  size trailing with k|m|g or not

  rwcheck work in 3 mode:
  1. -s # : files have the same size, keep testing until no enough space
  2. -b # :
  2. -e # : file size increase by The multiplier of 2, loop
			from beginning to ending size until no enough space
  3. none : file size is 50% of the free space, keep testing until
			the less space is less than 64K
```

例如：

```
rwcheck -d /mnt/UDISK -b 128k -p 90 -j 2 -t 10000000
```

上面命令的含义：

* 在 **/mnt/UDISK** 目录下进行读写
* **UP模式**，测试从 **128K** 文件大小开始，使用默认最大文件大小（16G）
* 测试总大小为总容量的 **90%**
* 以 **2个线程** 同时写
* 循环测试 **10000000次**

### 1次循环

```-t <次数>```可以指定循环测试次数，那么，怎样才是1次循环呢？

每一次循环，包含3个步骤：

1. 写：按[不同模式](#测试模式)要求创建多个文件，循环创建直至空间使用率达到设定的百分比
2. 校验：读取每一个文件，校验文件CRC值 *(允许最后一个文件CRC校验值错误)*
3. 删除：删除测试文件

```
Q：为什么允许最后一个文件CRC校验值错误？
A：因为在随机掉电情况下，最后一个文件写入不完整，会导致校验值不准确，但实际上，这并不是错误

Q：为什么有时候没写任何文件直接跳入到校验环节？
A：当检查到已有文件存在的时候，写环节会跳过，进入到校验环节，以此实现检查上一次执行时创建的文件
```

### 随机数据

rwcheck写入的数据是从```/dev/urandom```获取的随机数据，确保了测试覆盖到尽可能多的数据组合。但这样也存在个问题，当出现数据校验出错时，并没有原始数据对比分析，怎么办呢？

rwcheck是这么做的：

1. 在每一次循环开始之前，从```/dev/urandom```读取一定大小的随机数据到 **rwcheck.org**
2. **rwcheck.org** 作为数据源，从 **rwcheck.org** 循环读，获取足够数据写入到测试文件
3. 一次测试循环后，删除旧的 **rwcheck.org** 文件，以便下一次循环创建新的随机数据源

由于**一旦出错rwcheck会即刻退出以保留现场**，因此可以通过对比 **rwcheck.org** 与出错文件，分析错误情况

### 测试日志

```Shell
root# rwcheck -d /mnt/UDISK -t 2 -b 128k -p 85 -j2

        rwcheck: do read and write check

        version: v0.1.0
        build: Compiled in Aug 20 2019 at 02:32:41
        date: Mon Aug 19 18:01:21 2019

        free/total ddr: 33/53 MB
        free/total flash: 62/63 MB
        flash filesystem ubifs
        set mode to Up Mode
        set file begin size to 128 KB
        set file end size to 1 GB
        set times to 2
        set max percent of total space to 85%
        set buf size to 512 KB
        set check diretory as /mnt/UDISK
        set orgin file as /mnt/UDISK/rwcheck.org
        set jobs as 2

		--- CREATE ---
        create  : /mnt/UDISK/rwcheck.tmp.0_1 ... OK (64K)
        create  : /mnt/UDISK/rwcheck.tmp.0_0 ... OK (64K)
        create  : /mnt/UDISK/rwcheck.tmp.1_1 ... OK (128K)
        create  : /mnt/UDISK/rwcheck.tmp.1_0 ... OK (128K)
						......
        create  : /mnt/UDISK/rwcheck.tmp.7_0 ... OK (64K)
        create  : /mnt/UDISK/rwcheck.tmp.7_1 ... OK (64K)
        --- CHECK ---
        check   : /mnt/UDISK/rwcheck.tmp.0_0 ... OK
        check   : /mnt/UDISK/rwcheck.tmp.0_1 ... OK
						......
        check   : /mnt/UDISK/rwcheck.tmp.7_0 ... OK
        check   : /mnt/UDISK/rwcheck.tmp.7_1 ... OK
        --- REMOVE ---
        remove  : /mnt/UDISK/rwcheck.tmp.0_0 ... OK
        remove  : /mnt/UDISK/rwcheck.tmp.0_1 ... OK
						......
        remove  : /mnt/UDISK/rwcheck.tmp.7_0 ... OK
        remove  : /mnt/UDISK/rwcheck.tmp.7_1 ... OK
```

一旦出现错误，会打印类似的错误信息

```
        --- CREATE ---
        --- CHECK ---
        check   : /mnt/UDISK/rwcheck.tmp.0_0 ... OK
        check   : /mnt/UDISK/rwcheck.tmp.0_1 ... OK
        check   : /mnt/UDISK/rwcheck.tmp.1_0 ... OK
        check   : /mnt/UDISK/rwcheck.tmp.1_1 ... OK
        check   : /mnt/UDISK/rwcheck.tmp.2_0 ... FAILED (crc error)

        --- ERROR INFO ---
        file: rwcheck.c (573)
        errno: I/O error (5)
        info: /mnt/UDISK/rwcheck.tmp.2_0 crc error
```

## 测试模式

rwcheck支持3种测试模式，分别是**AUTO模式**，**UP模式**，**SAME模式**

***不同测试模式下，测试文件大小会不一样***，测试文件大小策略可见函数 **get_test_size()** 。

| 模式 | 选项 | 特点 |
| :---: | :--- | :--- |
| AUTO | (default) | 每次测试取当前剩余空间的一半做测试文件大小 |
| UP | -b/-e | 从设定的begin开始到设定的end结束，文件大小以2的倍数递增 |
| SAME | -s | 固定每个测试文件的大小 |

### AUTO模式

默认为 **auto模式**，在此模式下，每次测试的文件大小为剩余分区空间的 **50%**（最大不允许超过4G），直至测试文件大小 **小于64K** 或 **剩余空间达到了设置的百分比**。

因此，** auto模式 ** 也可以说是二分递减模式

例如，当前剩余空间为64M，测试百分比设置为95%，则创建的文件大小依次为：

```
32M -> 16M -> 8M -> 4M -> 1M
```

在最后一次测试中，由于剩余空间不足，因此跳过2M，直接测试1M大小文件

### UP模式

与 **auto模式** 相反，**up模式** 是以2的倍数递增，通过选项```-b <开始大小>```和```-e <结束大小>```设置递增的范围，直至 **剩余空间达到了设置的百分比** 。

如果达到了设置的结束大小，但还有充足剩余空间，则从设置的开始大小重新循环。

例如，当前剩余空间为64M，测试百分比设置为95%，开始大小设置为128K，则创建的文件大小为：

```
128K -> 256K -> 512K -> 1M -> 2M -> 4M -> 8M -> 16M -> 128K -> 256K -> ...
```

### SAME模式

**same模式** 是固定测试文件大小，通过选项```-s <文件大小>```设置测试文件大小，循环创建文件直至 **剩余空间达到了设置的百分比** 或 **没有充足空间继续创建 **

例如，当前剩余空间为64M，测试百分比设置为95%，设置固定大小为5M，则创建的文件大小为：

```
5M -> 5M -> 5M -> 5M -> 5M -> 5M -> 5M -> 5M -> 5M -> 5M -> 5M -> 5M
```
