#pragma once

#if __APPLE__
#include <os/lock.h>
typedef os_unfair_lock YalogSpinlock;

#define YALOG_SPINLOCK_INIT OS_UNFAIR_LOCK_INIT

static inline void YalogSpinInit(YalogSpinlock *spinlock) {
  *spinlock = OS_UNFAIR_LOCK_INIT;
}

static inline void YalogSpinLock(YalogSpinlock *spinlock) {
  os_unfair_lock_lock(spinlock);
}

static inline void YalogSpinUnlock(YalogSpinlock *spinlock) {
  os_unfair_lock_unlock(spinlock);
}

#else
#include <pthread.h>
#ifdef PTHREAD_SPINLOCK_INITIALIZER

typedef pthread_spinlock_t YalogSpinlock;

#define YALOG_SPINLOCK_INIT PTHREAD_SPINLOCK_INITIALIZER

static inline void YalogSpinInit(YalogSpinlock *spinlock) {
  pthread_spin_init(spinlock, PTHREAD_PROCESS_PRIVATE);
}

static inline void YalogSpinLock(YalogSpinlock *spinlock) {
  pthread_spin_lock(spinlock);
}

static inline void YalogSpinUnlock(YalogSpinlock *spinlock) {
  pthread_spin_unlock(spinlock);
}

#else
#include <sched.h>
#include <stdatomic.h>

typedef atomic_flag YalogSpinlock;

#define YALOG_SPINLOCK_INIT ATOMIC_FLAG_INIT

static inline void YalogSpinInit(YalogSpinlock *spinlock) {
  atomic_flag_clear(spinlock);
}

static inline void YalogSpinLock(YalogSpinlock *spinlock) {
    for (int i = 40; i--;) {
        if (!atomic_flag_test_and_set_explicit(spinlock, memory_order_acquire)) {
            return;
        }
    }
    while (atomic_flag_test_and_set_explicit(spinlock, memory_order_acquire)) {
        sched_yield();
    }
}

static inline void YalogSpinUnlock(YalogSpinlock *spinlock) {
  atomic_flag_clear_explicit(spinlock, memory_order_release);
}

#endif
#endif
