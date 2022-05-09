
#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/platform_device.h>
#include <linux/device.h>
#include <linux/ioport.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <linux/fs.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/interrupt.h>
#include <linux/list.h>
#include <linux/timer.h>
 
 
//probe方法实现步骤：
/*
1、注册设备号，
2、创建设备节点
3、初始化硬件
4、实现各种io接口
*/
 
struct led_dev{
	int dev_major;   //主设备号
	struct class *cls;   //设备类
	struct device *dev;   //设备节点
	struct resource *res;  //获取到的资源
	void *reg_base;   //物理地址映射后的虚拟地址
};
 
struct led_dev *samsung_led;
 
ssize_t pled_write(struct file *filp, const char __user *buf, size_t count, loff_t *fops)
{
	int val;
	int ret;
 
	printk("--------------%s----------\n",__FUNCTION__);
	
	ret = copy_from_user(&val, buf, count);
	if(ret > 0)
	{
		printk("copy_from_user error\n");
 
		return -EFAULT;
	}
	
	if(val)   //灭灯
	{
		writel(readl(samsung_led->reg_base+4)|(1<<5), samsung_led->reg_base+4);  //一个寄存器4个字节 data寄存器为con寄存器+4
	}
	else  //亮灯
	{
		
		writel(readl(samsung_led->reg_base+4)&~(1<<5), samsung_led->reg_base+4);  //一个寄存器4个字节 data寄存器为con寄存器+4
	}
 
 
	return 0;
}
 
int pled_open(struct inode *inode, struct file *filp)
{
	printk("--------------%s----------\n",__FUNCTION__);
 
	return 0;
 
}
 
int pled_close (struct inode *ionde, struct file *filp)
{
	printk("--------------%s----------\n",__FUNCTION__);
 
	return 0;
}
 
 
static struct file_operations pled_fops = {
 
	.open = pled_open ,
	.release = pled_close ,
	.write = pled_write ,
 
};
 
static int led_pdrv_probe(struct platform_device * pdev)
{
	printk("--------------%s----------\n",__FUNCTION__);
 
	samsung_led = kmalloc(sizeof(struct led_dev), GFP_KERNEL);
	if(samsung_led==NULL)
	{
		printk("samsung_led kmalloc error\n");
 
		return -ENOMEM;
	}
 
	samsung_led->dev_major = register_chrdev(0, "led_drv", &pled_fops); //动态申请
	
	samsung_led->cls = class_create(THIS_MODULE, "led_class") ;
 
	samsung_led->dev = device_create(samsung_led->cls, NULL, 
						MKDEV(samsung_led->dev_major, 0), NULL, "led0");
 
	//获取资源
	//参数1、从哪个pdev 获取资源
	//参数2、资源类型
 
	/*参数3、表示获取同种资源第几个 ,一定要注意是同种资源的排序（即先看同种资源，再看数组下标），
	而不是光看数组下标
	*/
 
	samsung_led->res = platform_get_resource(pdev,IORESOURCE_MEM, 0);
 
	samsung_led->reg_base = ioremap(samsung_led->res->start, 
						samsung_led->res->end-samsung_led->res->start+1);
 
	if(samsung_led->reg_base == NULL)
	{
		printk("ioremap error \n");
 
		return -ENOMEM;
 
	}
 
	
	//配置寄存器输出，writel对某个寄存器写数据；
 
	writel((readl(samsung_led->reg_base)&~(3<<10))|(1<<10), samsung_led->reg_base);
	
	
	return 0;
}
 
static int led_pdrv_remove(struct platform_device * pdev)
{
	printk("--------------%s----------\n",__FUNCTION__);
 
	
	iounmap(samsung_led->reg_base);    //释放io资源
	device_destroy(samsung_led->cls, MKDEV(samsung_led->dev_major, 0));
    class_destroy(samsung_led->cls);
	unregister_chrdev(samsung_led->dev_major, "led_drv");
	kfree(samsung_led);
	
	return 0;
}
 
//const struct platform_device_id *id_table  老版本的没有;
 
//构建一个platform_driver对象
 
struct platform_driver led_pdrv = {
	.probe = led_pdrv_probe ,    //匹配成功调用
	.remove = led_pdrv_remove ,
	.driver = {
			.name = "myled" ,   //可以用来作匹配，也可以用id匹配  ，
								//在driver目录下会生成myled驱动
				
	}
	
};
 
static int __init plat_led_drv_init(void)
{
	int ret;
 
	printk("--------------%s----------\n",__FUNCTION__);
 
	ret = platform_driver_register(&led_pdrv);
	if(ret < 0)
	{
		printk("driver register error!\n");
		return ret;
	
	}
	
	return 0;
 
}
 
static void __exit plat_led_drv_exit(void)
{
	printk("--------------%s----------\n",__FUNCTION__);
 
	platform_driver_unregister(&led_pdrv);
 
 
}
 
 
module_init(plat_led_drv_init);
module_exit(plat_led_drv_exit);
MODULE_LICENSE("GPL");
 
