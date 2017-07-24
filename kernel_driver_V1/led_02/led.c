#include <linux/module.h>
#include <linux/fs.h>

#include "../include/board.h"
#include "../include/head.h"
#include "common.h"
#include "led.h"



/////////////////////////////////////////////////////////////////////裸板驱动

void led_all_off(void)
{
	printk("led all off set val\n");
	set_nbits_val(LED_DAT_ADDR, 0, LED_TOTLE_NUM, (0x01<<(LED_TOTLE_NUM))-1);		// Set all GPIO output high
}

void led_all_on(void)
{
	set_nbits_val(LED_DAT_ADDR, 0, LED_TOTLE_NUM, 0x0);	// Set all GPIO output low
}


void led_on(unsigned int num)
{
	set_bit_val(LED_DAT_ADDR, num, 0);		// Set GPIO output low, LED ON
}


void led_off(unsigned int num)
{
	set_bit_val(LED_DAT_ADDR, num, 1);		// Set GPIO output high, LED OFF
}

void led_config(void)
{
#if 1
	set_nbits_val(LED_CON_ADDR, 0, 4*LED_TOTLE_NUM, 0x1111);	// Set GPIO as output
#else
	int i = 0;
	for (i = 0; i < LED_TOTLE_NUM; i++)
	{
		set_nbits_val(LED_CON_ADDR, i*4, 4, 0x01);	// Set GPIO to output
	}
#endif
}

void led_init(void)
{
	printk("Init pin\n");
	led_config();

	printk("All led off\n");
	led_all_off();
}

unsigned int led_all_status_get(void)
{
	return get_nbits_val(LED_DAT_ADDR, 0, LED_TOTLE_NUM);
}

void led_all_status_set(unsigned int LedStatus)
{
	set_nbits_val(LED_DAT_ADDR, 0, LED_TOTLE_NUM, LedStatus);
}

void led_set_blink(unsigned long BlinkData)
{
	int dir = -1;
	int num = -1;
	int blinkNtime = 0;
	int blinkPtime = 0;
	int blinkCount = 0;

	dir = LED_IOC_CTRL_DIR(BlinkData);
	num = LED_IOC_CTRL_NUM(BlinkData);
	blinkCount = LED_IOC_CTRL_CNT(BlinkData);
	blinkNtime = LED_IOC_CTRL_NTIME(BlinkData);
	blinkPtime = LED_IOC_CTRL_PTIME(BlinkData);

	while (blinkCount--)
	{
		led_on(num);
		msleep(blinkPtime);
		led_off(num);
		msleep(blinkNtime);
	}
}

void led_set_run_lamp_positive(int startNum, int runLampCount, int runLmapPtime, int runLmapNtime)
{
	int i = 0;

	for (i = startNum; i < (runLampCount*LED_TOTLE_NUM+startNum); i++)
	{
		led_on(i%4);
		msleep(runLmapPtime);
		led_off(i%4);
		msleep(runLmapNtime);
	}
}

void led_set_run_lamp_negative(int startNum, int runLampCount, int runLmapPtime, int runLmapNtime)
{
	int i = 0;

	for (i = (runLampCount*LED_TOTLE_NUM+startNum); i > startNum; i--)
	{
		led_on(i%4);
		msleep(runLmapPtime);
		led_off(i%4);
		msleep(runLmapNtime);
	}
}

void led_set_runlamp(unsigned long RunLampData)
{
	int dir = -1;
	int startNum = -1;
	int runLampCount = 0;
	int runLampNtime = 0;
	int runLampPtime = 0;

	dir = LED_IOC_CTRL_DIR(RunLampData);
	startNum = LED_IOC_CTRL_NUM(RunLampData);
	runLampCount = LED_IOC_CTRL_CNT(RunLampData);
	runLampNtime = LED_IOC_CTRL_NTIME(RunLampData);
	runLampPtime = LED_IOC_CTRL_PTIME(RunLampData);

	if (dir > 0)
	{
		led_set_run_lamp_positive(startNum, runLampCount, runLampPtime, runLampNtime);
	}
	else if (dir == 0)
	{
		led_set_run_lamp_negative(startNum, runLampCount, runLampPtime, runLampNtime);
	}
}

void led_set_run_lamp_positive_dep(void)
{
	int i = 0;

	for (i = 0; i < LED_TOTLE_NUM * 100; i++)
	{
		led_on(i%4);
		msleep(500);
		//delay();
		led_off(i%4);
		msleep(500);
		//delay();
	}
}

void led_set_run_lamp_negative_dep(void)
{
	int i = 0;

	for (i = LED_TOTLE_NUM * 100; i > 0; i--)
	{
		led_on(i%4);
		msleep(500);
		led_off(i%4);
		msleep(500);
	}
}

////////////////////////////////////////////////////////////////////////////字符设备框架

