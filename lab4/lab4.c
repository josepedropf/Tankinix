// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include "i8042.h"
#include "mouse.h"
#include <lcom/lab4.h>
#include "timer.h"

#include "keyboard.h"

#include <stdint.h>
#include <stdio.h>

int counter = 0;
uint8_t scancode = 0;

uint8_t packet[3];

// Any header files included below this line should have been created by you

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab4/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}


int (mouse_test_packet)(uint32_t cnt) {
  int ipc_status;
  message msg;
  uint8_t bit_no = 0;
  //mouse_enable_data_reporting();
  mouse_enable ();

  mouse_subscribe_int(& bit_no);
  //mouse_enable();

  uint64_t irq_set = BIT(bit_no);
  int r;

  while(cnt > 0){
    if((r = driver_receive(ANY, &msg, &ipc_status)) != OK){
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if(is_ipc_notify(ipc_status)){
      switch (_ENDPOINT_P(msg.m_source)){
        case HARDWARE:
          if (msg.m_notify.interrupts & irq_set){
            mouse_ih();
            assemble_bytes(&counter, packet);
            build_packet(&counter, packet, &cnt);
          }
          break;
        default:
          break;
      }
    }
  }
  mouse_unsubscribe_int();
  disable_mouse_data();
  return 0;
}

int (mouse_test_async)(uint8_t idle_time) {
  int ipc_status;
  int idle = idle_time;
  message msg;

  uint8_t bit_no_timer = 0, bit_no_mouse = MOUSE_HOOK_BIT;
  //mouse_enable_data_reporting();
  mouse_enable();

  mouse_subscribe_int(&bit_no_mouse);
  timer_subscribe_int(&bit_no_timer);

  counter = 0;
  int mousecounter = 0;

  uint64_t irq_set_mouse = BIT(bit_no_mouse);
  uint64_t irq_set_timer = BIT(bit_no_timer);
  int r;

  int freq = sys_hz();
  printf("Freq: %d  \n", freq);
  printf("Init Idle: %d \n", idle);

  while(idle_time > 0) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != OK) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & irq_set_mouse) {
            printf("MOUSE NOTIFICATION  \n");
            printf("Idle: %d \n", idle_time);
            mouse_ih ();
            assemble_bytes(&mousecounter, packet);
            build_packet_nolim(&mousecounter, packet);
            idle_time = idle;
            counter = 0;
          }
          if (msg.m_notify.interrupts & irq_set_timer) {
            printf("TIMER NOTIFICATION  Counter: %d \n", counter);
            timer_int_handler();
            if (counter % freq == 0) {
              timer_print_elapsed_time();
              counter = 0;
              idle_time--;
            }
          }
          break;
        default:
          break;
      }
    }
  }

  timer_unsubscribe_int();
  mouse_unsubscribe_int();
  disable_mouse_data();
  return 0;
}

