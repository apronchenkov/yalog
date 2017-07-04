#ifndef __yalog_spinlock_h__
#define __yalog_spinlock_h__ 1

#if __APPLE__
#include <libkern/OSAtomic.h>
typedef OSSpinLock YalogSpinlock;

static inline void YalogSpinInit(YalogSpinlock *spinlock) { spinlock = 0; }

static inline void YalogSpinLock(YalogSpinlock *spinlock) {
  OSSpinLockLock(spinlock);
}

static inline void YalogSpinUnlock(YalogSpinlock *spinlock) {
  OSSpinLockUnlock(spinlock);
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
#endif  //_yalog_spinlock_h__