long led_test_ioctl (struct file *filp, unsigned int cmd, unsigned long arg)
{
	int ret = -1;
	unsigned int LedStatus = 0;
	int LedBlinkData = 0;
	int LedRunLampData = 0;

	printk("led test ioctl!\n");

	if (_IOC_TYPE(cmd) != LED_IOC_TYPE)
	{
		printk("Driver: LED cmd type error!\n");
		return	-EINVAL;
	}

	if (_IOC_NR(cmd) > LED_IOC_MAX_NR)
	{
		printk("Driver: LED cmd error, No such cmd!\n");
		return	-EINVAL;
	}

/*
 *    if (_IOC_DIR(cmd) & _IOC_READ) {
 *        ret = access_ok(VERIFY_WRITE, (void *)args, _IOC_SIZE(cmd));
 *    } else {
 *        ret = access_ok(VERIFY_READ, (void *)args, _IOC_SIZE(cmd));
 *    }
 */

	switch(cmd)
	{
		case	LED_ALL_OFF:
			printk("ioctl: All LED Off\n");
			led_all_off();
			break;
		case	LED_ALL_ON:
			printk("ioctl: All LED On\n");
			led_all_on();
			break;
		case	LED1_OFF:
			printk("ioctl: LED1 Off\n");
			led_off(0);
			break;
		case	LED1_ON:
			printk("ioctl: LED1 On\n");
			led_on(0);
			break;
		case	LED2_OFF:
			printk("ioctl: LED2 Off\n");
			led_off(1);
			break;
		case	LED2_ON:
			printk("ioctl: LED2 On\n");
			led_on(1);
			break;
		case	LED3_OFF:
			printk("ioctl: LED3 Off\n");
			led_off(2);
			break;
		case	LED3_ON:
			printk("ioctl: LED3 On\n");
			led_on(2);
			break;
		case	LED4_OFF:
			printk("ioctl: LED4 Off\n");
			led_off(3);
			break;
		case	LED4_ON:
			printk("ioctl: LED4 On\n");
			led_on(3);
			break;

		case	LED_IOC_SET_STATUS:
			printk("ioctl: LED set status\n");
			ret = __get_user(LedStatus, (int *)arg);
			led_all_status_set(LedStatus);
			printk("ioctl: LED set status, LedStatus = %u\n", LedStatus);
			break;
		case	LED_IOC_GET_STATUS:
			printk("ioctl: LED get status\n");
			LedStatus = led_all_status_get();
			ret = __put_user(LedStatus, (int *)arg);
			break;
		case	LED_IOC_SET_BLINK_DATA:
			printk("ioctl: LED set blink data\n");
			ret = __get_user(LedBlinkData, (int *)arg);
			printk("ioctl: arg = %lu\n", arg);
			led_set_blink(arg);
			break;
		case	LED_IOC_GET_BLINK_DATA:
			printk("ioctl: LED get blink data\n");
			ret = __put_user(LedBlinkData, (int *)arg);
			break;
		case	LED_IOC_SET_RUN_LAMP_DATA:
			printk("ioctl: LED set RunLamp positive data\n");
			ret = __get_user(LedRunLampData, (int *)arg);
			printk("ioctl: arg = %lu\n", arg);
			led_set_runlamp(arg);
			break;
		case	LED_IOC_GET_RUN_LAMP_DATA:
			printk("ioctl: LED get RunLamp negative data\n");
			ret = __put_user(LedRunLampData, (int *)arg);
			break;

		default:
			printk("Driver: LED cmd error");
			return	-EINVAL;
	}
	return 0;
}

ssize_t led_test_write (struct file *filp, const char __user *buf, size_t size, loff_t *offset)
{
	printk("led test write!\n");
	return size;
}

ssize_t led_test_read (struct file *filp, char __user *buf, size_t size, loff_t *offset)
{
	printk("led test read!\n");
	return size;
}

int led_test_open (struct inode *inodep, struct file *filp)
{
	printk("led test open!\n");
	return 0;
}

int led_test_close (struct inode *inodep, struct file *filp)
{
	printk("led test close!\n");
	return 0;
}

/////////////////////////////////////////////////////////////模块
struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = led_test_open,
	.release = led_test_close,
	.read = led_test_read,
	.write = led_test_write,
	.unlocked_ioctl = led_test_ioctl,
};

int major = 0;

static int led_test_init(void)
{
	int ret = -1;
	printk("Hello, led driver chrdev register test begin!\n");

	printk("Init led\n");
	led_init();

	ret = register_chrdev(major, "led_test", &fops);
	ERRP_K(ret < 0, "LED", "register_chrdev", goto ERR_dev_register);

	if (major == 0) {
		major = ret;
		printk("major = %d\n", major);
	} else {
		printk("major = %d\n", major);
	}

	return 0;
ERR_dev_register:
	return -1;
}

static void led_test_exit(void)
{
	led_all_off();
	printk("Goodbye, led test over!\n");
	unregister_chrdev(major, "led_test");
}


module_init(led_test_init);
module_exit(led_test_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mshrimp");
MODULE_DESCRIPTION("Test led");
MODULE_VERSION("V0.1");
