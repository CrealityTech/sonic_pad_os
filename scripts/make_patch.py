#!/usr/bin/env python
# -*- coding: utf-8 -*-
# Power by lisidong v1.0 20171102

import os, sys, getopt
import shutil
import time
import fileinput
import zipfile

class Config:
    TYPE_DIFF = 1
    TYPE_DIFF_WITH_NEW_FILE = 0
    TYPE_CACHE = 2
    TYPE_HISTORY = 3

    #可手动修改如下参数，执行时可不带参数
    P_TYPE = TYPE_DIFF
    P_GITS = []
    P_HISTORY_DIFF_BETWEEN = []
    P_NAME = ""
    P_MESSAGE = ""
    P_PLATFORM_VERSION = ""

    BASE_DIR = os.path.split(os.path.realpath(__file__))[0] + "/../"
    P_OUT_ROOT_DIR = BASE_DIR + "AUTO_CREATE_PATCH/"
    P_OUT_PATCH_DIR = P_OUT_ROOT_DIR + time.strftime('%Y_%m_%d', time.localtime(time.time())) + "/" + P_NAME + time.strftime('%Y_%m_%d', time.localtime(time.time())) + "/"
    P_OUT_README = P_OUT_PATCH_DIR + "README.txt"
    P_OUT_SOURCE_FOLDER = P_OUT_PATCH_DIR + "source/"
    P_OUT_DIFF_FOLDER = P_OUT_PATCH_DIR + "diff/"
    P_OUT_DIFF_PATCH_NAME = P_NAME + ".patch"
    P_TEMP_FILE = P_OUT_ROOT_DIR + "temp.patch"
    P_TEMP_DIR = P_OUT_ROOT_DIR + "temp/"

    @staticmethod
    def setname(name):
        Config.P_NAME = name;
        Config.P_OUT_PATCH_DIR = Config.P_OUT_ROOT_DIR + time.strftime('%Y_%m_%d',
                                                         time.localtime(time.time())) + "/" + name + time.strftime(
            '%Y_%m_%d', time.localtime(time.time())) + "/"
        Config.P_OUT_README = Config.P_OUT_PATCH_DIR + "README.txt"
        Config.P_OUT_SOURCE_FOLDER = Config.P_OUT_PATCH_DIR + "source/"
        Config.P_OUT_DIFF_FOLDER = Config.P_OUT_PATCH_DIR + "diff/"
        Config.P_OUT_DIFF_PATCH_NAME = Config.P_NAME + ".patch"

class FileState:
    name = ""
    hashfrom = ""
    hashto = ""
    cachestatus = ""
    diffstatus = ""

    def __init__(self, name, hashfrom, hashto, cachestatus, diffstatus):
        self.name = name
        self.hashfrom = hashfrom
        self.hashto = hashto
        self.cachestatus = cachestatus
        self.diffstatus = diffstatus

    def isdir(self):
        return os.path.isdir(self.name)

    def istypecache(self):
        return self.cachestatus not in [' ', '?', '']

    def istypediff(self):
        return self.diffstatus not in ['', ' ']

    def dump(self):
        return self.cachestatus + self.diffstatus + " " + self.name

    def dumpall(self):
        return "name=" + self.name + " hashfrom=" + self.hashfrom + " self.hashto=" + self.hashto + \
               " cache=" + self.cachestatus + " diff=" + self.diffstatus

allgits = []
gitslist = []
gitsdic = {}


def parseConfig():
    try:
        shortargs = 'd:D:c:p:b:n:m:v:h'
        longargs = ['diff=', 'DIFF=', 'cache=', 'patch=', 'between=', 'name=', 'message=', 'version=', 'help']
        opts,args= getopt.getopt( sys.argv[1:], shortargs, longargs)
        print 'opts=',opts
        print 'args=',args

        for opt,arg in opts:
            #print 'prints',opt,arg
            if opt in ('-d','--diff'):
                Config.P_TYPE = Config.TYPE_DIFF
                Config.P_GITS = arg.split(";")
            if opt in ('-D', '--DIFF'):
                Config.P_TYPE = Config.TYPE_DIFF_WITH_NEW_FILE
                Config.P_GITS = arg.split(";")
            elif opt in ('-c','--cache'):
                Config.P_TYPE = Config.TYPE_CACHE
                Config.P_GITS = arg.split(";")
            elif opt in ('-p', '--patch'):
                Config.P_TYPE = Config.TYPE_HISTORY
                Config.P_GITS = arg.split(";")
            elif opt in ('-b', '--between'):
                Config.P_HISTORY_DIFF_BETWEEN = arg.split(";")
            elif opt in ('-n', '--name'):
                Config.setname(arg)
            elif opt in ('-m', '--message'):
                Config.P_MESSAGE = arg
            elif opt in ('-v', '--version'):
                Config.P_PLATFORM_VERSION = arg
            elif opt in ('-h', '--help'):
                usage()
                sys.exit(1)

    except getopt.GetoptError:
        print 'getopt error!'
        usage()
        sys.exit(1)

