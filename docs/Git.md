# Git简明教程
# By 徐日

***

**目录**

1 安装Git

- 1.1 从源代码安装
- 1.2 Linux包管理工具安装

2 配置Git

- 2.1 配置用户名及邮箱
- 2.2 常用文本编辑器
- 2.3 差异分析工具
- 2.4 查看配置信息
- 2.5 help

3 Git基础

- 3.1 创建Git仓库
    * 3.1.1 将当前目录转化为Git仓库
    * 3.1.2 从远程仓库clone到本地仓库
- 3.2 文件操作
    * 3.2.1 查看当前文件状态
    * 3.2.2 跟踪文件
    * 3.2.3 忽略文件
    * 3.2.4 diff命令
    * 3.2.5 提交文件
    * 3.2.6 移除文件
    * 3.2.7 文件改名
    * 3.2.8 查看提交历史
    * 3.2.9 取消变更

4 远程仓库

- 4.1 克隆远程仓库
- 4.2 添加远程仓库
- 4.3 推送数据到远程仓库
- 4.4 远程仓库的删除与重命名

5 标签

- 5.1 查看和新建标签
- 5.2 后期加注标签

6 分支

- 6.1 创建、切换和删除分支
- 6.2 合并分支
- 6.3 推送与获取
- 6.4 分支管理
    * 6.4.1 长期分支
    * 6.4.2 bug 分支
    * 6.4.3 多人协作

***
## 1 安装Git

***

### 1.1 从源代码安装

***

首先安装依赖工具：

    $ yum install curl-devel expat-devel gettext-devel \

    openssl-devel zlib-devel

    $ apt-get install curl-devel expat-devel gettext-devel \

    openssl-devel zlib-devel
