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

#include <linux/cpumask.h> // for_each_possible_cpu()
#include <linux/sched/stat.h>
#include <linux/sched/cputime.h>
#include <linux/kernel_stat.h>
#include <linux/tick.h>
#include <linux/time64.h>
#include <linux/types.h>
#include <linux/delay.h>

#ifndef arch_irq_stat_cpu
#define arch_irq_stat_cpu(cpu) 0
#endif
#ifndef arch_irq_stat
#define arch_irq_stat() 0
#endif


void __attribute__((weak)) arch_report_meminfo(struct seq_file *m){}

struct cpuinfo{
    unsigned long long total;
    unsigned long long used;
};

#ifdef arch_idle_time

static u64 get_idle_time(struct kernel_cpustat *kcs, int cpu){
    u64 idle;

    idle = kcs->cpustat[CPUTIME_IDLE];
    if (cpu_online(cpu) && !nr_iowait_cpu(cpu))
        idle += arch_idle_time(cpu);
    return idle;
}

static u64 get_iowait_time(struct kernel_cpustat *kcs, int cpu){
    u64 iowait;

    iowait = kcs->cpustat[CPUTIME_IOWAIT];
    if (cpu_online(cpu) && nr_iowait_cpu(cpu))
        iowait += arch_idle_time(cpu);
    return iowait;
}

#else

static unsigned long long get_idle_time(struct kernel_cpustat *kcs, int cpu){
    u64 idle, idle_usecs = -1ULL;

    if (cpu_online(cpu))
        idle_usecs = get_cpu_idle_time_us(cpu, NULL);

    if (idle_usecs == -1ULL)
        idle = kcs->cpustat[CPUTIME_IDLE];
    else
        idle = idle_usecs * NSEC_PER_USEC;

    return idle;
}

static u64 get_iowait_time(struct kernel_cpustat *kcs, int cpu){
    u64 iowait, iowait_usecs = -1ULL;

    if (cpu_online(cpu))
        iowait_usecs = get_cpu_iowait_time_us(cpu, NULL);

    if (iowait_usecs == -1ULL)
        iowait = kcs->cpustat[CPUTIME_IOWAIT];
    else
        iowait = iowait_usecs * NSEC_PER_USEC;

    return iowait;
}

#endif

static void show_stat(struct cpuinfo *val){
    int i;
    u64 user, nice, system, idle, iowait, irq, softirq, steal;
    u64 guest, guest_nice;

    u64 user2, nice2, system2, idle2, iowait2, irq2, softirq2, steal2;
    u64 guest2, guest_nice2;

    u64 PrevIdle, Idle, PrevNonIdle, NonIdle, PrevTotal, Total, TotalD, IdleD;

    user = nice = system = idle = iowait =
        irq = softirq = steal = 0;
    guest = guest_nice = 0;

    user2 = nice2 = system2 = idle2 = iowait2 =
        irq2 = softirq2 = steal2 = 0;
    guest2 = guest_nice2 = 0;

    for_each_possible_cpu(i) {
        struct kernel_cpustat *kcs = &kcpustat_cpu(i);

        user += kcs->cpustat[CPUTIME_USER];
        nice += kcs->cpustat[CPUTIME_NICE];
        system += kcs->cpustat[CPUTIME_SYSTEM];
        idle += get_idle_time(kcs, i);
        iowait += get_iowait_time(kcs, i);
        irq += kcs->cpustat[CPUTIME_IRQ];
        softirq += kcs->cpustat[CPUTIME_SOFTIRQ];
        steal += kcs->cpustat[CPUTIME_STEAL];
        guest += kcs->cpustat[CPUTIME_GUEST];
        guest_nice += kcs->cpustat[CPUTIME_GUEST_NICE];
    }

    msleep(1000);

    for_each_possible_cpu(i) {
        struct kernel_cpustat *kcs = &kcpustat_cpu(i);

        user2 += kcs->cpustat[CPUTIME_USER];
        nice2 += kcs->cpustat[CPUTIME_NICE];
        system2 += kcs->cpustat[CPUTIME_SYSTEM];
        idle2 += get_idle_time(kcs, i);
        iowait2 += get_iowait_time(kcs, i);
        irq2 += kcs->cpustat[CPUTIME_IRQ];
        softirq2 += kcs->cpustat[CPUTIME_SOFTIRQ];
        steal2 += kcs->cpustat[CPUTIME_STEAL];
        guest2 += kcs->cpustat[CPUTIME_GUEST];
        guest_nice2 += kcs->cpustat[CPUTIME_GUEST_NICE];
    }

    PrevIdle = idle+ iowait;
    Idle= idle2+iowait2;

    PrevNonIdle= user+nice+system+irq+softirq+steal;
    NonIdle= user2+nice2+system2+irq2+softirq2+steal2;

    PrevTotal= PrevIdle+PrevNonIdle;
    Total= Idle+NonIdle;

    TotalD= Total-PrevTotal;
    IdleD= Idle-PrevIdle;
    

    val->total = TotalD;
    val->used = TotalD-IdleD;
}

static int my_proc_show(struct seq_file *m, void *v){
    struct sysinfo i;
    struct cpuinfo cpu;
    unsigned long u_cpu, t_cpu, u_mem, t_mem;

    si_meminfo(&i);
    u_mem = i.totalram - i.freeram;
    t_mem = i.totalram;

    show_stat(&cpu);
    u_cpu = cpu.used;
    t_cpu = cpu.total;

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