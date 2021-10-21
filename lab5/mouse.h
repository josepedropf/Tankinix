#ifndef LAB4_MOUSE_H
#define LAB4_MOUSE_H

#include <lcom/lcf.h>
#include "i8042.h"

void (mouse_enable) ();
int (mouse_subscribe_int)(uint8_t *mouse_hook_bit);
int (mouse_unsubscribe_int) ();
void (assemble_bytes)(int *counter, uint8_t pac[3]);
void (build_packet)(int *counter, uint8_t pac[3], uint32_t *cnt);
void (build_packet_ret)(int *counter, uint8_t pac[3], struct packet *p);
void (build_packet_nolim)(int *counter, uint8_t pac[3]);
void (mouse_ih) ();
void (disable_mouse_data) ();
bool (check_but) (struct packet p);

#endif //LAB4_MOUSE_H
