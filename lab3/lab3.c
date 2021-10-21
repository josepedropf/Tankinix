#include <lcom/lcf.h>

#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>
#include "keyboard.h"
#include "i8042.h"

#include "utils.c"
#include "timer.h"

uint8_t scancode = 0;
int count = 0;
int timercounter = 0;


int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab3/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(kbd_test_scan)() {
  int ipc_status;
  message msg;

  uint8_t kbd_bit;
  kbd_subscribe(&kbd_bit);
  uint64_t irq_set = BIT(kbd_bit);

  int r;

  while(scancode != ESC_BREAK) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != OK) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & irq_set) {
            (kbc_ih)();
          }
          break;
        default:
          break;
      }
    }
  }

  timer_unsubscribe_int();
  kbd_unsubscribe(&kbd_bit);
  kbd_print_no_sysinb(count);
  return 0;
}

int(kbd_test_poll)() {
  uint8_t status = 0;
  count = 0;

  while (scancode != ESC_BREAK){
    kbc_ih();
  }

  //Restore
  keyboard_cmd_write(KBD_CMD_BUF, KBC_READ_CMD, 3);
  keyboard_cmd_read(status, 3);
  keyboard_cmd_write(KBD_CMD_BUF, KBC_WRITE_CMD, 3);
  keyboard_cmd_write(IN_BUF_STATUS, OUT_BUF_STATUS | scancode, 3);

  kbd_print_no_sysinb(count);

  return 0;
}

int(kbd_test_timed_scan)(uint8_t n) {
  //extern int counter;
  //counter = 0;
  int ipc_status;
  int idle = n;
  message msg;

  uint8_t bit1, bit2;
  kbd_subscribe(&bit2);
  timer_subscribe_int(&bit1);

  uint64_t kbd_int_bit = BIT(bit2);
  uint64_t timer0_int_bit = BIT(bit1);
  int r;

  while(scancode != ESC_BREAK && n > 0) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != OK) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & kbd_int_bit) {
            (kbc_ih)();
            n = idle;
          }
          if (msg.m_notify.interrupts & timer0_int_bit) {
            timer_ih();
            if (timercounter % 60 == 0) {
              timer_print_elapsed_time();
              timercounter = 0;
              n--;
            }
          }
          break;
        default:
          break;
      }
    }
  }

  timer_unsubscribe_int();
  kbd_unsubscribe(&bit2);
  return 0;
}
