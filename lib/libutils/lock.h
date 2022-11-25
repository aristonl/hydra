/*
 * Copyright (c) 2021 Ariston Lorenzo. All rights reserved
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "../LibC/stddef.h"
#include "../LibC/stdint.h"

namespace Utils {
enum lock_state {
	LOCK_UNLOCKED = 0,
	LOCK_LOCKED = 1,
};
class lock_type {
    bool raw;
public:
    lock_type()
        : raw(LOCK_UNLOCKED) {}; // The lock is unlocked by default

	bool is_locked() const {
        bool res;
		__atomic_load(&raw, &res, __ATOMIC_SEQ_CST);
		return res;
    }

	void lock() {
		while (!__sync_bool_compare_and_swap(&raw, LOCK_UNLOCKED, LOCK_LOCKED)) {
			while (is_locked()) {
				asm volatile("pause"); // Don't burn the CPU
			}
		}

		__sync_synchronize();
	};

	void force_lock() {
		raw = LOCK_LOCKED;

		__sync_synchronize();
	}

	void unlock() {
        __atomic_store_n(&raw, LOCK_UNLOCKED, __ATOMIC_SEQ_CST);
		raw = LOCK_UNLOCKED;
    };
};

class context_lock {
    lock_type &raw;
public:
	context_lock(lock_type &value)
		: raw(value) {
		raw.lock();
	}
	~context_lock() {
		raw.unlock();
	}
};
}