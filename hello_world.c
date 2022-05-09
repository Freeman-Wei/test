#include <linux/types.h> //基本的类型定义
#include <linux/fs.h> //文件系统
#include <linux/mm.h> //内存管理
#include <linux/errno.h> //错误码
#include <asm/segment.h>// 汇编语言的一段程序
// 定义版本信息
#include <asm/uaccess.h>
#include <linux/module.h>

//#define THIS_MODULE (&__this_module)


unsigned int hello_world_major = 0; //定义主设备号(通过系统自动获取主设备号)
//定义一个读函数，对应read系统调用。(static表示该函数在本文件有效)

static ssize_t read_hello_world(struct file *file,char __user *buf, size_t count, loff_t *offt)
{
	char *temp= "raw:hello world !";

	printk("\n\ncopy_to_user is read \n");

	// if (access_ok(VERIFY_WRITE, buf, count) == -EFAULT ){ //验证是否可以向buf中写入数据。
	// 	return -EFAULT; 
    // }
    // else{
    //     printk("write_ok\n");
    // }
		
	printk("this is read_1 \n");

	if(strlen(temp)==0){
	printk("temp is empty");
	return count;
	} 

	printk("this is read_2 \n");

	if(!copy_to_user(buf,temp,count)){
	printk("copy_to_user is OK ");
	printk(KERN_EMERG "hello_world: OK %d\n",count);
	}
	else{
	printk("copy_to_user is read_fial ");
	return count;}

	printk("this is read_3 \n");

	return count; 
}


static ssize_t write_hello_world(struct file *file, const char __user *buf, size_t count ,loff_t *inode)
{
	char buff[128];

	printk("\n\nthis is write \n");

	// if (access_ok(4, buf, count) == -EFAULT ) //验证buf是否可以读。
	// 	return -EFAULT;
	// else printk("read_ok\n");

	printk("this is write_1 \n");


	if(copy_from_user(buff,buf,count)){
    
	printk("copy_from_user is write_fial \n");

		return -EFAULT;
    }
	else {
		printk("kernelSpace temp is : %s\n",buff);
		printk(KERN_EMERG "hello_worldwriter: OK %d\n",count);
	}
    
	printk("this is write_3 \n");

	return count;
}

static int open_hello_world(struct inode *inode, struct file *file )
{
	//MOD_INC_USE_COUNT; //注册到内核后，模块计数加一
	try_module_get(THIS_MODULE);
	printk("this is open_ok\n\n ");
	return 0;
}



static int release_hello_world(struct inode *inode, struct file *file )
{
	//MOD_DEC_USE_COUNT; //模块数减一
	module_put(THIS_MODULE);

	printk("close\n");

	return 0;
}



struct file_operations hello_world_fops =
{
	.owner   = THIS_MODULE,
	.read    = read_hello_world,
	.write   = write_hello_world,
	.open    = open_hello_world,
	.release = release_hello_world
};



int init_module1(void)
//int module_iit(void)
{
	int result;
	result = register_chrdev(0, "hello_world", &hello_world_fops); //注册字符型设备到内核中
	if (result < 0)
	{
		printk(KERN_INFO "hello_world: can't get major number\n"); //KERN_INFO打印信息
		return result;
	} else {
		printk("hello_world get success major\n");
	}
	if (hello_world_major == 0)
		hello_world_major = result; /* 获取系统默认的主设备号 */
	if(hello_world_major == 0) {
		printk("hello_world_major == 0\n");
	} else {
		printk("hello_world_major != 0 hello_world_major = %d \n",hello_world_major);
	}
	return 0;
}
void cleanup_module1(void)
//void module_exit(void)
{
	unregister_chrdev(hello_world_major,"hello_world");
    printk("\nhello_world cleanup\n");
//	printk(“%u has been relased”,hello_world_major);
}
module_init(init_module1);
module_exit(cleanup_module1);













// #include <linux/init.h>             
// #include <linux/module.h>          
// #include <linux/kernel.h>   

// //指定license版本
// MODULE_LICENSE("GPL");              

// //设置初始化入口函数
// static int __init hello_world_init(void)
// {
//     printk(KERN_DEBUG "hello world!!!\n");
//     return 0;
// }

// //设置出口函数
// static void __exit hello_world_exit(void)
// {
//     printk(KERN_DEBUG "goodbye world!!!\n");
// }

// //将上述定义的init()和exit()函数定义为模块入口/出口函数
// module_init(hello_world_init);
// module_exit(hello_world_exit);