int (mouse_test_gesture)(uint8_t x_len, uint8_t tolerance) {
  int ipc_status;
  message msg;
  uint8_t bit_no = MOUSE_HOOK_BIT, kbd_hook = 1;
  typedef enum {INIT, FREE, DRAW_UP, VERT, DRAW_DWN, END} state_t;
  state_t state = INIT;
  //mouse_enable_data_reporting();
  mouse_enable ();
  struct packet pck;
  int mousecounter = 0;
  mouse_subscribe_int(& bit_no);
  kbd_subscribe(& kbd_hook);

  float slope = 0;
  int x = 0;
  int y = 0;
  bool pbuilt = false;

  uint64_t irq_set = BIT(bit_no), irq_kbd = BIT(kbd_hook);
  int r;

  while(state != END && scancode != ESC_BREAK){
    if((r = driver_receive(ANY, &msg, &ipc_status)) != OK){
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if(is_ipc_notify(ipc_status)){
      switch (_ENDPOINT_P(msg.m_source)){
        case HARDWARE:
          if (msg.m_notify.interrupts & irq_kbd){
            printf("KB INT \n");
            kbc_ih();
          }
          if (msg.m_notify.interrupts & irq_set){
            mouse_ih();
            assemble_bytes(&mousecounter, packet);
            if (mousecounter == 3){
              build_packet_ret(&mousecounter, packet, &pck);
              pbuilt = true;
            }
            if(pbuilt){
              printf("(Before) State : %d \n", state);
              switch (state){

                case INIT:
                  if(!check_but(pck)) state = FREE;
                  break;

                case FREE:
                  if(pck.lb == 1 && (pck.rb | pck.mb) == 0) state = DRAW_UP;
                  else if (!check_but(pck)) state = FREE;
                  else state = INIT;
                  x = 0;
                  y = 0;
                  break;
            
                case DRAW_UP:
                  if(x >= x_len){
                    if(!check_but(pck)) state = VERT;
                    break;
                  }
                  if(pck.lb != 1){
                    if (!check_but(pck)) state = FREE;
                    else state = INIT;
                    break;
                  }
                  if((pck.delta_x <= 0 && abs(pck.delta_x) > tolerance) || (pck.delta_y <= 0 && abs(pck.delta_y) > tolerance)){
                    if (!check_but(pck)) state = FREE;
                    else state = INIT;
                    break;
                  }
                  x += pck.delta_x;
                  y += pck.delta_y;
                  slope = ((float)(y) / (float)(x));
                  if(slope < 1){
                    if (!check_but(pck)) state = FREE;
                    else state = INIT;
                  }
                  break;

                case VERT:
                  if(pck.rb == 1 && (pck.lb | pck.mb) == 0) {
                    state = DRAW_DWN;
                    x = 0;
                    y = 0;
                    break;
                  }
                  else if((pck.lb | pck.mb) == 1){
                    state = INIT;
                    break;
                  }
            
                  if(abs(pck.delta_x) > tolerance || abs(pck.delta_y) > tolerance){
                    if (!check_but(pck)) state = FREE;
                    else state = INIT;
                  }
                  break;

                case DRAW_DWN:
                  printf("X: %d \n Y: %d \n DeltaX: %d \n DeltaY: %d \n", x, y, pck.delta_x, pck.delta_y);
                  if(x >= x_len){
                    if(!check_but(pck)) state = END;
                    break;
                  }
                  if(pck.rb != 1){
                    printf("NO RB \n");
                    if (!check_but(pck)) state = FREE;
                    else state = INIT;
                    break;
                  }
                  if((pck.delta_x <= 0 && abs(pck.delta_x) > tolerance) || (pck.delta_y >= 0 && abs(pck.delta_y) > tolerance)){
                    printf("Death by Delta \n");
                    printf("Dx: %d \n Dy: %d \n", pck.delta_x, pck.delta_y);
                    if (!check_but(pck)) state = FREE;
                    else state = INIT;
                    break;
                  }
                  x += pck.delta_x;
                  y += pck.delta_y;
                  slope = -((float)(y) / (float)(x));
                  if(slope < 1){
                    printf("Death by Slope \n");
                    printf("Slope: %f \n, slope");
                    printf("X: %d \n Y: %d \n DeltaX: %d \n DeltaY: %d \n", x, y, pck.delta_x, pck.delta_y);
                    if (!check_but(pck)) state = FREE;
                    else state = INIT;
                  }
                  break;

                default:
                  break;
              }
              pbuilt = false;
              printf("(After) State : %d \n", state);
            }
          }
          break;
        default:
          break;
      }
    }
  }
  mouse_unsubscribe_int();
  kbd_unsubscribe();
  disable_mouse_data();
  return 0;
}

int (mouse_test_remote)(uint16_t period, uint8_t cnt) {
    /* To be completed */
    printf("%s(%u, %u): under construction\n", __func__, period, cnt);
    return 1;
}
