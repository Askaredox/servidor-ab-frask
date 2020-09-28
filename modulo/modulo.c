/**
 * @file    modulo.c
 * @author  Andrés Carvajal
 * @date    23 Septiembre 2020
 * @version 0.1
 * @brief   Obtención del uso de RAM y CPU desde el kernel de linux por medio de un modulo
 *
*/


#include <linux/init.h>   // Macros para funciones __init __exit
#include <linux/module.h> // Header para cargar el modulo al kernel
#include <linux/kernel.h> // Contiene tipos, macros y funciones para el kernel
#include <linux/fs.h>
#include <linux/seq_file.h>
#include <linux/proc_fs.h>
#include <linux/mm.h>
#include <linux/cpufreq.h>

void __attribute__((weak)) arch_report_meminfo(struct seq_file *m){}

static int my_proc_show(struct seq_file *m, void *v){
    struct sysinfo i;
    struct cpufreq_policy *cp;
    unsigned long uc_temp, tc_temp, um_temp, tm_temp, mem_usage, cpu_usage;

    si_meminfo(&i);
    um_temp = i.totalram - i.freeram;
    tm_temp = i.totalram;
    mem_usage = um_temp / tm_temp;
    //mem_usage = 33;

    //cp = cpufreq_cpu_get(0);
    //uc_temp = cp->cur - cp->min;
    //tc_temp = cp->max - cp->min;
    //cpu_usage = 100 * uc_temp / tc_temp;
    uc_temp = cpufreq_quick_get(0);
    tc_temp = cpufreq_quick_get_max(0);
    cpu_usage = 0;

    seq_printf(m, "{\n\t\"um_temp\":%lu,\n\t\"tm_temp\":%lu,\n\t\"uc_temp\":%lu,\n\t\"tc_temp\":%lu\n}\n", um_temp, tm_temp, uc_temp, tc_temp);

    arch_report_meminfo(m);

    return 0;
}

static int my_proc_open(struct inode *inode, struct file *file){
    return single_open(file, my_proc_show, NULL);
}

static ssize_t my_proc_write(struct file *file, const char __user *buffer, size_t count, loff_t *f_pos){
    return 0;
}

static struct file_operations my_fops={
    .owner = THIS_MODULE,
    .open = my_proc_open,
    .release = single_release,
    .read = seq_read,
    .llseek = seq_lseek,
    .write = my_proc_write
};

static int __init modulo_init(void){
    struct proc_dir_entry *entry;
    entry = proc_create("modulo", 0777, NULL, &my_fops);
    if(!entry) {
        return -1;    
    } else {
        printk(KERN_INFO "Inicio\n");
    }
    return 0;
}

static void __exit modulo_exit(void){
    remove_proc_entry("modulo",NULL);
    printk(KERN_INFO "Fin\n");
}

module_init(modulo_init);
module_exit(modulo_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Obtención del uso de RAM y CPU desde el kernel de linux por medio de un modulo");
MODULE_AUTHOR("Andrés Carvajal");