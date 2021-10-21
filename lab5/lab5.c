// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/lab5.h>
#include "video_gr.h"
#include <lcom/lab2.h>
#include "keyboard.h"
#include "i8042.h"

#include <stdint.h>
#include <stdio.h>

// Any header files included below this line should have been created by you

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab5/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(video_test_init)(uint16_t mode, uint8_t delay) {
  vg_init(mode);
  sleep(delay);
  vg_exit();
  return 0;
}

int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y,
                          uint16_t width, uint16_t height, uint32_t color) {
  vg_init(mode);
  vg_draw_rectangle(x, y, width, height, color);
  wait_for_ESC();
  vg_exit();

  return 0;
}

int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {
  vg_init(mode);
  draw_rect_pat(no_rectangles, first, step);
  wait_for_ESC();
  vg_exit();

  return 0;
}

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  vg_init(0x105);
  xpm_image_t img;
  uint8_t *map;
  map = xpm_load(xpm, XPM_INDEXED, & img);

  draw_pixmap(map, img, x, y);

  wait_for_ESC();
  vg_exit();
  return 0;
}

int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf,
                     int16_t speed, uint8_t fr_rate) {
 vg_init(0x105);
 xpm_image_t img;
 uint8_t *map;
 map = xpm_load(xpm, XPM_INDEXED, & img);

uint16_t img_width = img.width;
uint16_t img_height = img.height;
//printf("W: %d \n H: %d \n", img_width, img_height);

 typedef enum {DONE, XMOV_POS, YMOV_POS, XMOV_NEG, YMOV_NEG} mov_t;

 //float delay = 1 / (float) (fr_rate);
 uint8_t freq = 60;
 uint8_t wait_time = freq / fr_rate;
 uint8_t time_counter = 0;
 uint16_t pos_counter = 0;
 bool change_pos = true;

 uint16_t displacement = 0;
 if(speed >= 0){
   displacement = speed;
 }
 else{
   displacement = 1;
   change_pos = false;
 }

 mov_t movement = DONE;
 uint16_t x = xi, y = yi;

 if(xf == xi && yf != yi){
   if(yf - yi > 0) movement = YMOV_POS;
   else movement = YMOV_NEG;
 }
 else if (yf == yi && xf != xi){
   if(xf - xi > 0) movement = XMOV_POS;
   else movement = XMOV_NEG;
 }

 if(speed == 0) movement = DONE;

  int ipc_status;
  message msg;

  uint8_t kbd_bit = 0, timer_bit = 1;
  timer_subscribe_int(& timer_bit);
  kbd_subscribe(&kbd_bit);
  uint64_t irq_kbd = BIT(kbd_bit);
  uint64_t irq_timer = BIT(timer_bit);

  timer_set_frequency(0, freq);
  
  int r;

  extern uint8_t scancode;

 draw_pixmap(map, img, x, y);

 while(scancode != ESC_BREAK){
   time_counter = 0;
   while(time_counter < wait_time && scancode != ESC_BREAK){
     if ((r = driver_receive(ANY, &msg, &ipc_status)) != OK) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
        if (msg.m_notify.interrupts & irq_timer){
            time_counter++;
          }
          if (msg.m_notify.interrupts & irq_kbd) {
            (kbc_ih)();
          }
          break;
        default:
          break;
      }
    }
   }
   switch (movement){
    case XMOV_POS:
      if(change_pos || pos_counter == abs(speed)){
        pos_counter = 0;
        x += displacement;
        if (x + displacement > xf) x = xf;
      }
      else{
        pos_counter++;
      }
      vg_draw_rectangle_buffer(xi, yi, x-xi, img_height, 0);
      break;

    case YMOV_POS:
      if(change_pos || pos_counter == abs(speed)){
        pos_counter = 0;
        y += displacement;
        if (y + displacement > yf) y = yf;
      }
      else{
        pos_counter++;
      }
      vg_draw_rectangle_buffer(xi, yi, img_width, y-yi, 0);
      break;

    case XMOV_NEG:
      if(change_pos || pos_counter == abs(speed)){
        pos_counter = 0;
        if (displacement <= x){
          x -= displacement;
          if (x - displacement < xf) x = xf;
        }
        else x = xf;
      }
      else{
        pos_counter++;
      }
      vg_draw_rectangle_buffer(x + img_width, y, xi-x, img_height, 0);
      break;

    case YMOV_NEG:
      if(change_pos || pos_counter == abs(speed)){
        pos_counter = 0;
        if (displacement <= y){
          y -= displacement;
          if (y - displacement < yf) y = yf;
        }
        else y = yf;
      }
      else{
        pos_counter++;
      }
      vg_draw_rectangle_buffer(x, y + img_height, img_width, yi-y, 0);
      break;

    default:
      break;
  }
  printf("The Coordinates \n X: %d \n Y: %d \n", x, y);
  draw_pixmap_buffer(map, img, x, y);
  update_screen();
  if(x == xf && y == yf) movement = NONE;
 }

 timer_unsubscribe_int();
 kbd_unsubscribe();
 wait_for_ESC();
 vg_exit();
 return 0;
}

int(video_test_controller)() {
  vg_vbe_contr_info_t controller_info;
  vg_get_controller_info(& controller_info);
  vg_display_vbe_contr_info(& controller_info);
  return 0;
}
