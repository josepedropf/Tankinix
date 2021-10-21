#include "keyboard.h"

int kbd_hook_id;
uint8_t status;
uint8_t bytes[2];
extern uint8_t scancode;

int (kbd_subscribe_int) (uint8_t* bit_no) {
  kbd_hook_id = *bit_no;
  if (sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, & kbd_hook_id) != OK) return 1;  
  return 0;
}

int (kbd_unsubscribe_int) () {
  if(sys_irqrmpolicy(& kbd_hook_id) != OK) return 1;
  return 0;
}

int (keyboard_cmd_write) (int reg, int cmd, int tries){
  for(int i = 0; i < tries; i--){ // loop while 8042 input buffer is not empty max [tries] times, then exit
    if(util_sys_inb(STATUS_PORT, &status) != OK) return 1;
    if((status & IN_BUF_STATUS) == 0){
      sys_outb(reg, cmd);
      return 0;
    }
  }
  return 0;
}

int (keyboard_cmd_read) (uint8_t status, int tries){
  for(int i = 0; i < tries; i--){
    if(util_sys_inb(STATUS_PORT, &status) != OK)
      return 1;
    if(status & OUT_BUF_STATUS){
      if(util_sys_inb(KBD_OUT_BUF, &scancode) != OK)
      return 1;
      if((status & (TIME_OUT_ERROR | PARITY_ERROR)) == 0)
        return 0;
      else return 1;
    }
  }
  return 1;
}

void (kbc_ih) () {
  bool make;
  if(keyboard_cmd_read(status, 3) == 0){
   uint8_t mk1 = scancode >> 7; // shift value to stay with make

   if(!mk1)
     make = true;
   else
     make = false;
  }

  bytes[0] = scancode;

  if(scancode != TWOB_CODE){
    //kbd_print_scancode(make, 1, bytes);
  }
  else{
    scancode = keyboard_cmd_read(status, 3);
    bytes[1] = scancode;
    //kbd_print_scancode(make, 2, bytes);
  }
}

int (wait_for_ESC)() {
  int ipc_status;
  message msg;

  uint8_t kbd_bit;
  kbd_subscribe_int(&kbd_bit);
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

  kbd_unsubscribe_int();
  return 0;
}


bool (check_key)(uint8_t key_code){
  int ipc_status;
  message msg;

  uint8_t kbd_bit;
  kbd_subscribe_int(&kbd_bit);
  uint64_t irq_set = BIT(kbd_bit);

  int r;
  bool check = false;

  if ((r = driver_receive(ANY, &msg, &ipc_status)) != OK) {
    printf("driver_receive failed with: %d", r);
  }
  if (is_ipc_notify(ipc_status)) {
    switch (_ENDPOINT_P(msg.m_source)) {
      case HARDWARE:
        if (msg.m_notify.interrupts & irq_set) {
          (kbc_ih)();
          if(scancode == key_code) check = true;
        }
        break;
      default:
        break;
    }
  }

  kbd_unsubscribe_int();
  return check;
}

void (print_scancode)(uint16_t tries){
  int ipc_status;
  message msg;

  uint8_t kbd_bit;
  kbd_subscribe_int(&kbd_bit);
  uint64_t irq_set = BIT(kbd_bit);

  int r;

  while(scancode != ESC_BREAK && tries > 0) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != OK) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & irq_set) {
            (kbc_ih)();
            printf("\n ScanCode (Decimal): %d", scancode);
            printf("\n ScanCode (Hexadecimal): %X \n", scancode);
            tries--;
          }
          break;
        default:
          break;
      }
    }
  }

  kbd_unsubscribe_int();
}


uint8_t (get_scancode)(){
  int ipc_status;
  message msg;

  uint8_t kbd_bit;
  kbd_subscribe_int(&kbd_bit);
  uint64_t irq_set = BIT(kbd_bit);

  int r;
  uint8_t code = 0xFF;

  if ((r = driver_receive(ANY, &msg, &ipc_status)) != OK) {
    printf("driver_receive failed with: %d", r);
  }
  if (is_ipc_notify(ipc_status)) {
    switch (_ENDPOINT_P(msg.m_source)) {
      case HARDWARE:
        if (msg.m_notify.interrupts & irq_set) {
          (kbc_ih)();
          code = scancode;
        }
        break;
      default:
        break;
    }
  }

  kbd_unsubscribe_int();
  return code;
}

