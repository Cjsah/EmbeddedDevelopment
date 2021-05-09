此工程是根据开源步兵车程序根据实际步兵车进行更改来的，里面进行了一些修改并添加了一些个人的代码注释，方便新手学习。
目录：
CMSIS：内核相关的文件
FWLIB：标准库文件
Project：工程文件
startup:芯片启动文件
user：用户编写的相关文件，主要编写的文件都在这个文件夹下

user/main.c\h :main函数所在的文件
user/AHRS：陀螺仪驱动以及姿态解算
user/APP：freeRTOS任务
user/DSP：DSP库
user/FreeRTOS:移植的freeRTOS文件
user/hardware：硬件层
user/user_lib：编写的数学函数