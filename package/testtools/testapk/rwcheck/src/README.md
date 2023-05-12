# read/write check

A tool to do read/write test for embedded devices.

[显示中文介绍(Chinese README)](https://github.com/gmpy/rwcheck/blob/master/README.zh.md)

## What's rwcheck

As it's name, rwcheck is a tool to do read/write check.  Where do it work? When do it work? And how do it work?

rwcheck is designed for embedded devices, to take a storage smoke test. Make sure that the storage is strong enough under much I/O test. Besides, rwcheck can also be used to do read/write power-failures testing, random power-off during reading and writing, becuase it will check the files, created before power failure.

It has the following features:

* Multiple test modes, covering test files of various sizes
* Read and write random data, covering various data combinations
* CRC check data, the error immediately exits and retains the problem site
* Support reading and writing power failure, verify historical data after restart
* Dynamically obtain system information, limit the total test size according to the storage space usage ratio
* Support multi-threaded write, simulate real-life scenarios

## How to begin

### Download and Compile

```
git clone https://github.com/gmpy/rwcheck.git rwcheck && make -C rwcheck
```

### Usage

You can get the follow message by **rwcheck -h**

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

For example:

```
rwcheck -d /mnt/UDISK -b 128k -p 90 -j 2 -t 10000000
```

It means that:

* Test on **/mnt/UDISK** directory
* **UP Mode**，test starts with **128K** file size, using the default maximum file size(16G)
* The total test size is 90% of total capacity
* **2 threads** write at the same time
* Ring test **10000000 times**

### One cycle

```-t <number>``` can specify the number of loop tests, then, how is one cycle?

Each cycle consists of 3 steps:

1. Write: Press [Different Mode](#test-mode) to create multiple files, create a loop until the space usage reaches the set percentage
2. Check: Read each file, check the file CRC value *(Allow the last file CRC check value is wrong)*
3. Delete: delete the test file

```
Q: Why is the last file CRC checksum error allowed?
A: Because the last file is written incomplete under random power failure, the check value will be inaccurate, but in fact, this is not an error.

Q: Why do it sometimes skip to the verification step without writing any files?
A: When it is checked that the existing file exists, the write step will be skipped and the verification step will be entered to check the file created during the last execution.
```

### Random data

The data written by rwcheck is random data obtained from ```/dev/urandom```, ensuring that the test covers as many data combinations as possible. However, there is also a problem. When there is an error in data verification, there is no comparison analysis of the original data. What should rwcheck do?

Rwcheck does this:

1. Read a random amount of random data from ```/dev/urandom``` to **rwcheck.org** before each loop starts.
2. **rwcheck.org** as a data source, loop reading from **rwcheck.org** to get enough data to write to the test file
3. After the test loop, delete the old **rwcheck.org** file so that the next loop creates a new random data source

Since **once the error rwcheck will exit immediately to retain the live**, you can analyze the error by comparing **rwcheck.org** with the error file.

### Test log

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

Once an error occurs, a similar error message will be printed:

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

## Test mode

Rwcheck supports 3 test modes, namely **AUTO mode**, **UP mode**, **SAME mode**

***In different test modes, the test file size will be different***, test file size policy visible function **get_test_size()**.

| Mode | Options | Features |
| :---: | :--- | :--- |
| AUTO | (default) | Test the file size by half of the current remaining space for each test |
| UP | -b/-e | From the set begin to the end of the set end, the file size is incremented by 2 |
| SAME | -s | Fix the size of each test file |

### AUTO mode

The default is **auto mode**, in which the file size for each test is **50%** of the remaining partition space (maximum of 4G is not allowed) until the test file size** is less than 64K** or **the remaining space has reached the set percentage**.

Therefore, **auto mode ** can also be said to be a dichot mode

For example, if the current free space is 64M and the test percentage is set to 95%, the file size created is:

```
32M -> 16M -> 8M -> 4M -> 1M
```

In the last test, due to insufficient space, skip 2M and test 1M size files directly.

### UP mode

Contrary to **auto mode**, **up mode** is incremented by a multiple of 2, with options ```-b <start size>``` and ```-e <end size>```. Increment the range until **the remaining space reaches the set percentage**.

If the set end size is reached, but there is enough free space, re-circulate from the set begin size.

For example, if the current free space is 64M, the test percentage is set to 95%, and the starting size is set to 128K, the file size created is:

```
128K -> 256K -> 512K -> 1M -> 2M -> 4M -> 8M -> 16M -> 128K -> 256K -> ...
```

### SAME mode

**same mode** is a fixed test file size, set the test file size by the option ```-s <file size>```, create a file cyclically until **the remaining space reaches the set percentage** or **no sufficient space to continue to create**

For example, if the current free space is 64M, the test percentage is set to 95%, and the fixed size is set to 5M, the created file size is:

```
5M -> 5M -> 5M -> 5M -> 5M -> 5M -> 5M -> 5M -> 5M -> 5M -> 5M -> 5M
```
