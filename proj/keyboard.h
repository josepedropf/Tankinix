#ifndef LAB3_KEYBOARD_H
#define LAB3_KEYBOARD_H

#include <lcom/lcf.h>

#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>
#include "i8042.h"


int (kbd_subscribe_int) (uint8_t *bit_no);
int (keyboard_cmd_write) (int reg, int cmd, int tries);
int (keyboard_cmd_read) (uint8_t status, int tries);
void (kbc_ih) ();
int (wait_for_ESC) ();
bool (check_key) (uint8_t key_code);
void (print_scancode) (uint16_t tries);
uint8_t (get_scancode) ();
int (kbd_unsubscribe_int) ();


#endif //LAB3_KEYBOARD_H