然后从Git官网下载最新版本源代码：
 [Git](http://git-scm.com/download)
编译安装：

    $ tar -zxf git-1.6.0.5.tar.gz

    $ cd git-1.6.0.5

    $ make prefix=/usr/local all

    $ sudo make prefix=/usr/local install

### 1.2 Linux包管理工具安装

***

Fedora上用yum安装:

    $ yum install git-core

Ubuntu等Debian体系的系统上，用apt-get安装：

    $ apt-get instal git-core

## 2 配置Git

***

### 2.1 配置用户名及邮箱

***

配置您的名字和邮箱，这里用户名为 *John Doe* ，

邮箱为 *johndoe@example.com* 。

    $ git config --global user.name "John Doe"

    $ git config --global user.email johndoe@example.com

### 2.2 常用文本编辑器

***

选择Git默认的文本编辑器，方便输入信息，

这里编辑器设为emacs。

    $ git config --global core.editor emacs

### 2.3 差异分析工具

***

在Git中常常会进行冲突合并，这时就要采用差异分析工具，

这里用到的是 vimdiff

    $ git config --global merge.tool vimdiff

### 2.4 查看配置信息

***

在进行了上述操作，可以打印出配置信息查看是否配置成功，

使用 *git config --list* 命令。

    $ git config --list

    user.name=Scott Chacon

    user.email=schacon@gmail.com

    color.status=auto

    color.branch=auto

    color.interactive=auto

    color.diff=auto

    ...

### 2.5 help

***

如果不了解某个操作的具体命令，可以输入：

    $ git help   <  verb  >

    $ git  <  verb  >  --help

    $ man git-< verb >

进行命令的查询。

## 3 Git 基础

***

### 3.1 创建Git仓库

***

#### 3.1.1 将当前目录转化为Git仓库

对现有的某个项目进行Git管理，那么Git的操作十分简单，只需

进入该项目的目录，执行：

    $git init

该目录就转化为Git仓库，但现在仓库里还没开始跟踪任何一个文件

如果想将某个文件纳入版本控制，只需执行：

    $git add *.c
    $git add README
    $git commit -m "初始化版本控制"

上面的命令就将当前目录下所有的C文件和README文件纳入版本控制

提交到了暂存区，并一起提交到了版本库。

#### 3.1.2 从远程仓库clone到本地仓库

如果您在Github上已经有了项目的版本库，或者工作需要很多人一起进行

项目的版本控制，那么从远程库上直接克隆将是最快捷最正确的方式：

首先登录你的Github账号，找到需要克隆到本地的项目，点击 *Clone or download*

选择 *Clone with SSH* 获得SSH方式下载地址，如图： [Github](https://github.com/CrazyCatJack/gitskills)

    $ git clone git@github.com:CrazyCatJack/gitskills.git

上面这条语句就是从我的Github账号上克隆gitskills仓库到本地，

你会发现在本地会多了一个名为gitskills的目录，里面有一个.git的目录和Github上

已克隆项目的全部文件，这时就可以工作了。当然，你也可以自定义库存在目录的名称：

    $ git clone git@github.com:CrazyCatJack/gitskills.git yourname

### 3.2 文件操作

***

#### 3.2.1 查看当前文件状态

***

通过 git status 命令查看当前项目文件的状态：

    $ git status

    On branch master

    nothing to commit (working directory clean)

这表示该工作目录很干净，没有未提交的更改。如果进行了提交暂存区但没有commit，

或者没有提交暂存区但修改了文件（修改文件内容、新建或删除文件）。通过git status命令

我们就能知道项目中有哪些文件还未提交。

    $ vim README

    $ git status

    On branch master

    Untracked files:

    (use "git add <file>..." to include in what will be committed)

    README

    nothing added to commit but untracked files present (use "git add" to track)
#### 3.2.2 跟踪文件

***

使用命令 git add 可以跟踪一个新文件

    $git add README

上例将README文件跟踪，相当于提交到暂存区。

我们用 git status 可以查到当前项目的文件状态

    $ git status

    On branch master

    Changes to be committed:

    (use "git reset HEAD <file>..." to unstage)

    new file:

    README

可以看到git提示 README是一个新文件并提交到了暂存区，等待commit。

需要注意的是，如果新创建了文件，或者将已经提交的文件进行修改，那么

**需要重新对有变更的文件进行git add 和 git commit**。

#### 3.2.3 忽略文件

***

在工程项目中，无论是编译内核还是编译根文件系统，编译bootloader

都会产生很多中间文件，比 如 *.o  *.a等。因此为了避免不必要的麻烦，保持工作区的“干净”

有时我们会忽略这些中间文件，只提交最终生成的文件。

    $ cat .gitignore

    *.[oa]

    *~

这里我们创建一个名为 .gitignore的文件，列出了要忽略的文件格式：所有.o和.a后缀的文件，所有

副本文件都将被git忽略。

文件 .gitignore 的格式规范如下:

• 所有空行或者以注释符号 # 开头的行都会被 Git 忽略。

• 可以使用标准的 glob 模式匹配。

• 匹配模式最后跟反斜杠(/)说明要忽略的是目录。

• 要忽略指定模式以外的文件或目录,可以在模式前加上惊叹号(!)取反。

这里解释一下glob模式：（*）匹配零个或多个任意字符；[ abc ] 匹配任意一个在括号里的字符；

（？）匹配任意一个字符；[0-9]表示匹配0到9范围内的任意一个字符。

这里给出了部分例子和注释：

    此为注释 – #将被 Git 忽略

    *.a           # 忽略所有 .a 结尾的文件
    !lib.a        # 但 lib.a 除外

    /TODO     # 仅仅忽略项目根目录下的 TODO 文件,不包括 subdir/TODO

    build/       # 忽略 build/ 目录下的所有文件

    doc/*.txt  # 会忽略 doc/notes.txt 但不包括 doc/server/arch.txt
#### 3.2.4 diff命令

***

如果想查看已暂存但未提交的文件发生了哪些更改，可直接输入 git diff命令

    diff --git a/addfile.c b/addfile.c

    index 751cc4a..922af89 100644

    --- a/addfile.c

    +++ b/addfile.c

    @@ -1 +1,5 @@

     #include<iostream>

    +int main()

    +{

    +return 0;

    +}

该命令比较的是工作区当前文件和暂存区快照的差异,上述例子就是addfile.c文件已经add到暂存区后

再对其内容进行修改，增添的内容会用“+”表示出来。所以你应该能够猜到原来的文件是只有头文件，更改后

增加了主函数吧。

另一种情况是，你想查看这次暂存的文件和已经提交的文件的差别，那么需要键入git diff --cached 以查看情况：

    xuri@Ubuntu-XuRi:~/gitskills$ git diff --cached

    diff --git a/addfile.c b/addfile.c

    index 922af89..3b5ad8c 100644

    --- a/addfile.c

    +++ b/addfile.c

    @@ -1,4 +1,4 @@

    -#include<iostream>

    +#include<stdio.h>

    int main()
    {
    return 0;
    }
可以得知，我修改了头文件，在上一次提交中，我使用的是 #include< iostream > 头文件，在这次我提交到

缓存区的addfile.c文件中，我将头文件改为了 #include < stdio.h > 。

#### 3.2.5 提交文件

***

当我们的更新后的文件已全部add到暂存库，一切准备就绪，那么我们就可以进行提交了。

输入git commit -m "对本次提交的注释" 即可进行提交

    xuri@Ubuntu-XuRi:~/gitskills$ git commit -m "修改了文件addfile.c"

    [master 3b33dd3] 修改了文件addfile.c

     1 file changed, 3 insertions(+), 2 deletions(-)
可以看到，git提示，有一个文件发生了改变，修改了addfile.c文件

这时，我们再看一下工作区是否干净：

    xuri@Ubuntu-XuRi:~/gitskills$ git status

    位于分支 master

    您的分支领先 'origin/master' 共 2 个提交。

      （使用 "git push" 来发布您的本地提交）

    无文件要提交，干净的工作区

这里我们看到系统提示是干净的工作区，又因为我的这个目录是连接到Github上的远程仓库，所以这里

git提示我的分支领先远程仓库两个提交。这部分内容我后面会详细介绍。

#### 3.2.6 移除文件

***

首先，如果仅仅是简单地用rm指令移除已经跟踪的文件，则工作区是不干净的，也就是说，这属于一种文件更新

    xuri@Ubuntu-XuRi:~/gitskills$ ls
    addfile.c  README.md  xuri.c
    xuri@Ubuntu-XuRi:~/gitskills$ rm README.md
    xuri@Ubuntu-XuRi:~/gitskills$ ls
    addfile.c  xuri.c
    xuri@Ubuntu-XuRi:~/gitskills$ git status
    位于分支 master
    您的分支领先 'origin/master' 共 2 个提交。
    （使用 "git push" 来发布您的本地提交）
    尚未暂存以备提交的变更：
    （使用 "git add/rm <文件>..." 更新要提交的内容）
    （使用 "git checkout -- <文件>..." 丢弃工作区的改动）
    删除:         README.md
    修改尚未加入提交（使用 "git add" 和/或 "git commit -a"）
可以看到Git提示我们这种文件更新需要提交，或者我们取消这种提交。

    xuri@Ubuntu-XuRi:~/gitskills$ git commit -m "删除README.MD文件"
    [master c4cea03] 删除README.MD文件
     1 file changed, 4 deletions(-)
     delete mode 100644 README.md
可以看到我们已经提交了这次文件更新。如果我们想要移除跟踪但不删除文件，可以用 *git rm --cached* 文件名

指令来移除跟踪。并在.gitignore文件中声明此文件，则git将忽略这一文件。

如果我们想递归的删除某个目录下的所有文件，可以用*git rm 目录名/\*.c *这条指令意为递归的删除此目录下的所有

C文件。同时，也可以不指定目录，直接递归删除当前目录下所有的指定文件。eg: *git rm \*~* 这条命令会递归删除当

前目录下所有~结尾的文件。

#### 3.2.7 文件改名

***

在git下，改名非常容易，只需用*git mv name1 name2*命令即可实现：

    xuri@Ubuntu-XuRi:~/gitskills$ git mv addfile.c addfile2.c

    xuri@Ubuntu-XuRi:~/gitskills$ git status

    位于分支 master

    您的分支领先 'origin/master' 共 3 个提交。
    （使用 "git push" 来发布您的本地提交）

    要提交的变更：

    （使用 "git reset HEAD <文件>..." 以取消暂存）

    重命名:       addfile.c -> addfile2.c
可以看到，git跟踪到了这次重命名，并且提示这次变更需要提交。

#### 3.2.8 查看提交历史

***

在git中，我们可以不但可以查阅当前仓库的所有提交信息，还能够查到有关分支合并，处理冲突的信息。

这些都基于*git log*指令。

    xuri@Ubuntu-XuRi:~/gitskills$ git log

    commit c4cea037ed4cf2c15393362759e23864ddb7aad2

    Author: xuri <xuri@allwinnertech.com>

    Date:   Mon Mar 13 09:08:05 2017 +0800

    删除README.MD文件

    commit 3b33dd3632ccfa7bc3395e33c1a6e4c37214f4a3

    Author: xuri <xuri@allwinnertech.com>

    Date:   Fri Mar 10 17:35:56 2017 +0800

    修改了文件addfile.c

    commit afe63dbeace7dcbc89ea7a8a16d5d75bb0a9fd96

    Author: xuri <xuri@allwinnertech.com>

    Date:   Fri Mar 10 16:32:42 2017 +0800

    double yinhao

    commit eb2ddb21a8dc23d2947d3e95a85443fbbcc96ef1

    Merge: d19d73d 0a14898
    Author: xuri <xuri@allwinnertech.com>

    Date:   Tue Mar 7 14:59:00 2017 +0800

上面是*git log*指令的执行结果，可以看到以往提交的历史，包含完整的哈希字串，合并分支，作者，提交日期以及

当初提交时填写的提交信息。但是这样看起来还是有些麻烦。而且合并分支的过程不是很明显。我们可以用 --pretty 来

让log指定格式显示，还可以用 graph显示合并过程。

    xuri@Ubuntu-XuRi:~/gitskills$ git log --pretty=format:"%h %s" --graph

    * c4cea03 删除README.MD文件

    * 3b33dd3 修改了文件addfile.c

    * afe63db double yinhao

    *   eb2ddb2 merged bug fix 101
    |\
    | * 0a14898 fix bug 101
    |/
    *   d19d73d merge with no-ff
    |\
    | * 9f7ccc3 no-ff merge mode
    |/
    *   4445115 conflict fixed
    |\
    | * 2469543 AND simple
    * | 4e89f0d & simple
    |/

    * ca44755 add branchtest words

    * a8b200c add xuri.c

    * 7bd3450 Initial commit

上面的 --pretty 指出了log的输出格式，这里对其他输出格式做出列表：

    %H 提交对象(commit)的完整哈希字串

    %h 提交对象的简短哈希字串

    %T 树对象(tree)的完整哈希字串

    %t 树对象的简短哈希字串

    %P 父对象(parent)的完整哈希字串

    %p 父对象的简短哈希字串

    %an 作者(author)的名字

    %ae 作者的电子邮件地址

    %ad 作者修订日期(可以用 -date= 选项定制格式)

    %ar 作者修订日期,按多久以前的方式显示

    %cn 提交者(committer)的名字

    %ce 提交者的电子邮件地址

    %cd 提交日期

    %cr 提交日期,按多久以前的方式显示

    %s 提交说明
#### 3.2.9 取消变更

***

如果修改了工作区被跟踪的文件，且已经提交到暂存区。但是想撤回这一提交怎么办呢？其实我们跟着命令走就好，

因为Git十分的智能。这里我们更改了addfile2.c文件，并且提交到暂存区。这时我们用*git status*命令去查看工作区

状态：

    xuri@Ubuntu-XuRi:~/gitskills$ git status

    位于分支 master

    您的分支领先 'origin/master' 共 4 个提交。

      （使用 "git push" 来发布您的本地提交）


    要提交的变更：

      （使用 "git reset HEAD <文件>..." 以取消暂存）

    	修改:         addfile2.c

可以看到git提示，如果对于addfile2.c的文件修改需要取消暂存，那么使用*got reset HEAD *命令

    xuri@Ubuntu-XuRi:~/gitskills$ git reset HEAD addfile2.c

    重置后取消暂存的变更：

    M	addfile2.c

    xuri@Ubuntu-XuRi:~/gitskills$ git status

    位于分支 master

    您的分支领先 'origin/master' 共 4 个提交。

      （使用 "git push" 来发布您的本地提交）

    尚未暂存以备提交的变更：

      （使用 "git add <文件>..." 更新要提交的内容）

      （使用 "git checkout -- <文件>..." 丢弃工作区的改动）

    	修改:         addfile2.c

    修改尚未加入提交（使用 "git add" 和/或 "git commit -a"）
可以看到在执行命令后，文件取消暂存，git提示需要对文件进行add和commit。

    xuri@Ubuntu-XuRi:~/gitskills$ git checkout -- addfile2.c

    xuri@Ubuntu-XuRi:~/gitskills$ git status

    位于分支 master

    您的分支领先 'origin/master' 共 4 个提交。

      （使用 "git push" 来发布您的本地提交）
    无文件要提交，干净的工作区
当我们执行*git checkout --*后，就取消了对文件的修改。文件回到修改前的状态。刺客查看工作区，发现是干净的。

## 4 远程仓库

***

### 4.1 克隆远程仓库

***

首先，如果在Github 或者 Gitlab上有自己的项目仓库，则可以同步到本地，这里我们用到的是*git clone*指令

    xuri@Ubuntu-XuRi:~$ git clone git@github.com:CrazyCatJack/gitskills.git clonegit正克隆到 'clonegit'...

    remote: Counting objects: 29, done.

    remote: Compressing objects: 100% (20/20), done.

    remote: Total 29 (delta 8), reused 24 (delta 6), pack-reused 0

    接收对象中: 100% (29/29), done.

    处理 delta 中: 100% (8/8), done.

    检查连接... 完成。

可以看到，这里从Github上克隆了一个仓库下来，并且可以指定本地仓库的目录，这里我采用了clonegit作为本地仓库

的存储目录。

### 4.2 添加远程仓库

***

我们也可以添加一个新的远程仓库，并指定新仓库的名称。执行*git remote add [shortname] [url]* 来创建新仓库。

    xuri@Ubuntu-XuRi:~/clonegit$git remote add lg git@github.com:CrazyCatJack/learngit.git

    xuri@Ubuntu-XuRi:~/clonegit$ git remote -v

    lg	git@github.com:CrazyCatJack/learngit.git (fetch)

    lg	git@github.com:CrazyCatJack/learngit.git (push)

    origin	git@github.com:CrazyCatJack/gitskills.git (fetch)

    origin	git@github.com:CrazyCatJack/gitskills.git (push)

这里我们创建了一个新仓库lg，它来源于Github上的另一个项目仓库：learngit。同样，我们也可以把learngit上的分支和

文件抓取下来，这就要用到*git fetch <仓库名>*

    xuri@Ubuntu-XuRi:~/xuri_git$ git fetch origin2

    warning: no common commits

    remote: Counting objects: 29, done.

    remote: Compressing objects: 100% (20/20), done.

    remote: Total 29 (delta 8), reused 24 (delta 6), pack-reused 0

    Unpacking objects: 100% (29/29), done.

    来自 github.com:CrazyCatJack/gitskills
     * [新分支]          dev        -> origin2/dev

     * [新分支]          master     -> origin2/master

     * [新标签]          v1.2       -> v1.2

     * [新标签]          v0.8       -> v0.8

这里我们是将仓库origin2上的分支和文件同步到本地仓库。现在我们就可以更改这些文件和分支，并进行提交了。

### 4.3 推送数据到远程仓库

***

当我们在本地仓库进行了文件的修改，并最终决定可以提交的时候。我们就可以通过push指令，将本地仓库的分支提交

到远程仓库。

    xuri@Ubuntu-XuRi:~/xuri_git$ git checkout master
    之前的 HEAD 位置是 8ec153d... boy2 commit boy2.c
    切换到分支 'master'
    您的分支与上游分支 'origin/master' 一致。
    xuri@Ubuntu-XuRi:~/xuri_git$ ls
    helloworld.c  LICENSE  readme.txt
    xuri@Ubuntu-XuRi:~/xuri_git$ git push origin master
    Everything up-to-date

可以看到，在这个例子中，我将master分支提交到了origin远程仓库。

查看远程仓库的信息也可以用*git remote show <仓库名>*来实现：

    xuri@Ubuntu-XuRi:~/xuri_git$ git remote show origin

    * 远程 origin

      获取地址：git@github.com:CrazyCatJack/learngit.git

      推送地址：git@github.com:CrazyCatJack/learngit.git

      HEAD 分支：master

      远程分支：

        master 已跟踪

      为 'git pull' 配置的本地分支：

        master 与远程 master 合并

      为 'git push' 配置的本地引用：

        master 推送至 master (最新)

### 4.4 远程仓库的删除与重命名

***

我们可以用*git remote rename*命令修改远程仓库的名称。

    xuri@Ubuntu-XuRi:~/xuri_git$ git remote

    origin

    origin2

    xuri@Ubuntu-XuRi:~/xuri_git$ git remote rename origin2 or

    xuri@Ubuntu-XuRi:~/xuri_git$ git remote

    or

    origin

如上，将origin2仓库重命名为or。同时，我们也可以移除远程仓库：

    xuri@Ubuntu-XuRi:~/xuri_git$ git remote

    or

    origin

    xuri@Ubuntu-XuRi:~/xuri_git$ git remote rm or

    xuri@Ubuntu-XuRi:~/xuri_git$ git remote

    origin

这里，我们将远程仓库or删除，只剩下origin仓库。

## 5 标签

***

### 5.1 查看和新建标签

***

首先我们来看一下如何查看已经打好的标签。通过*git tag*命令就可以轻松的查看了：

    xuri@Ubuntu-XuRi:~/xuri_git$ git tag

    v0.8

    v1.2
这里能够看到我之前是给两次提交打了标签。如果你只对特定的标签感兴趣，可以这样执行：

    xuri@Ubuntu-XuRi:~/xuri_git$ git tag -l v1.*

    v1.2

就可以查到符合要求的标签了，这里是列出所有1.*的标签。下面我们来看两种新建标签的方法。

首先，可以创建一个含附注类型的标签，并用*-a*指定标签的名字：

    xuri@Ubuntu-XuRi:~/xuri_git$ git tag -a v1.6 -m "my v1.6 version"

即使将当前最近的一次提交打标签为v1.6版本。同时我们也能查看每个标签的具体信息：

    xuri@Ubuntu-XuRi:~/xuri_git$ git show v1.6

    tag v1.6

    Tagger: xuri <xuri@allwinnertech.com>

    Date:   Mon Mar 13 14:42:02 2017 +0800

    my v1.6 version

    commit 23fd13c166bf85285f81fd69863e32d98210a5df

    Author: xuri <xuri@allwinnertech.com>

    Date:   Tue Mar 7 11:28:44 2017 +0800

        add helloworld.c file

然后，我们也可以创建一个轻量级的标签，直接给出标签名即可。

    xuri@Ubuntu-XuRi:~/xuri_git$ git tag v1.8

    xuri@Ubuntu-XuRi:~/xuri_git$ git tag

    v0.8

    v1.2

    v1.6

    v1.8
### 5.2 后期加注标签

***

如果当初忘记给某次提交加标签，没有关系。我们可以查看提交历史，然后给那次提交打上标签。首先我们查看以往的

提交历史：

    xuri@Ubuntu-XuRi:~/xuri_git$ git log --pretty=oneline

    23fd13c166bf85285f81fd69863e32d98210a5df add helloworld.c file

    cf65c31294c72523b51c82f1497ee6757241d6f7 rmove test.txt

    63a23baaa51b405a974a8ba297bb6b46c432d286 add test.txt

    b16e8a0d3155d8f7eef2ef4db4257acb68af46fc add 'of files'

    ab72e32628dbbf0cfca4c3cf05c009e747723542 git tracks changes

    1feddd387a01a1ea66f4697b4588554e94b445ef understand how stage works

    7f8d525e642dc734856036622ca20b8b0447795e append GPL

    7d3ef5c14ab8de9f7e9580e1680a99b00eb80f65 add distributed

    9f43349582c2b4dc03f2519a880bd21893696b29 wrote a readme file

下面通过*git tag*命令直接把第一次提交打标签为v0.6 。

    xuri@Ubuntu-XuRi:~/xuri_git$ git tag -a v0.6 9f4334

    xuri@Ubuntu-XuRi:~/xuri_git$ git tag

    v0.6

    v0.8

    v1.2

    v1.6

    v1.8
如果想要推送标签的话，可以用*git push origin <标签号>*来实现推送某一个标签，如果想要把更新后的所有标签推送到远程仓库的话，可以用*git push origin --tags*实现。
    xuri@Ubuntu-XuRi:~/xuri_git$ git push origin v0.6

    Counting objects: 1, done.

    Writing objects: 100% (1/1), 187 bytes | 0 bytes/s, done.

    Total 1 (delta 0), reused 0 (delta 0)

    To git@github.com:CrazyCatJack/learngit.git

     * [new tag]         v0.6 -> v0.6

    xuri@Ubuntu-XuRi:~/xuri_git$ git push origin --tags

    Counting objects: 29, done.

    Delta compression using up to 4 threads.

    Compressing objects: 100% (26/26), done.

    Writing objects: 100% (28/28), 2.64 KiB | 0 bytes/s, done.

    Total 28 (delta 7), reused 0 (delta 0)

    remote: Resolving deltas: 100% (7/7), done.

    To git@github.com:CrazyCatJack/learngit.git

     * [new tag]         v0.8 -> v0.8

     * [new tag]         v1.2 -> v1.2

     * [new tag]         v1.6 -> v1.6

     * [new tag]         v1.8 -> v1.8

## 6 分支

***

### 6.1 创建、切换和删除分支

***

首先，如果你想创建一个分支，可以使用*git branch <branch name>*命令实现。举例说明：

    xuri@Ubuntu-XuRi:~/xuri_git$ git branch xuri

    xuri@Ubuntu-XuRi:~/xuri_git$ git branch

      dev

    * master

      xuri
这就创建了名称为"徐日"的分支。而查看当前仓库的所有分支和当前所在分支则输入*git branch*即可。上例说明我们

目前在master分支。如果我们想切换到dev分支，那么就需要*git checkout <branch name>*命令来实现：

    xuri@Ubuntu-XuRi:~/xuri_git$ git checkout dev

    切换到分支 'dev'。如果想创建分支并且直接切换到此分支，则我们可以使用*git checkout -b <branch name>*命令实

现：

    xuri@Ubuntu-XuRi:~/xuri_git$ git checkout -b quick

    切换到一个新分支 'quick'

    xuri@Ubuntu-XuRi:~/xuri_git$ git branch

      dev

      master

    * quick

      xuri

上例实现了创建并同时切换到quick分支。那么，如果我们想创建一个全新的无log记录无文件的空白分支，我们应该怎么做呢？有两种方法能够实现：
1. 使用symbolic-ref
```
git symbolic-ref HEAD refs/heads/<newbranchname>
rm .git/index
git clean -fdx
<do some change to commit>
git add <your change>
git commit -s
```
这里我们只有做出修改，并最终提交后才能在*git branch*命令下看到我们创建的全新分支。
2. 使用 --orphan
```
git checkout --orphan <newbranchname>
git rm -rf .
rm .gitignore
<do some change to commit>
git add <your change>
git commit -s
```
与第一种方法类似，在作出修改并最终提交后才能在*git branch*命令下看到新分支。

如果我们想删除quick分支，我们可以先切换回master分支，然后执行*git branch -d <branch name>*就可完成对指定分支的删除。

    xuri@Ubuntu-XuRi:~/xuri_git$ git branch -d quick

    已删除分支 quick（曾为 3a9bad1）。

### 6.2 合并分支

***

首先，我们在分支quick上对"helloworld.c"文件进行修改，并对其进行提交。

    xuri@Ubuntu-XuRi:~/xuri_git$ git status

    位于分支 quick

    尚未暂存以备提交的变更：

      （使用 "git add <文件>..." 更新要提交的内容）

      （使用 "git checkout -- <文件>..." 丢弃工作区的改动）

    	修改:         helloworld.c

    修改尚未加入提交（使用 "git add" 和/或 "git commit -a"）

    xuri@Ubuntu-XuRi:~/xuri_git$ git commit -a -m "modify helloworld.c file."

    [quick 3a9bad1] modify helloworld.c file.

     1 file changed, 1 insertion(+)

然后切换回主分支master，合并分支quick到master上。

    xuri@Ubuntu-XuRi:~/xuri_git$ git checkout master

    切换到分支 'master'

    您的分支与上游分支 'origin/master' 一致。

    xuri@Ubuntu-XuRi:~/xuri_git$ git merge quick

    更新 23fd13c..3a9bad1

    Fast-forward

     helloworld.c | 1 +

     1 file changed, 1 insertion(+)

在合并完分支后，如果用不到分支quick，可将其删除，删除分支见6.1节。

然而，并不是每次合并都如这次一般顺利，因为当你在当前分支对文件进行修改的同时，可能忽略了你在其他分支的修

改。当你在其他分支对某个文件修改并提交分支后，切换到当前分支，再次对这个文件进行修改并提交，如果此时合并

两个分支，那么结果就会出现冲突。不过聪明的Git将冲突的部分写到了冲突文件中，只有我们将冲突修复，我们才能够

提交文件。

    xuri@Ubuntu-XuRi:~/xuri_git$ git merge xuri

    自动合并 helloworld.c

    冲突（内容）：合并冲突于 helloworld.c

    自动合并失败，修正冲突然后提交修正的结果。

    xuri@Ubuntu-XuRi:~/xuri_git$ git status

    位于分支 master

    您的分支领先 'origin/master' 共 3 个提交。

      （使用 "git push" 来发布您的本地提交）

    您有尚未合并的路径。

      （解决冲突并运行 "git commit"）

    未合并的路径：

      （使用 "git add <文件>..." 标记解决方案）

    	双方修改：     helloworld.c

    修改尚未加入提交（使用 "git add" 和/或 "git commit -a"）

上例就是当两个分支修改同一个文件并且提交的话，出现的冲突，git已经提示冲突来源于双方提交。不仅如此，git还帮

助我们把helloworld文件中有冲突的部分标记了出来，将别的小伙伴提交的内容一并显示在helloworld.c中。

    xuri@Ubuntu-XuRi:~/xuri_git$ cat helloworld.c

    #include<stdio.h>

    int

    main()

    {

    <<<<<<< HEAD

    	cout<<"hello linux ........."<<endl;

    	cout<<"suannihen"<<endl;

    =======

    	cout<<"hello world!"<<endl;

    	cout<<"I like linux device."<<endl;
    >>>>>>> xuri

    	return 0;

    }

可以看到，git已经标出了冲突的分支来源。那么我们只需要将自己和别人的提交合并更改。再次提交helloworld.c这个

冲突文件。我们的冲突就解决了，合并成功。而且，我们还可以查看分支合并的图形界面。

用*git log --pretty="%h %s"*查看分支合并过程：

    xuri@Ubuntu-XuRi:~/xuri_git$ git log --pretty="%h %s" --graph

    *   17d6bbb conflict fixed

    |\
    | * 1a370d6 I like linux device.

    * | 0639ee6 ....

    * | 92bcd1f hello linux ^_^

    * | 3a9bad1 modify helloworld.c file.

    |/
    * 23fd13c add helloworld.c file

当我们想要知道有哪些分支已经合并到了当前分支，那么我们需要执行*git branch --merged*

    xuri@Ubuntu-XuRi:~/xuri_git$ git branch --merged

      dev

    * master

      xuri


这里能够看到dev和xuri分支都被合并到了master分支上。或者*git branch --no-merged*来查看哪些分支没有被合并到

当前分支：

    xuri@Ubuntu-XuRi:~/xuri_git$ git branch --no-merged

    xuri@Ubuntu-XuRi:~/xuri_git$

所以这说明我们的所有分支都合并到了master分支上。而当我们使用完了临时分支，并且合并了分支后。可以选择删除

它:
    xuri@Ubuntu-XuRi:~/xuri_git$ git branch -d xuri

    已删除分支 xuri（曾为 1a370d6）。

这里能够看到xuri分支已被删除，而如果我们的分支没有被合并，但是仍然想删除的话，

那么需要执行*git branch -D < branchname >* 。

### 6.3 推送与获取

***

首先，如果你完成了某个项目的开发，决定上传某个分支到服务器的时候。可以使用push指令来上传指定的分支到指定

的远程仓库。

    xuri@Ubuntu-XuRi:~/xuri_git$ git push origin master

    Counting objects: 12, done.

    Delta compression using up to 4 threads.

    Compressing objects: 100% (6/6), done.

    Writing objects: 100% (6/6), 671 bytes | 0 bytes/s, done.

    Total 6 (delta 3), reused 0 (delta 0)

    remote: Resolving deltas: 100% (3/3), completed with 1 local objects.

    To git@github.com:CrazyCatJack/learngit.git

       22575e5..6ac5bb4  master -> master

上例就是将master分支上传到远程服务器origin的master分支上。

**这个操作有一个前提是本地的master分支和远程仓库的master必须建立对应关系。**当然，也有能够避免这种可能发

生错误的方法。采用*git push origin master:master*就可以避免这种不必要的错误。即将本地的master分支更新远程仓

库origin的master分支。

相对的，如果自己想获取此刻远程仓库和自己本地仓库不同的部分，可以执行*git fetch origin*命令实现同步。该命令

执行后，本地上的origin/master指针被更新。因此也就能够访问目前远程仓库的状况。

    xuri@Ubuntu-XuRi:~/xuri_git$ git fetch origin

    xuri@Ubuntu-XuRi:~/xuri_git$ git merge origin/master

    Merge made by the 'recursive' strategy.

     lllll | 1 +

     1 file changed, 1 insertion(+)

     create mode 100644 lllll

上例是在同步之后，进行远程分支与本地分支操作的演示。

### 6.4 分支管理

***

#### 6.4.1 长期分支

***

首先需要明确一个概念：并不是所有分支都要提交。也就是说，只提交重要的分支，比如master分支，dev分支。

master分支可以用来发布稳定的版本。dev分支可以用来当做下一版本的存储库。在dev分支上还可以另开启其他分支，

其他分支用来维护dev这个分支，当dev到一定程度，可以满足下一版本的各种要求时，就把dev分支归并到master分支

。**在这里要提到一点的是**：通常情况下，分支合并用的是快速合并方式。但在这种模式下，删除分支后会丢掉分支

信息。所以这里建议采用*git merge --no-ff -m "commit information" <  branch name  >*  这样合并就会在分支历史上

留下分支信息。

#### 6.4.2 bug 分支

***

当我们正进行某项工作，但是突然必须解决其他事情。比如你正在写一个项目文件，突然接到通知，马上修改一个bug

那么现在就需要我们保存工作空间，等修复完bug再恢复它，完成我们的工作。

这里我们用*git stash*命令存储当前工作区，等忙完了优先级高的事情再回来回复它：

    xuri@Ubuntu-XuRi:~/xuri_git$ git status

    位于分支 master

    您的分支与上游分支 'origin/master' 一致。

    要提交的变更：

      （使用 "git reset HEAD <文件>..." 以取消暂存）

    	新文件:       newproject

    xuri@Ubuntu-XuRi:~/xuri_git$ git stash

    Saved working directory and index state WIP on master: 6ac5bb4 delete

    HEAD 现在位于 6ac5bb4 delete

可以看到这里我们将现有的工作区保存了。那么当我们忙完了其他事情想要回来接着这个工作区工作的话，

用*git stash list*来查看有几个存储的工作区备份，用*git stash apply stash@{num}*来释放存储的工作区：

    xuri@Ubuntu-XuRi:~/xuri_git$ git stash list

    stash@{0}: WIP on master: 6ac5bb4 delete

    xuri@Ubuntu-XuRi:~/xuri_git$ git stash apply stash@{0}

    位于分支 master

    您的分支与上游分支 'origin/master' 一致。

    要提交的变更：

      （使用 "git reset HEAD <文件>..." 以取消暂存）

    	新文件:       newproject

而这种释放方法不会删除掉保存的工作区，我们可以用*git stash drop*删除掉。当然，如果我们用*git stash pop*恢复

工作区的话则能够直接把存储的工作区自动删除掉免去drop操作。

#### 6.4.3 多人协作

***

Git最主要的功能之一就是能够有多个人一起完成一项工作。但是这种方式也可能会出现冲突，因此这里给出解决方案

首先，如果远程仓库上的某个文件被其他人更新，而你又恰好也在修改这个文件并进行推送。那么这时就会出现推送

失败。这时就需要用*git pull*命令来抓取你提交的分支与远程仓库上对应分支的文件冲突。

    xuri@Ubuntu-XuRi:~/xuri_git$ git push origin master

    To git@github.com:CrazyCatJack/learngit.git

     ! [rejected]        master -> master (non-fast-forward)

    error: 无法推送一些引用到 'git@github.com:CrazyCatJack/learngit.git'

    提示：更新被拒绝，因为您当前分支的最新提交落后于其对应的远程分支。

    提示：再次推送前，先与远程变更合并（如 'git pull ...'）。详见

    提示：'git push --help' 中的 'Note about fast-forwards' 小节。

上例显示了无法推送的错误，因此用*git pull*抓取不同之处

    xuri@Ubuntu-XuRi:~/xuri_git$ git pull

    Merge made by the 'recursive' strategy.

     anotherfile.c | 1 +

     newfile.c     | 6 ++++++

     2 files changed, 7 insertions(+)

     create mode 100644 anotherfile.c

     create mode 100644 newfile.c

用git pull抓取该分支的不同之处到本地仓库，进行合并后再提交就可以了：

    xuri@Ubuntu-XuRi:~/xuri_git$ git commit -m "merge"

    [master 8055c20] merge

     1 file changed, 1 deletion(-)

     delete mode 100644 another
     xuri@Ubuntu-XuRi:~/xuri_git$ git push origin master

    Counting objects: 28, done.

    Delta compression using up to 4 threads.

    Compressing objects: 100% (21/21), done.

    Writing objects: 100% (22/22), 2.31 KiB | 0 bytes/s, done.

    Total 22 (delta 7), reused 0 (delta 0)

    remote: Resolving deltas: 100% (7/7), completed with 1 local objects.

    To git@github.com:CrazyCatJack/learngit.git

       95b66d1..8055c20  master -> master

可以看到我们这次推送成功了。
