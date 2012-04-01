/*
hole.c 测试ldd3 列子
*/
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>

#include <linux/kernel.h>	/* printk() */
#include <linux/slab.h>		/* kmalloc() */
#include <linux/fs.h>		/* everything... */
#include <linux/errno.h>	/* error codes */
#include <linux/types.h>	/* size_t */
#include <linux/proc_fs.h>
#include <linux/fcntl.h>	/* O_ACCMODE */
#include <linux/seq_file.h>
#include <linux/cdev.h>

#include <asm/system.h>		/* cli(), *_flags */
#include <asm/uaccess.h>	/* copy_*_user */

#define HOLE_MAJOR 0
#define HOLE_MINOR 0

int hole_major=HOLE_MAJOR;
int hole_minor=HOLE_MINOR;
int hole_nr_devs = 1;

MODULE_AUTHOR("huangmingyou huangmingyou@gmail.com");
MODULE_LICENSE("Dual BSD/GPL");
int iread=0;

struct hole_set{
  void **data;
  struct hole_set *next;
};

struct hole_dev {
  char data[200];
  int data_len;
  struct hole_set *vdata;
  struct cdev cdev;
};

struct hole_dev *hole_devices;

ssize_t hole_write(struct file *filp,const char __user *buf,size_t count, loff_t *f_pos){

  if(count>199)return 0;
  copy_from_user(hole_devices->data,buf,count);
  
  hole_devices->data_len=count;
  return count;
}
ssize_t hole_read(struct file *filp,const char __user *buf,size_t count, loff_t *f_pos){

 
  int len=hole_devices->data_len;
  int i = (count >len)?len:count;
  if (*f_pos >=len) return 0;
	copy_to_user(buf,hole_devices->data,i);
	*f_pos+=i;
  	return i;
}

int hole_release(struct inode *inode,struct file *filp){
  return 0;
}

int hole_open(struct inode *inode,struct file *filp){
  struct hole_dev *dev;
  dev = container_of(inode->i_cdev,struct hole_dev,cdev);
  filp->private_data = dev;

  return 0;
}

loff_t hole_llseek(struct file *filp,loff_t off,int whence){
  return 0;
}
  struct file_operations hole_fops = {
    .owner = THIS_MODULE,
    .llseek = hole_llseek,
    .read = hole_read,
    .write = hole_write,
    .open = hole_open,
    .release = hole_release,
  };




 static void hole_setup_cdev(struct hole_dev *dev,int index){
    int err,devno=MKDEV(hole_major,hole_minor+index);
    cdev_init(&dev->cdev,&hole_fops);
    dev->cdev.owner=THIS_MODULE;
    dev->cdev.ops = &hole_fops;
    err=cdev_add(&dev->cdev,devno,1);
    if(err){
      printk(KERN_NOTICE,"Error %d adding hole%d",err,index);
    }
  }


void hole_cleanup_module(void){
  int i;
  dev_t devno=MKDEV(hole_major,hole_minor);

  if(hole_devices){
    for(i=0;i<hole_nr_devs;i++){
      cdev_del(&hole_devices[i].cdev);
    }
    kfree(hole_devices);
  }
}

int hole_init_module(void){
  int result,i;
  dev_t dev = 0;

  //动态分配设备号

  result=alloc_chrdev_region(&dev,hole_minor,hole_nr_devs,"hole");
  hole_major=MAJOR(dev);
  if(result <0){
    printk(KERN_WARNING "hole: can't get major %d\n",hole_major);
    return result;
  }

  /*
    分配存储设备结构的内存并清0
  */

  hole_devices=kmalloc(hole_nr_devs*sizeof(struct hole_dev),GFP_KERNEL);

 if(!hole_devices){
    result = -ENOMEM;
    goto fail;
  }

  memset(hole_devices,0,hole_nr_devs*sizeof(struct hole_dev));

  //初始化设备

   for (i=0;i<hole_nr_devs;i++){
    hole_setup_cdev(&hole_devices[i],i);
  }

	return 0;
 fail:
  hole_cleanup_module();
  return result;
}
  
  

module_init(hole_init_module);
module_exit(hole_cleanup_module);
