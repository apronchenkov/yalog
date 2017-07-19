#pragma once

#if __APPLE__
#include <os/lock.h>
typedef os_unfair_lock YalogSpinlock;

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
typedef pthread_spinlock_t YalogSpinlock;

static inline void YalogSpinInit(YalogSpinlock *spinlock) {
  pthread_spin_init(spinlock, PTHREAD_PROCESS_PRIVATE);
}

static inline void YalogSpinLock(YalogSpinlock *spinlock) {
  pthread_spin_lock(spinlock);
}

static inline void YalogSpinUnlock(YalogSpinlock *spinlock) {
  pthread_spin_unlock(spinlock);
}
#endif
