/*
 * Intercept pthread_mutex_lock/pthread_mutex_unlock and display
 * a message and a backtrace for each call.
 *
 * Written for CS 3204 Virginia Tech, Fall 2006.
 *
 * Godmar Back <godmar@gmail.com>
 */
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#define __USE_GNU 1     /* for RTLD_NEXT */
#include <dlfcn.h>

static int (*real_pthread_mutex_lock)(pthread_mutex_t *);
static int (*real_pthread_mutex_unlock)(pthread_mutex_t *);

/* 
 * Find symbol values for pthread_mutex_lock and pthread_mutex_unlock
 * from the next library in the link chain.
 */
static void
init()
{
    static int inited;

    if (inited++ > 0) {
        return;
    }

    real_pthread_mutex_lock = dlsym(RTLD_NEXT, "pthread_mutex_lock");
    if (!real_pthread_mutex_lock) {
        printf("error in dlsym pthread_mutex_lock %s\n", dlerror());
        exit(-1);
    }

    real_pthread_mutex_unlock = dlsym(RTLD_NEXT, "pthread_mutex_unlock");
    if (!real_pthread_mutex_unlock) {
        printf("error in dlsym pthread_mutex_unlock %s\n", dlerror());
        exit(-1);
    }
}

/* Translate pc into source file+line */ 
static void
addr2line(void *pc)
{
    char addr2line_cmd[100];
    snprintf(addr2line_cmd, sizeof addr2line_cmd, 
            "addr2line -e /proc/%d/exe %p", getpid(), pc);
    printf("  -> ");
    fflush(stdout);
    system (addr2line_cmd);
}

/* Walk the stack and call 'addr2line' for each saved pc */
static void
print_backtrace(void (*addr2line)(void *), bool verbose)
{
    // source: http://lists.apple.com/archives/Darwin-dev/2005/May/msg00305.html
    void **fp = (void **) __builtin_frame_address (0);
    void *saved_fp = __builtin_frame_address (1);
    void *saved_pc = __builtin_return_address (1);
    int depth = 0;

    if (verbose)
        printf ("  [%d] pc == %p fp == %p\n", depth++, saved_pc, saved_fp);

    addr2line(saved_pc);
    fp = saved_fp;
    while (fp != NULL) {
        saved_fp = *fp;
        fp = saved_fp;
        if (*fp == NULL)
            break;
        saved_pc = *(fp + 1);
        if (verbose)
            printf ("  [%d] pc == %p fp == %p\n", depth++, saved_pc, saved_fp);
        addr2line(saved_pc);
    }
}

int 
pthread_mutex_lock(pthread_mutex_t *mutex)
{
    init();
    printf("pthread_mutex_lock: Locking mutex located at %p, called from\n", mutex);
    print_backtrace(addr2line, false);
    return real_pthread_mutex_lock(mutex);
}

int 
pthread_mutex_unlock(pthread_mutex_t *mutex)
{
    init();
    printf("pthread_mutex_unlock: Unlocking mutex located at %p, called from\n", mutex);
    print_backtrace(addr2line, false);
    return real_pthread_mutex_unlock(mutex);
}
