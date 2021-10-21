#ifndef LAB4_MOUSE_H
#define LAB4_MOUSE_H

#include <lcom/lcf.h>
#include "i8042.h"
/**
 * @brief enables mouse data reporting
 * 
 */
void (mouse_enable) ();
/**
 * @brief subscribes mouse interrupts
 * 
 */
int (mouse_subscribe_int)(uint8_t *bit_no);
/**
 * @brief unsubscribes mouse interrupts
 * 
 */
int (mouse_unsubscribe_int) ();
/**
 * @brief assemble a 3 bytes packet with the mouse provided information
 * 
 */
void (assemble_bytes)(int *counter, uint8_t pac[3]);
/**
 * @brief builds the struct packet using the 3 bytes array provided
 * 
 */
void (build_packet)(int *counter, uint8_t pac[3], uint32_t *cnt);
/**
 * @brief builds the packet and passes it by reference
 * 
 */
void (build_packet_ret)(int *counter, uint8_t pac[3], struct packet *p);
/**
 * @brief builds the packet without the limitation
 * 
 */
void (build_packet_nolim)(int *counter, uint8_t pac[3]);
/**
 * @brief handles mouse initerrupt
 * 
 */
void (mouse_ih) ();
/**
 * @brief disables mouse data reporting
 * 
 */
void (disable_mouse_data) ();
/**
 * @brief test function for buttons
 * 
 */
bool (check_but) (struct packet p);

#endif //LAB4_MOUSE_H
