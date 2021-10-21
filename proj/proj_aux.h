#include <stdbool.h>
#include <stdint.h>

#include "sprite.h"
#include "rtc.h"

typedef enum {DEATHMATCH, SURVIVAL, GOAL} LevelType_t;
typedef enum {NO_C, CX, CY, NCX, NCY} Coordinate_t;
typedef enum {NO_COLISION, TANK_C, PLAYER_C, BALL_C, DESTRUCT_C, SOLID_C, REBOUND_CX, REBOUND_CY, MINE_C} Colision_t;
typedef enum {FLOOR = 55, WALL = 20, PLAYER_TILE = 200, XTANK_U_TILE = 201, XTANK_D_TILE = 202, YTANK_R_TILE = 203, YTANK_L_TILE = 204, RSTANK_TILE = 205, DSTANK_TILE = 206, ISTANK_TILE = 207} Tile_t;
typedef enum {TIMER_INT, KBD_INT, MOUSE_INT} Interrupt_t;
typedef enum {XP, YP_XP, VP, YP_XN, XN, YN_XN, VN, YN_XP} Aim_Quadrant_t;
typedef enum {ALIVE, BOOM, DESTROYED} State_t;
typedef enum {TPLAYER, XTANK, YTANK, RANDOM_STATIC_TANK, DIRECT_STATIC_TANK, INDIRECT_STATIC_TANK} TankType_t;
typedef enum {NO_FIRE, NORMAL_AIM, SHOOTING} AimStatus_t;

#define F FLOOR
#define W WALL
#define P PLAYER_TILE
#define XU XTANK_U_TILE
#define XD XTANK_D_TILE
#define YR YTANK_R_TILE
#define YL YTANK_L_TILE
#define RS RSTANK_TILE
#define DS DSTANK_TILE
#define IS ISTANK_TILE

typedef struct{
  AnimSprite asp;
  bool ingame;
  AimStatus_t status;
} Aim_t;

typedef struct{
  AnimSprite asp;
  double angle;
  double vel;
  double vx, vy;
  int rebounds;
  int strength;
  int id;
  State_t state;
} Ball_t;

typedef struct{
  AnimSprite asp;
  AnimSprite * basp;
  double slope;
  double angle;
  uint8_t cooldown_count;
  uint8_t cooldown_frames;
  uint8_t ball_count;
  uint8_t max_balls;
  bool aim_right;
  bool aim_up;
  double ball_vel;
  int ball_rebounds;
  int ball_strength;
  int current_ball_id;
  int hits;
  int score_value;
  int last_fig;
  int i_frames;
  Aim_Quadrant_t aim_quadrant;
  Ball_t * balls;
  State_t state;
  TankType_t type;
  Aim_t * aim;
} Tank_t;

typedef struct{
  char * name;
  int lives;
  int score;
  Tank_t * PlayerTank;
} Player_t;

typedef struct{
  uint16_t x;
  uint16_t y;
  uint16_t width;
  uint16_t height;
  char * text;
  uint16_t color;
  uint16_t frame_color;
  int text_w;
  int text_h;
  bool clickable;
} Box_t;

typedef struct{
  uint16_t minutes;
  uint16_t seconds;
  int timer_tot;
  bool increasing;
} Clock_t;

typedef struct{
  LevelType_t type;
  Tile_t layout[NROWS][NCOLS];
  Clock_t clock;
  bool passed;
} Level_t;

typedef struct{
  uint16_t background[BACKGROUND_NTILES][BACKGROUND_NTILES];
  int num_boxes;
  Box_t * boxes;
} Menu_t;

void (game_loop)();


void (game_init)(Level_t * Game_Levels[MAX_LEVELS], Menu_t * Game_Menus[MAX_MENUS]);
void (play_level)(Menu_t * Game_Menus[MAX_MENUS], Level_t * Game_Levels[MAX_LEVELS], int level_number, Player_t * Player, Tank_t OG_PlayerTank, Aim_t OG_Aim);

int (add_tank)(Tank_t * tanks[MAX_TANKS], Tank_t * tank);
int (destroy_tank)(Tank_t * tanks[MAX_TANKS], int index);

bool (check_tank_aim_colision)(Tank_t * tank);

void(test)();

void (layout_init)(Tile_t layout_dest[][NCOLS], Tile_t level_to_copy[][NCOLS]);

