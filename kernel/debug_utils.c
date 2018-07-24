
#include <exos/common/basic_defs.h>
#include <exos/common/string_util.h>
#include <exos/common/arch/generic_x86/cpu_features.h>

volatile bool __in_panic;

#ifndef UNIT_TEST_ENVIRONMENT

#include <exos/kernel/debug_utils.h>
#include <exos/kernel/hal.h>
#include <exos/kernel/irq.h>
#include <exos/kernel/process.h>
#include <exos/kernel/term.h>
#include <exos/kernel/fb_console.h>
#include <exos/kernel/elf_utils.h>
#include <exos/kernel/arch/generic_x86/textmode_video.h>

#include <elf.h>
#include <multiboot.h>

void panic_save_current_state();

NORETURN void panic(const char *fmt, ...)
{
   disable_interrupts_forced(); /* No interrupts: we're in a panic state */

   if (__in_panic) {
      goto end;
   }

   __in_panic = true;

   x86_cpu_features.can_use_sse = false;
   x86_cpu_features.can_use_sse2 = false;
   x86_cpu_features.can_use_avx = false;
   x86_cpu_features.can_use_avx2 = false;

   panic_save_current_state();

   if (!term_is_initialized()) {
      if (use_framebuffer())
         init_framebuffer_console(in_hypervisor());
      else
         init_textmode_console(in_hypervisor());
   }


   printk("*********************************"
          " KERNEL PANIC "
          "********************************\n");

   va_list args;
   va_start(args, fmt);
   vprintk(fmt, args);
   va_end(args);

   printk("\n");

   task_info *curr = get_curr_task();

   if (curr && curr != kernel_process && curr->tid != -1) {
      if (!is_kernel_thread(curr)) {
         printk("Current task [USER]: tid: %i, pid: %i\n",
                curr->tid, curr->owning_process_pid);
      } else {
         ptrdiff_t off;
         const char *s = find_sym_at_addr_safe((uptr)curr->what, &off, NULL);
         printk("Current task [KERNEL]: tid: %i [%s]\n",
                curr->tid, s ? s : "???");
      }
   } else {
      printk("Current task: NONE\n");
   }

   panic_dump_nested_interrupts();
   //dump_regs(curr->state_regs);
   dump_stacktrace();


#ifdef DEBUG_QEMU_EXIT_ON_PANIC
   debug_qemu_turn_off_machine();
#endif

end:

   while (true) {
      halt();
   }
}

#define DUMP_STR_OPT(opt)  printk(NO_PREFIX "%-35s: %s\n", #opt, opt)
#define DUMP_BOOL_OPT(opt) printk(NO_PREFIX "%-35s: %u\n", #opt, opt)

void debug_show_build_opts(void)
{
   printk(NO_PREFIX "\n");
   printk(NO_PREFIX "------------------- BUILD OPTIONS ------------------\n");
   DUMP_STR_OPT(BUILDTYPE_STR);
   DUMP_BOOL_OPT(KERNEL_TRACK_NESTED_INTERRUPTS);
   DUMP_BOOL_OPT(TERM_PERF_METRICS);
   DUMP_BOOL_OPT(KMALLOC_FREE_MEM_POISONING);
   DUMP_BOOL_OPT(KMALLOC_SUPPORT_DEBUG_LOG);
   DUMP_BOOL_OPT(KMALLOC_SUPPORT_LEAK_DETECTOR);
   DUMP_BOOL_OPT(KMALLOC_HEAPS_CREATION_DEBUG);
   DUMP_BOOL_OPT(BOOTLOADER_POISON_MEMORY);
   DUMP_BOOL_OPT(DEBUG_CHECKS_IN_RELEASE_BUILD);
   printk(NO_PREFIX "\n");
}

#endif // UNIT_TEST_ENVIRONMENT
