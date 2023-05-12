# QVirtualKeyboard

#### 介绍
Qt5虚拟键盘支持中英文,仿qt官方的virtualkeyboard模块,但使用QWidget实现。

#### 界面效果
![输入图片说明](https://images.gitee.com/uploads/images/2019/0112/184531_f547af49_382617.png "无标题.png")

#### 使用说明
（这里是编译成debug版本）
1、编译pinyin，生成libqtpinyind.a
2、编译qt5softkeyboard，生成libQt5SoftKeyboardd.so
3、把libQt5SoftKeyboardd.so放到qt安装目录下的plugins/platforminputcontexts，例如我当前的目录是：/opt/Qt5.9.6/5.9.6/gcc_64/plugins/platforminputcontexts
4、用于自己的项目时，在项目main.cpp的main函数中设置好环境变量。代码：qputenv("QT_IM_MODULE",QByteArray("Qt5Input"));
