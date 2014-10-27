#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/time.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
MODULE_LICENSE("Dual BSD/GPL");

static int device_open(struct inode *inode,struct file *file)
{
  try_module_get(THIS_MODULE);
  return 0;
}

static int device_release(struct inode *inode,struct file *file)
{
  module_put(THIS_MODULE);
  return 0;
}

static ssize_t device_read(struct file *filp,char *buffer,size_t length,loff_t *offset)
{
  unsigned int result[3];
  struct timespec t;
  unsigned int c;
  if (length < sizeof result) return -EIO;
  asm volatile("mrc p15, 0, %0, c15, c12, 1" : "=r"(c));
  t = current_kernel_time();
  result[0] = c;
  result[1] = t.tv_nsec;
  result[2] = t.tv_sec;
  copy_to_user(buffer,result,sizeof result);
  return sizeof result;
}

#define DEVICE_NAME "cpucycles4ns"

static struct file_operations fops = {
  .read = device_read,
  .open = device_open,
  .release = device_release
} ;

static int major;

static int cpucycles4ns_init(void)
{
  unsigned int cycles1;
  unsigned int cycles2;
  printk(KERN_INFO DEVICE_NAME " starting\n");
  printk(KERN_INFO DEVICE_NAME " creating device\n");
  major = register_chrdev(0,DEVICE_NAME,&fops);
  if (major < 0) {
    printk(KERN_INFO DEVICE_NAME " failed with %d\n",major);
    return major;
  }
  printk(KERN_INFO DEVICE_NAME " suggests mknod /dev/" DEVICE_NAME " c %d 0\n",major);
  printk(KERN_INFO DEVICE_NAME " enabling cycle counter\n");
  asm volatile("mcr p15, 0, %0, c15, c12, 0" :: "r"(7));
  asm volatile("mrc p15, 0, %0, c15, c12, 1" : "=r"(cycles1));
  asm volatile("mrc p15, 0, %0, c15, c12, 1" : "=r"(cycles2));
  printk(KERN_INFO DEVICE_NAME " %u %u\n",cycles1,cycles2);
  return 0;
}

static void cpucycles4ns_exit(void)
{
  unsigned int cycles1;
  unsigned int cycles2;
  printk(KERN_INFO DEVICE_NAME " removing device\n");
  unregister_chrdev(major,DEVICE_NAME);
  printk(KERN_INFO DEVICE_NAME " disabling cycle counter\n");
  asm volatile("mcr p15, 0, %0, c15, c12, 0" :: "r"(0));
  asm volatile("mrc p15, 0, %0, c15, c12, 1" : "=r"(cycles1));
  asm volatile("mrc p15, 0, %0, c15, c12, 1" : "=r"(cycles2));
  printk(KERN_INFO DEVICE_NAME " %u %u\n",cycles1,cycles2);
  printk(KERN_INFO DEVICE_NAME " stopping\n");
}

module_init(cpucycles4ns_init);
module_exit(cpucycles4ns_exit);
