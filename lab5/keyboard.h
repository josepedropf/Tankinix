#ifndef LAB3_KEYBOARD_H
#define LAB3_KEYBOARD_H

#include <lcom/lcf.h>

#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>
#include "i8042.h"


int (kbd_subscribe) (uint8_t *kbd_hook_bit);
int (keyboard_cmd_write) (int reg, int cmd, int tries);
int (keyboard_cmd_read) (uint8_t status, int tries);
void (timer_ih)();
void (kbc_ih)();
int (kbd_unsubscribe) ();


#endif //LAB3_KEYBOARD_H
