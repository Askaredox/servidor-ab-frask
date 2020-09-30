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
#include <linux/smp.h>     // get_cpu(), put_cpu()
#include <linux/cpufreq.h> // cpufreq_get()
#include <linux/cpumask.h> // cpumask_{first,next}(), cpu_online_mask


void __attribute__((weak)) arch_report_meminfo(struct seq_file *m){}

static int my_proc_show(struct seq_file *m, void *v){
    struct sysinfo i;
    unsigned cpu;
    unsigned long u_cpu, t_cpu, u_mem, t_mem;

    si_meminfo(&i);
    u_mem = i.totalram - i.freeram;
    t_mem = i.totalram;

    cpu = cpumask_first(cpu_online_mask);
    u_cpu = cpufreq_quick_get(cpu);
    t_cpu = cpufreq_quick_get_max(cpu);

    seq_printf(m, "{\n\t\"u_mem\":%lu,\n\t\"t_mem\":%lu,\n\t\"u_cpu\":%lu,\n\t\"t_cpu\":%lu\n}\n", u_mem, t_mem, u_cpu, t_cpu);

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