/*
Driver de Kernel 

/* Necessary includes for device drivers */

#include <linux/init.h>
//#include <linux/config.h>
#include <linux/module.h>
#include <linux/kernel.h> /* printk() */
#include <linux/slab.h> /* kmalloc() */
#include <linux/fs.h> /* everything... */
#include <linux/errno.h> /* error codes */
#include <linux/types.h> /* size_t */
#include <linux/proc_fs.h>
#include <linux/fcntl.h> /* O_ACCMODE */
//#include <asm/system.h> /* cli(), *_flags */
#include <asm/uaccess.h> /* copy_from/to_user */

MODULE_LICENSE("Dual BSD/GPL");

/* Declaration of driver.c functions */

int driver_open(struct inode *inode, struct file *filp);
int driver_release(struct inode *inode, struct file *filp);
ssize_t driver_read(struct file *filp, char *buf, size_t count, loff_t *f_pos);
ssize_t driver_write(struct file *filp, char *buf, size_t count, loff_t *f_pos);
void driver_exit(void);
int driver_init(void);

/* Structure that declares the usual file */
/* access functions */

struct file_operations driver_fops = {
  read: driver_read,
  write: driver_write,
  open: driver_open,
  release: driver_release
};



/* Declaration of the init and exit functions */

module_init(driver_init);
module_exit(driver_exit);



/* Global variables of the driver */
/* Major number */
int driver_major = 60;
/* Buffer to store data */
char *driver_buffer;
/* Buffer to store data 1*/
char *driver_buffer_1;

int n;
int n1;
int driver_init(void) {

  int result;

  /* Registering device */

  result = register_chrdev(driver_major, "driver", &driver_fops);
  	if (result < 0) {
  	  printk(
  	    "<1>driver: cannot obtain major number %d\n", driver_major);
  	  return result;
  	}

  /* Allocating driver for the buffer */
  driver_buffer = kmalloc(10, GFP_KERNEL); 
  if (!driver_buffer) { 
    result = -ENOMEM;
    goto fail; 
  } 

   driver_buffer_1 = kmalloc(10, GFP_KERNEL); 
   if (!driver_buffer_1) { 
    result = -ENOMEM;
    goto fail; 
  } 

  n=0;
  n1=0;
  memset(driver_buffer, 0, 10);
  memset(driver_buffer_1, 0, 10);
  printk("<1>Inserting driver module\n"); 
  return 0;
  fail: 

    driver_exit(); 

    return result;

}

void driver_exit(void) {

  /* Freeing the major number */
  unregister_chrdev(driver_major, "driver");
  /* Freeing buffer driver */
  if (driver_buffer) {
    kfree(driver_buffer);
  }
  if (driver_buffer_1) {
    kfree(driver_buffer_1);
  }



  printk("<1>Removing driver module\n");
}

int driver_open(struct inode *inode, struct file *filp) {
  /* Success */
  printk("<1>Opening driver module\n");	
  return 0;
}
int driver_release(struct inode *inode, struct file *filp) {
  /* Success */
  printk("<1>Releasing driver module\n");	
  return 0;
}

ssize_t driver_read(struct file *filp, char *buf, 
                    size_t count, loff_t *f_pos) { 
 
	int minor = iminor(filp->f_path.dentry->d_inode);

	if (minor==0){
		 printk("<1>Reading driver module minor - %d \n",minor);
		  if (*f_pos==n1) return 0;
  			*f_pos+=n1;
 			copy_to_user(buf,driver_buffer,n1);
  			return n1;
	}else if(minor==1){
		 printk("<1>Reading driver module - minor  %d \n",minor);
		  if (*f_pos==n) return 0;
  			*f_pos+=n;
  			copy_to_user(buf,driver_buffer_1,n);
	 		return n;
	}
}
ssize_t driver_write( struct file *filp, char *buf,
                      size_t count, loff_t *f_pos) {

  
	int minor = iminor(filp->f_path.dentry->d_inode);


	if (minor==0){
		 printk("%c - <1>Writing driver module - buffer 1 - minor %d \n",driver_buffer_1[n-1],minor );
		 copy_from_user((char *)(driver_buffer_1+n),buf,1);
  		 n++;
	}else if(minor==1){
		 printk("%c - <1>Writing driver module - buffer 0 - minor %d \n",driver_buffer[n1-1], minor );
		 copy_from_user((char *)(driver_buffer+n1),buf,1);
  		 n1++;
	}
  return 1;
}