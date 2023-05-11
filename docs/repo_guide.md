###目录
[TOC]

***
####一. repo简介

  *    repo是对分布式版本控制系统Git中部分命令的封装，是一个命令行工具。它可以管理多个git项目（例如，在Android 4.2中，就包含了329个项目，每一个项目都是一个独立的Git仓库。这意味着，如果我们要创建一个分支来做feature开发，那么就需要到每一个子项目去创建对应的分支。这显然不能手动地使用git到每一个子项目里面去创建分支，必须要采用一种自动化的方式来处理。这些自动化处理工作就是由repo工具来完成的，这也是repo出现的原因之一)。对于所有repo管理的项目，例如myrepoproject，其中myrepoproject/.repo是repo的目录，myrepoproject/中除了.repo之外的内容为repo工作目录，来自相应repo目录的某个版本。
  *    repo工具分为两个部分，一部分是repo引导脚本，另一部分是repo代码。repo引导脚本里面设置有一个指向repo代码的地址，通过执行repo init -u URL [OPTIONS] (如repo init -u http://gerrit.allwinnertech.com:8081/product/tina/manifest.git), 把repo工具本身的代码和tina源码仓库的一个清单版本库下载到当前工作目录下;repo init指令repo工具的每个指令都用一个python脚本来定义的，每执行一条指令，都通过repo引导将本找到相应的指令，然后执行。
  *    这里有一个概念，是清单版本库，简称清单库；伴随着还有另一个名词---清单文件。一个清单库可以包含多个清单文件和分支，每个清单文件和分支都有对应的版本，清单文件以xml格式组织。举个例子：
![](http://p1.bqimg.com/567571/4ee9f6a12d38c717.png)
		-	remote元素，定义了名为txxxxxa的远程仓库，其库的基址为git：//gerrit.xxxxxx.com:80
		-	default元素，设置各个项目默认远程版本库为txxxxxa，默认的的分支为txxxxxa-dev。当然各个项目(project元素)还可以定义自己的remote和revision覆盖默认的配置
		-	project元素，用于定义一个项目，path属性表示在工作区克隆的位置，name属性表示该项目的远程版本库的相对路径
		-	project元素的子元素copyfile，定义了项目克隆后的一个附件动作，从src拷贝文件到dest
  *    清单文件的作用：
		-	根据清单文件指定的地址一一克隆清单文件中的项目
		-	通过清单文件对所有的git仓库进行统一管理

***
####二.  repo使用流程
![](http://i1.piimg.com/567571/68d9cc2777516b70.png)

***
####三. repo常用命令

#####***3.1 repo init***
  *	命令格式：  $repo init -u URL [OPTIONS]
  *	选项：
  	-	-u : 制定一个URL，其链接到一个清单库
  	-	-m : 在清单库中选择一个xml清单文件
  	-	-b : 选在一个清单库中的一个分支
  *	说明： repo init命令主要完成两个操作：
   	-	利用repo引导脚本完成repo工具的完整下载
   	-	克隆清单库manifest.git（地址来自-u参数)到本地.repo/manifests，克隆清单文件到本地.repo/manifest.xml，它只是一个符号链接，链接到.repo/manifests/default.xml，如果manifests中有多个xml文件，repo init可任选其中一个，默认选择是default.xml
  * 举例：$repo init -u http: //gerrit.allwinnertech.com:8081/product/tina/manifest.git

#####***3.2 repo sync***
  *	命令格式： $repo sync [< project >...]
  *	选项：
  	-	-c : 指当前分支，可以加快下载速度
  *	说明：用于参照清单文件.repo/manifest.xml克隆并同步版本库。如果某个项目版本库不存在，则执行repo sync命令相当于执行git clone,如果项目版本库已经存在，则相当于执行下面的两条命令：
  	-	$ git remote update 相当于对每一个remote源执行了fetch操作。
  	-	$ [git rebase origin/branch](http://blog.chinaunix.net/uid-27714502-id-3436696.html) 针对当前分支的跟踪分支执行rebase操作
  *	举例：$repo sync

#####***3.3 repo start***
  *	命令格式： $ repo start <newbranchname> [--all | < project >...]
  * 说明： $ 刚克隆下来的代码是没有分支的，repo start实际上是对git checkout -b命令的封装。为指定的项目或所有项目(若使用--all参数)，以清单文件中设定的分支为基础，创建特性分支。这条指令与git checkout -b是有区别的，git checkout -b 是在当前所在的分支基础上创建特性分支，而repo start是在清单文件设定分支的基础上创建特性分支
  * 举例：$repo start stable platform/build platform/bionic (假设清单文件中设定的分支是gingerbread-exdroid-stable，那么执行以上指令就是对platform/build、platform/bionic项目，在gingerbread-exdroid-stable的基础上创建特性分支stable)

#####***3.4 repo checkout***
  *	命令格式：$ repo checkout < branchname > [< project >...]
  *	说明：此命令实际上是对git checkout命令的封装，但不能带-b参数，所以不能用此命令来创建特性分支；只能用来切换分支
  *	举例：$repo checkout crane-dev (在清单文件中的设定分支的基础上切换到crane-dev分支)

#####***3.5 repo branch***
  *	命令格式： $ repo branch [< project >...]
  *	说明： 此命令是用来查看分支的
  *	举例： $repo branch 或 $repo branch platform/build

#####***3.6 repo diff***
  *	命令格式：$repo diff [< project >...]
  *	说明：实际上是对git diff命令的封装，用于显示各个项目工作区下的文件差异
  *	举例：$repo diff product/tina/docs	查看docs下面工作区中文件的改变

#####***3.7 repo stage***
  *	命令格式：$repo stage -i [< project >...]
  *	说明：实际上是对git add --interactive命令的封装，用于挑选各个项目工作区中的改动以加入暂存区，其中 -i 代表git add --interactive命令中的--interactive，给出界面供用户选择使用
  *	举例：$repo stage -i product/tina/docs


#####***3.8 repo prune***
  *	命令格式：$repo prune [< project >...]
  *	说明：它是对git branch -d命令的封装，该命令用于扫描项目的各个分支，并删除已经向上游服务器合并的分支

#####***3.9repo abandon***
  *	命令格式：$repo abandon < branchname > [ < project >...]
  *	说明：它是对git branch -D命令的封装
  *	举例：$repo abandon stable

#####***3.10 status***
  * 命令格式：$repo status [< project >...]
  * 查看文件状态。同时显示暂存区的状态和本地文件修改的状态
  * 举例：$repo status product/tina/docs  显示如下：

	![](http://p1.bqimg.com/567571/1451a9f9ce665a25.png)

	以上的实例输出显示了product/docs项目分支的修改状态
  *	每个小节的首行显示项目名称，以及所在分支的名称
  *	第一个字母表示暂存区的文件修改状态
  	- -:没有改变
  	- A:添加（不在HEAD中，在暂存区中）
  	- M：修改（在HEAD中，在暂存区中，内容不同）
  	- D:删除（在HEAD中，不在暂存区）
  	- R：重命名（不在HEAD中，在暂存区，
  	- C：拷贝（不在HEAD中，在暂存区，从其他文件拷贝）路径修改）
  	- T：文件状态改变（在HEAD中，在暂存区，内容相同）
  	- U：未合并，需要冲突解决
  *	第二个字母表示工作区文件的更改状态
  	- -：新/未知（不在暂存区，在工作区）
  	- m：修改（在暂存区，在工作区，被修改）
  	- d：删除（在暂存区，不在工作区）
  *	两个表示状态的字母后面，显示文件名信息。如果有文件重名还会显示改变前后的文件名及文件的相似度

#####***3.11 repo forall***
  *	命令格式：$repo forall [< project >...] -c <command>
  *	说明：迭代器，可以在所有指定的项目中执行同一个shell命令
  *	举例：$repo forall product/tina/docs product/tina/tools -c ls		显示product/tina/docs和product/tina/tools项目下的文件	$repo forall -c 'echo $REPO_PROJECT' 显示清单文件中的所有项目
  *	选项：
  	-	-c:后面所带的参数着是shell指令
  	-	-p:在shell指令输出之前列出项目名称
  	-	-v:列出执行shell指令输出的错误信息
  * 额外的环境变量：
  	-	REPO_PROJECT:指定项目的名称
  	-	REPO_PATH:指定项目在工作区的相对路径
  	-	REPO_REMOTE:指定项目远程仓库的名称
  	-	REPO_LREV:指定项目最后一次提交服务器仓库对应的哈希值
  	-	REPO_RREV:指定项目在克隆时的指定分支，manifest里的revision属性
  另外，如果-c后面所带的shell指令中有上述环境变量，则需要用单引号把shell指令括起来。

#####***3.12repo remote***
  *	命令格式：$repo remote add < remotename > < url [< project >…]
  *	说明：根据xml文件添加远程仓库
  *	备注：如果没有repo remote命令，可以使用下面设置远程仓库
  	-	命令格式：$repo forall -c 'git remote add 仓库名 url/$REPO_PROJECT.git'
  	-	说明：为各个项目设置远程仓库
  	-	举例：$ repo forall -c 'git remote add tina http: //gerrit.allwinnertech.com:8081/$REPO_PROJECT.git'

#####***3.13 repo push***
  *	命令格式：$repo push < remotename >
  *	说明：把当前所在的分支推送到远程仓库
  *	备注：如果未有此命令，可使用git相关命令推送到远程库

#####***3.14 repo grep***
  * 命令格式：$repo grep
  * 说明：相当于对git grep的封装，用于在项目文件中进行内容查找
  * 举例：$repo grep *file product/tina/docs

#####***3.15 repo manifest***
  * 命令格式：$repo manifest
  * 说明：显示manifest文件内容
  * 举例：$repo manifest

#####***3.16 repo version***
  *	命令格式：$repo version
  *	说明：显示repo的版本号
  *	举例：$repo version

#####***3.17 repo info***
  *	命令格式：$repo info
  *	说明：显示清单文件中各个项目的有关信息，如项目所在路径等
  *	举例：$repo info

#####***3.18 repo upload***
  * 命令格式：$repo upload [--re --cc] {[<project>]… | --replace <project>}
  * 说明：相当于git push，但是又有很大的不同。它不是将版本库改动推送到克隆时的远程服务器，而是推送到	代码审核服务器(Gerrit软件架设)的特殊引用上，使用SSH协议。代码审核服务器会对推送的提交进行特殊处	 理，将新的提交显示为一个待审核的修改集，并进入代码审查流程，只有当审核通过后，才会合并到官方正式的	  版本库中。
  * 选项：
  	-	-h: --help:显示帮助信息
  	-	-t:发送本地分支名称到Gerrit代码审核服务器
  	-	--replace:发送此分支的更新补丁集
  	-	--re=REVIEWERS:要求指定的人员进行审核
  	-	-cc=CC:同时发送通知到如下邮件地址

#####***3.19 repo download***
  *	命令格式：$repo download {project change [patchset]}…
  *	说明：主要用于代码审核者下载和评估贡献者提交的修订

#####***3.20 repo selfupdate***
  * 命令格式：$repo selfupdate
  * 说明：用于repo自身的更新

#####***3.21 repo help***
  * 命令格式：repo help (--all)
  * 说明：查看所有命令的帮助信息