void (draw_level)(Tile_t layout[][NCOLS], Tank_t * tanks[MAX_TANKS], Player_t * Player, Clock_t clock);
void (draw_screen)(Tile_t layout[][NCOLS], Tank_t * tanks[MAX_TANKS], Player_t * Player, Box_t menu_box, Clock_t clock);

void (print_mov)();

void (update_player_pos)(Tile_t layout[][NCOLS], Tank_t * PlayerTank, Tank_t * tanks[MAX_TANKS]);
void (update_balls_positions) (Tank_t * tanks[MAX_TANKS], Tile_t layout[][NCOLS]);

void (game_ih) (Menu_t * Game_Menus[MAX_MENUS], Interrupt_t int_type, Tile_t layout[][NCOLS], Tank_t * tanks[MAX_TANKS], Player_t * Player, bool * retry, bool * game_over, Box_t menu_box, bool * ingame_menu, bool * level_beat, Clock_t clock, LevelType_t level_type);
void (frame_loop)(Menu_t * Game_Menus[MAX_MENUS], Tile_t layout[][NCOLS], Tank_t * tanks[MAX_TANKS], Player_t * Player, bool * retry, bool * game_over, Box_t menu_box, bool * ingame_menu, bool * level_beat, Clock_t clock, LevelType_t level_type);

void (check_kbd)();
void (check_mouse_b)();

void (update_aim)(Tile_t layout[][NCOLS], Tank_t * tank);

Tank_t *(create_new_PlayerTank)(AnimSprite asp, Aim_t * aim, uint8_t cooldown_frames, uint8_t max_balls, int ball_rebounds, int ball_strength, double ball_vel, int i_frames, int hits);
Tank_t *(create_new_EnemyTank)(AnimSprite asp, AnimSprite * basp, uint8_t cooldown_frames, uint8_t max_balls, int ball_rebounds, int ball_strength, double ball_vel, TankType_t type, int hits, int score_value, int i_frames);

Menu_t *(create_newmenu) (uint16_t background[BACKGROUND_NTILES][BACKGROUND_NTILES], int num_boxes, Box_t box, ...);
Box_t *(create_newbox)(uint16_t x, uint16_t y, uint16_t color, uint16_t frame_color, bool clickable, char * text);
Level_t *(create_newlevel)(LevelType_t type, Tile_t layout[NROWS][NCOLS], int init_time);
Clock_t *(create_newclock)(bool increasing, int init_time);
Aim_t *(create_newaim)(AnimSprite asp);
Ball_t *(create_newball)(Tank_t * tank, bool real);
Player_t *(create_newplayer)(Tank_t * PlayerTank, int lives, char * name);
Player_t *(create_newplayer_spec)(AnimSprite asp, Aim_t * aim, uint8_t cooldown_frames, uint8_t max_balls, int ball_rebounds, int ball_strength, double ball_vel, int i_frames, int hits, int lives, char * name);

void (ball_init_position)(Tank_t * tank, Ball_t * ball);
void (shoot)(Tank_t * tank);
int (destroy_ball)(Tank_t * tank, Ball_t * ball);

Colision_t (check_ball_colision) (Tank_t * tank, Ball_t * ball, Tank_t * tanks[MAX_TANKS], Tile_t layout[][NCOLS], bool real);

Coordinate_t (check_colision_coord) (Ball_t * ball, Tile_t layout[][NCOLS], int * frontier);

bool (check_colision)(Tank_t * PlayerTank, Tile_t layout[][NCOLS], Coordinate_t coord, int *correct, Tank_t * tanks[MAX_TANKS]);
bool (valid_mouse_mov)(AnimSprite * aim, uint16_t deltax, uint16_t deltay);

void (draw_balls)(Tank_t * tanks[MAX_TANKS]);

void (update_aim_status) (Tile_t layout[][NCOLS], Tank_t * PlayerTank);
bool (check_shooting_position) (Tile_t layout[][NCOLS], Tank_t * PlayerTank);
void (update_tank_sprite) (Tank_t * PlayerTank);

void (get_tank_centre) (Tank_t * tank, double * x, double * y);

void (set_angle_slope) (double x1, double y1, double x2, double y2, double * angle, double * slope, Aim_Quadrant_t * aim_quadrant, bool * aim_right, bool * aim_up);
void (set_angle_slope_tank) (Tank_t * tank, double x2, double y2);
void (set_slope_tank) (Tank_t * tank);
void (point_to_tank) (Tank_t * tank, Tank_t * target);

