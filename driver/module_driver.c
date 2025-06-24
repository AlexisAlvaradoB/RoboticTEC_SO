#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alexis Alvarado");
MODULE_DESCRIPTION("Arduino Module");
MODULE_VERSION("0.1");

static dev_t arduino_mod_nr;
static struct class * arduino_class;
static struct cdev arduino_mod;


#define DRIVER_NAME "arduino_driver"
#define DRIVER_CLASS "arduinoModClass"

#define MAX_LEN 256

static char message[MAX_LEN] = {0};
static size_t message_len = 0;

/**
    @brief Lee los datos y los pega en el buffer
    
    @return La cantidad de datos que faltaron por copiar
*/
static ssize_t driver_read(struct file *File, char __user *buf, size_t count, loff_t *offset)
{
    if (*offset >= message_len)
        return 0; // EOF

    size_t to_copy = min(count, message_len - *offset);

    if (copy_to_user(buf, message + *offset, to_copy))
        return -EFAULT;

    *offset += to_copy;

    return to_copy;
}

/**
 * @brief Obtiene los datos del buffer y los transmite
 *
 * @return Cantidad de bytes que lograron copiarse
 */
static ssize_t driver_write(struct file *File, const char __user *buf, size_t len, loff_t *offset)
{
    size_t to_copy = min(len, (size_t)(MAX_LEN - 1));

    if (copy_from_user(message, buf, to_copy))
        return -EFAULT;

    message[to_copy] = '\0';
    message_len = to_copy;

    printk(KERN_INFO "arduino_driver: stored '%s'\n", message);

    return to_copy;

}


/**
 * @brief Función para cuando se abre el archivo del dispositivo
 * 
 * @return 0 en éxito
*/
static int driver_open(struct inode * device_file, struct file * instance){
    printk("arduino_hand_driver - open called\n");
    return 0;
}

/**
 * @brief Para cuando se cierra el archivo del dispositivo
 * 
 * @return 0 en éxito
 */
static int driver_close(struct inode * device_file, struct file * instance){
    printk("arduino_hand_driver - close called\n");
    return 0; 
}


static struct file_operations fops =
{
    .owner = THIS_MODULE,
    .open = driver_open,
    .release = driver_close,
    .read = driver_read,
    .write = driver_write
};

/**
 * @brief Función ante la montura del driver en el kernel
 * 
 * @return 0 en éxito
 */
static int __init my_module_init(void) {
    printk(KERN_INFO "Module loaded.\n");
    
    if(alloc_chrdev_region(&arduino_mod_nr, 0, 1, DRIVER_NAME) < 0){
        printk("El dispositivo no se pudo alocar\n");
        return -1;
    }else{
        printk("Arduino Driver - Device Nr. Major: %d, Minor: %d se registró\n", arduino_mod_nr >>20, arduino_mod_nr && 0xffff);
    }

    if((arduino_class = class_create(DRIVER_CLASS)) == NULL){
        printk("La clase no pudo crearse\n");
        goto ClassError;
    }
    if(device_create(arduino_class, NULL, arduino_mod_nr, NULL, DRIVER_NAME) == NULL){
        printk("No se pudo crear el archivo del dispositivo");
        goto FileError;
    }
    cdev_init(&arduino_mod, &fops);
    if(cdev_add(&arduino_mod, arduino_mod_nr, 1) == -1){
        printk("Error en el registro del dispositivo\n");
        goto Add_Error;
    }

    return 0;
Add_Error:
    device_destroy(arduino_class, arduino_mod_nr);
FileError:
    class_destroy(arduino_class);
ClassError:
    unregister_chrdev(arduino_mod_nr, DRIVER_NAME);
    return -1;
}

/**
 * @brief Función para cuando se desmonta el módulo del kernel
 * 
 */
static void __exit my_module_exit(void) {
    cdev_del(&arduino_mod);
    device_destroy(arduino_class, arduino_mod_nr);
    class_destroy(arduino_class);
    unregister_chrdev(arduino_mod_nr, DRIVER_NAME);
    printk(KERN_INFO "Module unloaded.\n");
}


module_init(my_module_init);
module_exit(my_module_exit);