def init():
    global gitslist,allgits
    if os.path.exists(Config.P_OUT_PATCH_DIR):
        shutil.rmtree(Config.P_OUT_PATCH_DIR)
    if os.path.exists(Config.P_TEMP_DIR):
        shutil.rmtree(Config.P_TEMP_DIR)
    makefiledirvalid(Config.P_TEMP_DIR)

    os.chdir(Config.BASE_DIR)
    for git in fileinput.input(".repo/project.list"):
        allgits.append(git.strip())

    for git in (allgits if len(Config.P_GITS) == 0 else Config.P_GITS):
        gitslist.append(git[:-1] if git.endswith("/") else git)

def initgits(gitslist):

    if Config.P_TYPE == Config.TYPE_HISTORY:
        if len(Config.P_GITS) == 0:
            for gitdir in gitslist:
                gitsdic[gitdir] = initgitshistory(gitdir, Config.P_HISTORY_DIFF_BETWEEN[0].split(":")[0], Config.P_HISTORY_DIFF_BETWEEN[0].split(":")[1])
        else:
            for gitdir, between in zip(gitslist, Config.P_HISTORY_DIFF_BETWEEN):
                gitsdic[gitdir] = initgitshistory(gitdir, between.split(":")[0], between.split(":")[1])
    else:
        for gitdir in gitslist:
            gitsdic[gitdir] = initgitsdiff(gitdir)
    return gitsdic

def initgitsdiff(gitdir):
    os.chdir(Config.BASE_DIR + gitdir)
    #print 'dir=', os.getcwd()

    gitfiles = []
    if Config.P_TYPE == Config.TYPE_DIFF or Config.P_TYPE == Config.TYPE_DIFF_WITH_NEW_FILE:
        for line in os.popen("git diff --raw").readlines():
            filestate = FileState(line.split()[5].strip(), line.split()[2].strip()[:-3], line.split()[3].strip()[:-3], "", line.split()[4].strip())
            gitfiles.append(filestate)

    if Config.P_TYPE == Config.TYPE_DIFF_WITH_NEW_FILE:
        for line in os.popen("git status -s").readlines():
            if line.split()[0].strip() == "??":
                filestate = FileState(line.split()[1].strip(), "", "", "?", "?")
                gitfiles.append(filestate)

    elif Config.P_TYPE == Config.TYPE_CACHE:
        for line in os.popen("git diff --cached --raw").readlines():
            filestate = FileState(line.split()[5].strip(), line.split()[2].strip()[:-3], line.split()[3].strip()[:-3], line.split()[4].strip(), "")
            gitfiles.append(filestate)

    return gitfiles

def initgitshistory(gitdir, difffrom, diffto):
    os.chdir(Config.BASE_DIR + gitdir)
    #print 'dir=', os.getcwd()
    gitfiles = []

    if isdatetype(difffrom) and isdatetype(diffto):
        os.system("git diff --after=" + difffrom + " --before=" + diffto + " ")
        list = os.popen("git log --format=\"%H\" --after=\"" + difffrom + "\" --before=\"" + diffto + "\"").readlines()
        difffrom = list(0)
        diffto = list(list.count()-1)

    for line in os.popen("git diff --raw "+ difffrom + " " + diffto):
        filestate = FileState(line.split()[5].strip(), line.split()[2].strip()[:-3], line.split()[3].strip()[:-3], "",
                              line.split()[4].strip())
        gitfiles.append(filestate)
    return gitfiles

def isdatetype(datestr):
    try:
        if ":" in datestr:
            time.strptime(datestr, "%Y-%m-%d %H:%M:%S")
        else:
            time.strptime(datestr, "%Y-%m-%d")
        return True
    except:
        return False


