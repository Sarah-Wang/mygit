/*************************************************************************
	> File Name: sig_drive.c
	> Author:fury_zhu
	> Created Time: 2018年09月11日 星期二 17时59分58秒
 ************************************************************************/
#include <linux/errno.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/ioctl.h>
#include <asm/io.h>
#include <asm/system.h>
#include <asm/uaccess.h>

#define SIGTESTDRV_MAJOR 0
#define SIGTESTDEV_SIZE 4096

struct sigtest_dev
{
	char *data;
	unsigned long size;
	struct fasync_struct *async_queue;
};

#define SIGTESTDEV_IOC_MAGIC 's'
#define SIGTESTDEV_PRINT _IO(SIGTESTDEV_IOC_MAGIC, 1)
#define SIGTESTDEV_IO_SIGNAL _IO(SIGTESTDEV_IOC_MAGIC, 2, int)

#define SIGTESTDEV_IOC_MAXNR 2

static int sigtest_major = SIGTESTDEV_MAJOR;
struct sigtest_dev *sigtest_devp;
struct cdev cdev;
module_param(sigtest_major, int, S_IRUGO);

	/* 文件打开函数 */
int sigtest_open(struct inode *inode, struct file *filp)
{
	struct sigtest_dev *dev;
	int num = MINOR(inode->i_rdev);

	if (num >= SIGTESTDEV_NR_DEVS)
		return -ENODEV;
	dev = &sigtest_devp[num];
	filp->private_data = dev;

	return 0;
}

	/*文件释放函数*/
int sigtest_release(struct inode *inode, struct file *filp)
{
	sigtest_fasync(-1, filp, 0);
	return 0;

}

	/* IO操作 */
int sigtestdev_ioctl(struct inode *inode, struct file *filp,
			unsigned int cmd, unsigned long arg)
{

	int err = 0;
	int ret = 0;
	int ioarg = 0;
	struct sigtest_dev *dev = filp->private_data;

	if (_IOC_TYPE(cmd) != SIGTESTDEV_IOC_MAGIC)
		return -EINVAL;
	if (_IOC_NR(cmd) > SIGTESTDEV_IOC_MAXNR)
		return -EINVAL;

	if (_IOC_DIR(cmd) & _IOC_READ)
		err = !access_ok(VERIFY_WRITE, (void *)arg, _IOC_SIZE(cmd));
	else if (_IOC_DIR(cmd) & _IOC_WRITE)
		err = !access_ok(VERIFY_READ, (void *)arg, _IOC_SIZE(cmd));
	if (err)
		return -EFAULT;

	switch(cmd) {

	/* 打印当前设备信息 */
	case SIGTESTDEV_PRINT:
		printk("SIGTESTDEV_PRINT\n");
		break;

	/* 获取参数 */
	case SIGTESTDEV_IO_SIGNAL:
		if(&dev->async_queue, SIGIO, POLL_IN)
		kill_fasync(&sigtest_fasync, SIGIO, POLL_IN);
		break;

	default:
		return EINVAL;
	}
	return ret;

}

	/* 异步操作函数 */
static int sigtest_fasync(int fd, struct file *filp, int mode)
{
	struct sigtest_dev *dev = filp->private_data;
	return fasync_helper(fd, filp, mode, &dev->async_queue);

}

static const struct file_operations sigtest_fops =
{
	.owner = THIS_MODULE,
	.open = sigtest_open,
	.release = sigtest_release,
	.ioctl = sigtestdev_ioctl,

};

	/*设备驱动模块加载函数*/
static int sigtestdev_init(void)
{
	int result;
	int i;

	dev_t devno = MKDEV(sigtest_major, 0);

	if (sigtest_major)
	result = register_chrdev_region(devno, 1, "sigtestdev");
	else
	{
	result = alloc_chrdev_region(&devno, 0, 1, "sigtestdev");
	sigtest_major = MAJOR(devno);
	}

	if (result < 0)
	return result;

	cdev_init(&cdev, &sigtest_fops);
	cdev.owner = THIS_MODULE;
	cdev.ops = &sigtest_fops;

	cdev_add(&cdev, MKDEV(sigtest_major, 0), 1);

	/* 为设备描述结构分配内存*/
	sigtest_devp = kmalloc(sizeof(struct sigtest_dev), GFP_KERNEL);
	if (!sigtest_devp)
	{
	result =  - ENOSIGTEST;
	goto fail_malloc;
	}
	memset(sigtest_devp, 0, sizeof(struct sigtest_dev));

	/*为设备分配内存*/
		sigtest_devp.size = SIGTESTDEV_SIZE;
		sigtest_devp.data = kmalloc(SIGTESTDEV_SIZE, GFP_KERNEL);
		memset(sigtest_devp.data, 0, SIGTESTDEV_SIZE);

	return 0;

	fail_malloc:
	unregister_chrdev_region(devno, 1);

	return result;

}

	/*模块卸载函数*/
static void sigtestdev_exit(void)
{
	cdev_del(&cdev);
	kfree(sigtest_devp);
	unregister_chrdev_region(MKDEV(sigtest_major, 0), 1);

}

MODULE_AUTHOR("fury_zhu");
MODULE_LICENSE("GPL");

module_init(sigtestdev_init);
module_exit(sigtestdev_exit);
