#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int hook_id = 0;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  uint8_t contw;
  if(timer_get_conf(timer, & contw) != OK) return 1;
  contw <<= 4;
  contw >>= 4;
  contw |= TIMER_LSB_MSB | (timer << 6);
  if(sys_outb(TIMER_CTRL, contw) != OK) return 1;
  uint16_t initv = TIMER_FREQ/freq;
  uint8_t mail;
  util_get_LSB(initv, & mail);
  if(sys_outb(TIMER_0 + timer, mail) != OK) return 1;
  util_get_MSB(initv, & mail);
  if(sys_outb(TIMER_0 + timer, mail) != OK) return 1;
  return 0;
}

int (timer_subscribe_int)(uint8_t* bit_no) {
  //int hook_id is a global variable
  *bit_no = hook_id;
  if(sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, & hook_id) != OK) return 1;
  return 0;
}

int (timer_unsubscribe_int)() {
  if(sys_irqrmpolicy(& hook_id) != OK) return 1;
  return 0;
}

void (timer_int_handler)() {
  extern int counter;
  counter++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  uint32_t rb = 0;
  rb |= TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);
  if(sys_outb(TIMER_CTRL, rb) != OK) return 1;
  int port = TIMER_0 + timer;
  if(util_sys_inb(port, st) != OK) return 1;
  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {
  union timer_status_field_val conf;
  switch(field) {
    case tsf_all:
      conf.byte = st;
      break;
    case tsf_initial:
      st <<= 2;
      st >>= 6;
      switch (st){
        case 1:
          conf.in_mode = LSB_only;
          break;
        case 2:
          conf.in_mode = MSB_only;
          break;
        case 3:
          conf.in_mode = MSB_after_LSB;
          break;
        default:
          conf.in_mode = INVAL_val;
      }
      break;
    case tsf_mode:
        st <<= 4;
        st >>= 5;
        if(st == 6) st = 2;
        if(st == 7) st = 3;
        conf.count_mode = st;
        break;
    case tsf_base:
      st <<= 7;
      st >>= 7;
      conf.bcd = st;
      break;
  }
  if(timer_print_config(timer, field, conf) != OK) return 1;
  return 0;
}