def domakepatch(gitsdic):
    makefiledirvalid(Config.P_TEMP_FILE)
    if Config.P_TYPE == Config.TYPE_DIFF or Config.P_TYPE == Config.TYPE_DIFF_WITH_NEW_FILE:
        for gitdir, gitfiles in gitsdic.iteritems():
            if len(gitfiles) == 0:
                continue
            os.chdir(Config.BASE_DIR + gitdir)
            os.system("git diff --binary > " + Config.P_TEMP_FILE)
            #print gitdir
            for file in gitfiles:
                #print file.name + " " + file.diffstatus

                if file.diffstatus in (['M', 'A', '?'] if Config.P_TYPE == Config.TYPE_DIFF_WITH_NEW_FILE else ['M', 'A']) :
                    assert copyfile(Config.BASE_DIR + gitdir + "/" + file.name, Config.P_OUT_SOURCE_FOLDER + gitdir + "/" + file.name), "copy fail"

                    if (file.diffstatus in ['?']):
                        if os.path.isdir(file.name):
                            os.system(
                                "git diff --no-index " + Config.P_TEMP_DIR + " " + file.name + " >> " + Config.P_TEMP_FILE)
                        else:
                            os.system(
                                "git diff --no-index /dev/null " + file.name + " >> " + Config.P_TEMP_FILE)

            assert copyfile(Config.P_TEMP_FILE, Config.P_OUT_DIFF_FOLDER + gitdir + "/" + Config.P_OUT_DIFF_PATCH_NAME, True), "copy fail"

    if Config.P_TYPE == Config.TYPE_CACHE:
        for gitdir, gitfiles in gitsdic.iteritems():
            if len(gitfiles) == 0:
                continue
            os.chdir(Config.BASE_DIR + gitdir)
            os.system("git diff --cached --binary > " + Config.P_TEMP_FILE)
            assert copyfile(Config.P_TEMP_FILE, Config.P_OUT_DIFF_FOLDER + gitdir + "/" + Config.P_OUT_DIFF_PATCH_NAME, True), "copy fail"

            for file in gitfiles:
                if file.cachestatus in ['M','A']:
                    #print file.dump() + " "+ file.hashto
                    os.system("git show " + file.hashto + " > " + Config.P_TEMP_FILE)
                    assert copyfile(Config.P_TEMP_FILE, Config.P_OUT_SOURCE_FOLDER + gitdir + "/" + file.name, True), "copy fail"

    if Config.P_TYPE == Config.TYPE_HISTORY:
        for gitdir, gitfiles in gitsdic.iteritems():
            if len(gitfiles) == 0:
                continue
            os.chdir(Config.BASE_DIR + gitdir)
            for file in gitfiles:

                #print "git show --binary " + file.hashto
                os.system("git show --binary " + file.hashto + " > " + Config.P_TEMP_FILE + "_")
                assert copyfile(Config.P_TEMP_FILE + "_", Config.P_OUT_SOURCE_FOLDER + gitdir + "/" + file.name, True), "copy fail"
                #print "git diff --binary " + file.hashfrom + " " + file.hashto + " >> " + Config.P_TEMP_FILE
                os.system("git diff --binary " + file.hashfrom + " " + file.hashto + " >> " + Config.P_TEMP_FILE)
            assert copyfile(Config.P_TEMP_FILE, Config.P_OUT_DIFF_FOLDER + gitdir + "/" + Config.P_OUT_DIFF_PATCH_NAME, True), "copy fail"

def packpatch():
    os.chdir(Config.P_OUT_PATCH_DIR + "../")
    patchname = Config.P_NAME + time.strftime('%Y_%m_%d', time.localtime(time.time()))
    os.system("tar -czf " + patchname + ".tar.gz " + patchname)
    print "OUT:" + os.path.abspath(patchname)

def makefiledirvalid(dir):
    desdir = ""
    if os.path.isdir(dir):
        desdir = dir
    else:
        desdir = os.path.dirname(dir)
    if not os.path.exists(desdir):
        os.makedirs(desdir, mode=0o777)
    return desdir

def copyfile(src, des, deletesrc=False):
    try:
        desdir = makefiledirvalid(des)
        if os.path.isdir(src) and os.path.isdir(des):
            os.rmdir(desdir)
            shutil.copytree(src, desdir)
        else:
            shutil.copy(src, des)

        if deletesrc:
            os.remove(src)
            #os.removedirs(src)
    except Exception, e:
        print "error---:" + str(e)
        return False
    else:
        return True

def check():
    assert set(gitslist).issubset(set(allgits)), '请检查.repo/project.list中是否包含有如下仓库名字：%s'%gitslist
    if Config.P_TYPE == Config.TYPE_HISTORY:
        if (len(gitslist) == 0):
            assert len(Config.P_HISTORY_DIFF_BETWEEN) == 1, "当前为HISTORY(-p)模式，请检查-p -b 对应的数组大小是否一致，仓库以’;‘分割"
        else:
            assert len(gitslist) == len(Config.P_HISTORY_DIFF_BETWEEN), "当前为HISTORY(-p)模式，请检查-p -b 对应的数组大小是否一致，仓库以’;‘分割"

    assert Config.P_NAME != None and Config.P_NAME != "", "-n name:名字不能为空"
    assert Config.P_MESSAGE != None and Config.P_MESSAGE != "", "-m message:描述不能为空"

