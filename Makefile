# #KERNELDIR := /lib/modules/$(shell uname -r)/build
# PWD := $(shell pwd)

# obj-m := hello_world.o

# #在编绎内核本身export-objs中的文件时，make会增加一个＂-DEXPORT_SYMTAB＂编绎标志，它使源文件嵌入mo#dversions.h文件，将EXPORT_SYMBOL宏展开中的函数名字符串进行版本名扩展；同时，它也定义_set_ver()宏#为一空操作，使代码中的函数名不受其影响。　　
# #在编绎模块时，make会增加＂-include=linux/modversion.h -DMODVERSIONS＂编绎标志，使模块中代码的函##数名得到相应版本扩展。

# all:
# 	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

# clean:
# 	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean










#include makefile_test/test.mk

# -y：表示编译进 内核
# -m：表示编译成 .ko 文件
obj-m := demo_jpeg.o
# obj-m := led_dev.o
# obj-m += led_dri.o

#内核目录
KERNELDIR := /home/fanvil/share/ssd21x/kernel
#编译器
CROSS_COMPILE:=/opt/gcc-arm-8.2-2018.08-x86_64-arm-linux-gnueabihf/bin/arm-linux-gnueabihf-
#当前目录
CURRENT_PATH := $(shell pwd)
 
build: kernel_modules

kernel_modules: 
	$(MAKE) ARCH=arm CROSS_COMPILE=$(CROSS_COMPILE) -C $(KERNELDIR) M=$(CURRENT_PATH) modules
clean:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) clean
