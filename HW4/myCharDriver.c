/**
 * @file 
 *		myCharDriver.c
 * @brief 
 *		This file implements char Driver for LED
 * @author 
 *		Saritha Senguttuvan
 * @date 
 *		November 19,2017
 *
 */
#include <linux/init.h>           //For init and exit functions
#include <linux/module.h>         // For loading the kernel modules
#include <linux/device.h>         // Supports drivers 
#include <linux/kernel.h>         
#include <linux/fs.h>             // Supports linux file system
#include <linux/uaccess.h>        // Supports copy_to_user
#include <linux/gpio.h>    
#include <linux/slab.h>
#include <linux/gfp.h> 
#define  DEVICE_NAME "LED_CharDriver"    // Device name
#define  CLASS_NAME  "LED_Class"        // Class name

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("My Char driver kernel module");
MODULE_AUTHOR("Saritha Senguttuvan");

void ktimer_CB_func(unsigned long data);        /* function prototype for call bcak function for timer */
struct timer_list my_ktimer;                    /* declaring a timerof type struct timer_list */

static int    majorNumber;                  // Major number
static char   *message;           // User space message
static int  msg_size;               // Size of the message
static int    openCnt = 0;              // Number of times the device is opened
static struct class*  LEDcharClass  = NULL; // Class pointer
static struct device* LEDcharDevice = NULL; // Device struct pointer

/* LED */
static bool  ledOn = 0;
static unsigned int gpioLED = 53; 
int period;
int dutyCycle;  
int toggle_LED;
bool led_val = false;
int query_single;

/* Function prototypes for file operations*/
static int     char_dev_open(struct inode *, struct file *);
static int     char_dev_release(struct inode *, struct file *);
static ssize_t char_dev_read(struct file *, char *, size_t, loff_t *);
static ssize_t char_dev_write(struct file *, const char *, size_t, loff_t *);

/* Fops struct definition */
static struct file_operations fops =
{
   .open = char_dev_open,
   .read = char_dev_read,
   .write = char_dev_write,
   .release = char_dev_release,
};

/* struct for storinf=g the current state information to be stored in the private data*/
typedef struct 
{
	int st_rate;
	int st_dutyCycle;
	bool st_LED_toggle;
}myState;

myState currentState; /* variable of type myState */

/* Timer Call back function */
void ktimer_CB_func(unsigned long data)         
{
    printk("In the Timer callback\n");
    if(led_val == true)       /* Toggle the led if on */
    {
    	led_val = false;
    	gpio_set_value(gpioLED, led_val); 
    	mod_timer( &my_ktimer, jiffies + msecs_to_jiffies(period - dutyCycle));    /* modify the timer value */   
    }
    else if(led_val == false)   /* Toggle the led if off */
    {
    	led_val = true;
    	gpio_set_value(gpioLED, led_val); 
    	mod_timer( &my_ktimer, jiffies + msecs_to_jiffies(dutyCycle));    /* modify the timer value */   
    }

}

/* Init function for the kernel module */
static int __init my_driver_init(void)
{
   printk(KERN_INFO "charDriver: Initializing Kernel module\n");
   majorNumber = register_chrdev(0, DEVICE_NAME, &fops);		//Registering the character device
   if (majorNumber<0)
   {
      printk(KERN_ALERT "charDriver: failed to register a major number\n");
      return majorNumber;
   }
   printk(KERN_INFO "charDriver: registered correctly with major number %d\n", majorNumber);
   LEDcharClass = class_create(THIS_MODULE, CLASS_NAME);		// Register the device class
   if (IS_ERR(LEDcharClass))
   {                												
	  unregister_chrdev(majorNumber, DEVICE_NAME);
	  printk(KERN_ALERT "Failed to register device class\n");
	  return PTR_ERR(LEDcharClass);          
   }
   printk(KERN_INFO "charDriver: device class registered correctly\n");

   LEDcharDevice = device_create(LEDcharClass, NULL, MKDEV(majorNumber, 0), NULL, DEVICE_NAME);  //Device driver registers
   if (IS_ERR(LEDcharDevice))
   {               
      class_destroy(LEDcharClass);           
      unregister_chrdev(majorNumber, DEVICE_NAME);
      printk(KERN_ALERT "Failed to create the device\n");
      return PTR_ERR(LEDcharDevice);
   }
   printk(KERN_INFO "charDriver: device class created correctly\n"); 
   
	ledOn = false;
	
  gpio_direction_output(gpioLED, ledOn);  
  gpio_export(gpioLED, false);			 
    
  printk("Timer module installing \n");  
  my_ktimer.expires = jiffies + msecs_to_jiffies(1000);        /* giving the timer expiration time as 500 ms */
  my_ktimer.data = 0;                             /* argument passed to the call back function */
  my_ktimer.function = ktimer_CB_func;               /* assigning call back function name */ 
      
  init_timer(&my_ktimer);                         /* initialzing the timer */  
   return 0;
}