def usage():
    usage = '''
用法: make_patch [选项...]
生成标准格式的PATCH文件，可以从DIFF、CACHE、HISTORY中抽取特定格式，提高PATCH的可追溯行和可读性。

Examples:

  make_patch -d "android/build/;lichee/linux-3.10" -m message -n name -v "H6 v1.0" #上述两个仓库的diff抽取成补丁
  make_patch -D "android/build/;lichee/linux-3.10" -m message -n name -v "H6 v1.0" #上述两个仓库的diff(包括未跟踪的文件)抽取成补丁
  make_patch -c "android/build/;lichee/linux-3.10" -m message -n name -v "H6 v1.0" #上述两个仓库的暂存区(cached中)抽取成补丁
  make_patch -p "android/build/;lichee/linux-3.10" -b "HEAD^:HEAD;HEAD^^:HEAD" -m message -n name -v "H6 v1.0" #上述两个仓库的历史提交中抽取补丁
  make_patch -p "android/build/;lichee/linux-3.10" -b "2017-01-01:2017-02-01;hash1:hash2" -m message -n name -v "H6 v1.0" #上述两个仓库的历史提交中抽取补丁
  可修改make_patch.py中的必要属性(Config中:P_TYPE,P_GITS,P_HISTORY_DIFF_BETWEEN,P_NAME,P_MESSAGE,P_PLATFORM_VERSION)，而后直接执行make_patch.py即可(参数赋值)(不推荐)

 主操作模式:

  -d, --diff       类型1：创建各仓库的diff差异补丁(git diff )，各仓库以';'分割。如-d "android/frameworks/base;android/build/"
  -D, --DIff       类型2：创建各仓库的diff差异（同时包括未追踪的新文体）补丁，各仓库以';'分割。如-D "android/frameworks/base;android/build/"
  -c, --cache      类型3：创建各仓库的已加入到暂存区的差异补丁(git diff --cache),各仓库以';'分割。如-D "android/frameworks/base;android/build/"

  -p, --patch      类型4：创建提取各仓库的历史提交内容，各仓库以';'分割。如-D "android/frameworks/base;android/build/"，需要-b参数描述抽取点。
  -b, --between    配合-p使用，描述对应仓库的抽取点(hash值，时间等),以':'分割两个抽取点，以';'分割各仓库。如-b "HEAD^:HEAD;2017-01-01:2017-01-02;hash1:hash2"

  -m, --message    此补丁的描述信息，必选。
  -n, --name       此补丁的名字，必选。
  -v, --version    此补丁的适用(或当前发布时)的平台与版本信息等，必选

  -h, --help       打印此帮助信息


TIPS:
	1、补丁抽取后必须检查是否正确，同时注意log打印。
	2、当仓库被修改得相对复杂(比如含有未跟踪的文件、未暂存的工作区、暂存的文件等)，建议将需要抽取的补丁add到暂存区(而后用-c模式)，有利于更清晰明确抽取目标diff。
	3、当使用-p模式时，针对-b参数较为严格(对参数要求较高，如当仓库只有一个提交，HEAD^记录会找不到)，需要谨慎使用!

    '''
    print usage

def dumpinfo(gitsdic):
    for (key,values) in gitsdic.items():
        print "key=[" + key + "]"
        if (len(values) == 0):
            print '\033[1;31;40m'
            print "Empty,maybe something wrong with this git "
            print '\033[0m'
        for value in values:
            print "    " + value.dump()

def patchreadme(gitsdic):
    readme = '''
----------------------------------------------------------------------------------------------------
补丁名称：''' + Config.P_NAME + '''
适用版本：''' + Config.P_PLATFORM_VERSION + '''
发布日期：''' + time.strftime('%Y-%m-%d', time.localtime(time.time())) + '''

文件结构:
    ├── source         所有涉及修改的源码文件
    ├── diff           所有涉及的修改
    └── README.txt     本说明文件

    使用方法:
        在对应的diff_patch下的各个目录使用git apply ***.patch即可,如发生冲突，请使用source目录手动移植。注意查阅log信息，存在删除文件时较容易忽略。

    补丁说明:''' + Config.P_MESSAGE + '''

----------------------------------------------------------------------------------------------------
File INFORMATION

'''
    makefiledirvalid(Config.P_OUT_README)
    os.mknod(Config.P_OUT_README)
    file = open(Config.P_OUT_README, 'r+')
    file.write(readme)

    for (key,values) in gitsdic.items():
        #print "key=" + key + " value=" + values
        file.write(key + "\n")
        for value in values:
            file.write("    " + value.dump() + "\n")
        file.write("\n")
    file.write('''
----------------------------------------------------------------------------------------------------
File Tree List

''')
    file.flush()
    file.close()
    os.chdir(Config.P_OUT_PATCH_DIR)
    os.system("tree -a  >> " + Config.P_OUT_README)

parseConfig()
init()
check()

initgits(gitslist)
dumpinfo(gitsdic)
domakepatch(gitsdic)
patchreadme(gitsdic)
packpatch()
