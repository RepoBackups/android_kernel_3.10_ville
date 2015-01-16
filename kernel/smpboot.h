#ifndef SMPBOOT_H
#define SMPBOOT_H

struct task_struct;

<<<<<<< HEAD
int smpboot_prepare(unsigned int cpu);

=======
>>>>>>> common/android-3.10.y
#ifdef CONFIG_GENERIC_SMP_IDLE_THREAD
struct task_struct *idle_thread_get(unsigned int cpu);
void idle_thread_set_boot_cpu(void);
void idle_threads_init(void);
#else
static inline struct task_struct *idle_thread_get(unsigned int cpu) { return NULL; }
static inline void idle_thread_set_boot_cpu(void) { }
<<<<<<< HEAD
static inline void idle_threads_init(unsigned int cpu) { }
=======
static inline void idle_threads_init(void) { }
>>>>>>> common/android-3.10.y
#endif

int smpboot_create_threads(unsigned int cpu);
void smpboot_park_threads(unsigned int cpu);
void smpboot_unpark_threads(unsigned int cpu);

#endif