/* Exit function for the linux kernel module */
static void __exit my_driver_exit(void)
{
	gpio_set_value(gpioLED, 0);              // Turn the LED off
	gpio_unexport(gpioLED);                  // Unexport the LED GPIO
	del_timer( &my_ktimer );
	device_destroy(LEDcharClass, MKDEV(majorNumber, 0));     // remove the device with the device number
	class_unregister(LEDcharClass);                          // unregister the device class
	class_destroy(LEDcharClass);                             // remove the device class
	unregister_chrdev(majorNumber, DEVICE_NAME);             // unregister the major number
	printk(KERN_INFO "charDriver: exit the kernel module\n");
}

/* open the device */
static int char_dev_open(struct inode *inodep, struct file *filep)
{
	openCnt++;
	currentState.st_rate = 0;
	currentState.st_dutyCycle = 0;
	currentState.st_LED_toggle = false;
	filep->private_data = (void *)&currentState;  /* Storing the current state in the private data */
	printk(KERN_INFO "charDriver: Device has been opened \n");
	return 0;
}

/* Read the device */
static ssize_t char_dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset)
{	
	int error_count = 0;
	message = (char *)kmalloc(len * sizeof(char), GFP_KERNEL); 
	filep->private_data = (void *)&currentState; 
  if(query_single == 4)
    sprintf(message, "Period %d, Dutycycle %d, Led Toggling %d", currentState.st_rate, currentState.st_dutyCycle, currentState.st_LED_toggle); 
  else if(query_single == 1)
    sprintf(message, "Period %d", currentState.st_rate);
  else if(query_single == 2)
    sprintf(message, "Dutycycle %d", currentState.st_dutyCycle);
  else if(query_single == 4)
    sprintf(message, "Toggling %d", currentState.st_LED_toggle);




  printk(KERN_INFO "charDriver: Device has been read \n");
  msg_size = strlen(message);
  error_count = copy_to_user(buffer, message, msg_size);  /* send to userspace */
  if (error_count==0)
  {            
     	printk(KERN_INFO "charDriver: Sent %d characters to the user\n", msg_size); 
  }
  else 
  {
     	printk(KERN_ALERT "charDriver: Failed to send %d characters to the user\n", error_count);
     	kfree(message);
     	return -EFAULT;            
 	} 	
 	kfree(message);
  return 0;
}

/* write to the device */
static ssize_t char_dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset)
{
    int error_count = 0;
	  message = (char *)kmalloc(len * sizeof(char), GFP_KERNEL);  
  	error_count = copy_from_user((void*)message, (const void*)buffer, len);
  	if (error_count==0)
  	{            
  		sscanf(message, "%d %d %d %d", &period, &dutyCycle, &toggle_LED, &query_single); /* Get the input from the user */ 		  		
  		printk(KERN_INFO "charDriver: Received %d number of characters \n", len);
  		printk(KERN_INFO "charDriver: Received period is %d\n", period);
  		printk(KERN_INFO "charDriver: Received duty Cycle is %d\n", dutyCycle);
  		printk(KERN_INFO "charDriver: Received LED toggling state is %d\n", toggle_LED);
      printk(KERN_INFO "charDriver: Received query number is %d\n", query_single);
  	}
  	else 
  	{
  		printk(KERN_ALERT "charDriver: Failed to send %d characters to the user\n", error_count);
  		kfree(message);
  		return -EFAULT;            
 	  }
	printk(KERN_INFO "charDriver: Device has been written \n");
	currentState.st_rate = period;
	currentState.st_dutyCycle = dutyCycle;	

	if(toggle_LED)       /* If toggling on given by the user */
	{
		led_val = true;
		currentState.st_LED_toggle = led_val;
		gpio_set_value(gpioLED, led_val); 
		add_timer(&my_ktimer); 
	}
	else           /* If toggling off given by the user */
	{
		led_val = false;
		currentState.st_LED_toggle = led_val;		
		gpio_set_value(gpioLED, led_val); 
		del_timer( &my_ktimer );
	}
	
	filep->private_data = (void *)&currentState;  /* Store the current state in the private data */
	kfree(message);
	return 0;
}

/* Release the device */
static int char_dev_release(struct inode *inodep, struct file *filep)
{ 
	printk(KERN_INFO "charDriver: Device successfully closed\n");
	return 0;
}

module_init(my_driver_init);    /* Initialize the module */
module_exit(my_driver_exit);    /* Exit the module */



























