/* SPDX-License-Identifier: BSD-2-Clause */

/*
 * This is a TEMPLATE. The actual config file is generated by CMake and stored
 * in <BUILD_DIR>/tilck_gen_headers/.
 */

#pragma once
#include <tilck_gen_headers/config_global.h>

#define MAX_HANDLES            @MAX_HANDLES@

/* disabled by default */
#cmakedefine01 KERNEL_DO_PS2_SELFTEST
#cmakedefine01 KERNEL_BIG_IO_BUF
#cmakedefine01 KERNEL_FORCE_TC_ISYSTEM

/*
 * --------------------------------------------------------------------------
 *                  Hard-coded global & derived constants
 * --------------------------------------------------------------------------
 *
 * Here below there are some pseudo-constants not designed to be easily changed
 * because of the code makes assumptions about them. Because of that, those
 * constants are hard-coded and not available as CMake variables. With time,
 * some of those constants get "promoted" and moved in CMake, others remain
 * here. See the comments and think about the potential implications before
 * promoting a hard-coded constant to a configurable CMake variable.
 */

#define KERNEL_STACK_SIZE          (KERNEL_STACK_PAGES << PAGE_SHIFT)
#define KMALLOC_MAX_ALIGN          (64 * KB)
#define KMALLOC_MIN_HEAP_SIZE      KMALLOC_MAX_ALIGN

#define MAX_MOUNTPOINTS                            16
#define MAX_NESTED_INTERRUPTS                      32
#define WTH_MAX_THREADS                            64
#define WTH_MAX_PRIO_QUEUE_SIZE                    40
#define WTH_KB_QUEUE_SIZE                          80



#define MAX_SCRIPT_REC                                          2

#define USERAPP_MAX_ARGS_COUNT                                 32
#define USER_ARGS_PAGE_COUNT                                    1

#if KERNEL_BIG_IO_BUF
   #define IO_COPYBUF_PAGE_COUNT                               63
#else
   #define IO_COPYBUF_PAGE_COUNT                                3
#endif

#define IO_COPYBUF_SIZE       (IO_COPYBUF_PAGE_COUNT * PAGE_SIZE)
#define ARGS_COPYBUF_SIZE      (USER_ARGS_PAGE_COUNT * PAGE_SIZE)