void (update_tanks_pos)(Tile_t layout[][NCOLS], Tank_t * tanks[MAX_TANKS], Player_t * Player);
void (update_tanks_shooting)(Tile_t layout[][NCOLS], Tank_t * tanks[MAX_TANKS], Player_t * Player);
void (draw_tanks)(Tank_t * tanks[MAX_TANKS]);
Colision_t (check_tank_colision)(Tank_t * tank, Tile_t layout[][NCOLS], Tank_t * tanks[MAX_TANKS], int xcorrection, int ycorrection);

void (create_new_XTank)(Tank_t * tanks[MAX_TANKS], bool up, uint16_t x_init, uint16_t y_init);
void (create_new_YTank)(Tank_t * tanks[MAX_TANKS], bool right, uint16_t x_init, uint16_t y_init);
void (create_new_RSTank)(Tank_t * tanks[MAX_TANKS], double angle_init, uint16_t x_init, uint16_t y_init);
void (create_new_DSTank)(Tank_t * tanks[MAX_TANKS], double angle_init, uint16_t x_init, uint16_t y_init);
void (create_new_ISTank)(Tank_t * tanks[MAX_TANKS], double angle_init, uint16_t x_init, uint16_t y_init);

bool (check_tank_tank_colision)(Tank_t * tank1, Tank_t * tank2, int xcorrection, int ycorrection);

bool (check_player_hit) (Tank_t * tank, Ball_t * temp_ball, Tank_t * tanks[MAX_TANKS], Tile_t layout[][NCOLS]);

bool (check_direct_hit) (Tank_t * tank, Tank_t * tanks[MAX_TANKS], Tile_t layout[][NCOLS]);
bool (check_indirect_hit) (Tank_t * tank, Tank_t * tanks[MAX_TANKS], Tile_t layout[][NCOLS]);
bool (try_indirect_shot) (Tank_t * tank, Tank_t * tanks[MAX_TANKS], Tile_t layout[][NCOLS]);

void (draw_letter) (uint16_t x, uint16_t y, bool alt, char c);
void (draw_letter_test) (uint16_t x, uint16_t y, bool alt, int ind);
void (draw_string) (uint16_t x, uint16_t y, bool alt, char * string);

void (draw_clock) (uint16_t x, uint16_t y, bool alt, Clock_t * clock);

void (create_levels) (Level_t * Game_Levels[MAX_LEVELS]);
void (reset_mouse_info)();

void (draw_box) (Box_t box, bool mouse_over);
void (background_init)(uint16_t bg_dest[][BACKGROUND_NTILES], uint16_t bg_to_copy[][BACKGROUND_NTILES]);

void (draw_bg)(uint16_t bg[][BACKGROUND_NTILES]);

void (update_mouse) (Tank_t * tank);
bool (mouse_over_box) (Player_t * Player, Box_t box);

void (draw_menu) (Menu_t * menu, Player_t * Player, bool * transparent, bool rtc_d);
int (menu_loop) (Menu_t * menu, Player_t * Player, bool * transparent, bool rtc_d, bool click);
void (create_menus) (Menu_t * Game_Menus[MAX_MENUS]);

int (align_mid) (size_t lstr);

void (menu_ih)(Menu_t * Game_Menus[MAX_MENUS], Interrupt_t int_type, Level_t * Game_Levels[MAX_LEVELS], Player_t * Player, int * menu_sel, bool * quit, bool * lets_play, bool * transparent, int * level_number);
void (menu_select)(int * menu_sel, Menu_t * Game_Menus[MAX_MENUS], Player_t * Player, bool * lets_play, bool * quit, bool * transparent, int * level_number);

void (main_menu)(Menu_t * Game_Menus[MAX_MENUS], Level_t * Game_Levels[MAX_LEVELS], Player_t * Player);

void (update_clock) (Clock_t * clock);

void (make_rtc) (rtc_time rtc, char * string_rtc);
void (draw_rtc) (uint16_t x, uint16_t y, bool alt, rtc_time rtc);

bool (check_level_passed) (LevelType_t level_type, Player_t * Player, Tank_t * tanks[MAX_TANKS], Clock_t clock, Tile_t layout[][NCOLS]);





