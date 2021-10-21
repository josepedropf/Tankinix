#include "keyboard.h"

int kbd_bit;
uint8_t status;
uint8_t bytes[2];
extern int count;
extern uint8_t scancode;

int (kbd_subscribe) (uint8_t *kbd_hook_bit) {
  *kbd_hook_bit = kbd_bit;
  if (sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &kbd_bit) == OK) {
      return 0;
    }
  else return 1;
}

int (kbd_unsubscribe) () {
  if(sys_irqrmpolicy(&kbd_bit) != OK) return 1;
  return 0;
}

int (keyboard_cmd_write) (int reg, int cmd, int tries){
  for(int i = 0; i < tries; i--){ // loop while 8042 input buffer is not empty max [tries] times, then exit
    #ifdef LAB3
    count++;
    #endif
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
    #ifdef LAB3
    count++;
    #endif
    if(util_sys_inb(STATUS_PORT, &status) != OK)
      return 1;
    if(status & OUT_BUF_STATUS){
      #ifdef LAB3
      count++;
      #endif
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
