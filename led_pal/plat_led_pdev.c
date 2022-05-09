
#include <linux/module.h>
#include <linux/version.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/interrupt.h>
#include <linux/list.h>
#include <linux/timer.h>
#include <linux/init.h>
#include <linux/serial_core.h>
#include <linux/platform_device.h>
 
 
 
//led GPF 4 5 6
 
 
struct resource led_res[]=
{
	[0] = {
		.start = 57 ,               //这个是起始地址 GPFCON
		.end =  58 , 		    //GPFUP终止地址      0x56000058-1 
		.flags = IORESOURCE_MEM ,		  //说明是什么类型的资源 这里是IO内存资源
	},
 
	[1] = {
 
		.start = 5 ,             //start和end是一样的，中断号为5
		.end =   5 , 		    
		.flags =  IORESOURCE_IRQ ,              //中断资源
	},
};
 
 
struct platform_device led_pdev = {
 
	.name = "myled" ,  //名称和driver里面的保持一致
	.id = -1 ,
	.num_resources = ARRAY_SIZE(led_res),  //资源数量
	.resource = led_res ,
};
 
 
 
static int __init plat_led_dev_init(void)
{
	int ret;
 
	printk("--------------%s----------\n",__FUNCTION__);
 
	ret = platform_device_register(&led_pdev);
	if(ret < 0)
	{
		printk("platform_device_register error!\n");
		return ret;
	
	}
	
	return 0;
 
}
 
static void __exit plat_led_dev_exit(void)
{
	printk("--------------%s----------\n",__FUNCTION__);
 
	platform_device_unregister(&led_pdev);
 
 
}
 
module_init(plat_led_dev_init);
module_exit(plat_led_dev_exit);
MODULE_LICENSE("GPL");
 
 
