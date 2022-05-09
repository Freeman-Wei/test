#include <linux/types.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/errno.h>
#include <asm/segment.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <asm/uaccess.h>
#include <linux/module.h>
#include <linux/cdev.h>  
#include <linux/device.h> 
#include <linux/gpio.h>
#include </home/fanvil/share/ssd21x/kernel/drivers/sstar/include/pioneer3/gpio.h>


MODULE_LICENSE("GPL");   

char stat_led[6] = "on\n";
static unsigned int led_io = 58;           //led_io口
static int io_status = -1;                 //led 状态
static struct class *myclass;		//设备类
static struct device *mydev;		//设备节点
unsigned int led_lv_of_major = 0; //定义主设备号(通过系统自动获取主设备号)


static void led_gpio_init(void)
{
    gpio_request(led_io, "lv_of");
    gpio_direction_output(led_io, 1);
    gpio_set_value(led_io, 1);
    io_status = 1;
    stat_led[0] = 'o';
    stat_led[1] = 'n';
    
}


static void led_set(char *opt)
{
    if(memcmp(stat_led, "on", 2) == 0){

        gpio_set_value(led_io, 1);
        io_status = 1;
    }else if(memcmp(stat_led, "off", 3) == 0){
        
        gpio_set_value(led_io, 0);
        io_status = 0;
    }
}



static int open_led_lv_of(struct inode *inode, struct file *file )
{
	//MOD_INC_USE_COUNT; //注册到内核后，模块计数加一
	try_module_get(THIS_MODULE);
    led_gpio_init();
	printk("led_io is open\n ");
	return 0;
}


static ssize_t read_led_lv_of(struct file *file,char __user *buf,	//__user 宏用以声明该变量来自用户空间
								size_t count, loff_t *offt)
{

	if (!access_ok(VERIFY_WRITE, buf, count)){ //验证是否可以向buf中写入数据。
		return -EFAULT; 
    }
    else{
        printk("write_ok\n");
    }    

	if(!copy_to_user(buf,stat_led,count)){	//将内核数据 temp 复制到 用户空间 buf

        printk(KERN_EMERG "led statsu read: OK !\n");

        if(io_status == 1){
            printk("led is light\n");
        }else{
            printk("led is extinct\n");
        }
	}
	else{
		printk("copy_to_user fial_read ");
		return count;
	}

	return count; 
}


static ssize_t write_led_lv_of(struct file *file, const char __user *buf, size_t count ,loff_t *inode)
{

	if (!access_ok(4, buf, count)){//验证buf是否可以读。
		return -EFAULT;
	} 
	else {
		printk("read_ok\n");
	}

	if(copy_from_user(stat_led,buf,count)){
    
		printk("copy_from_user is write_fial \n");
		return -EFAULT;
    }
	else {

        led_set(stat_led);
	}

	return 0;
}



static int release_led_lv_of(struct inode *inode, struct file *file )
{
    if(1 == io_status){
        
        gpio_free(led_io);
    }

	//MOD_DEC_USE_COUNT; //模块数减一
	module_put(THIS_MODULE);
	printk("close\n");

	return 0;
}



struct file_operations led_lv_of_fops =
{
	.owner   = THIS_MODULE,
	.read    = read_led_lv_of,
	.write   = write_led_lv_of,
	.open    = open_led_lv_of,
	.release = release_led_lv_of
};

//入口初始化函数 -- 挂载模块执行
static int __init led_of_init(void)
{
	int result;
	// struct device_node *node_name = NULL;

	// node_name = of_find_node_by_name(NULL, "padmux");
	// if(!node_name){

	// 	printk("of_find_node_by_name fail\n");
	// }

	// if(of_property_read_u32_index(node_name, "schematic", 6, &led_io)){

	// 	printk("of_property_read_u32_index fail\n");
	// }else{

	// 	printk("led_io:%u\n", led_io);
	// }
	
	
	result = register_chrdev(0, "led_lv_of", &led_lv_of_fops); //注册字符型设备到内核中
	if (result < 0)
	{
		printk(KERN_INFO "led_lv_of: can't get major number\n"); //KERN_INFO打印信息
		return result;
	} else {
		printk("led_lv_of get success major\n");
	}

	//创建类
	myclass = class_create(THIS_MODULE, "led_lv_of_class");
	if(myclass == NULL){
		printk("class_cerate fail\n");
		return -1;
	}

	//创建 设备节点
	mydev = device_create(myclass, NULL, MKDEV(result, 0), NULL, "led_lv_of");
	if(mydev == NULL){
		printk("device_create fail\n");
		return -1;
	}

	if (led_lv_of_major == 0)
		led_lv_of_major = result; /* 获取系统默认的主设备号 */

	if(led_lv_of_major == 0) {
		printk("led_lv_of_major == 0\n");
	} else {
		printk("led_lv_of_major != 0 led_lv_of_major = %d \n",led_lv_of_major);
	}

	printk("led_lv_of init OK!\n");

	return 0;
}
//出口函数 -- 卸载模块执行
static void __exit led_of_exit(void)
{
	unregister_chrdev(led_lv_of_major,"led_lv_of");
	device_destroy(myclass, MKDEV(led_lv_of_major, 0));
	class_destroy(myclass);
    printk("\nled_lv_of cleanup\n");
}
module_init(led_of_init);
module_exit(led_of_exit);