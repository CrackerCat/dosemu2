/*
 * (C) Copyright 1992, ..., 2014 the "DOSEMU-Development-Team".
 *
 * for details see file COPYING in the DOSEMU distribution
 */

/* definitions for updating text modes */

#include "translate.h"
#include "remap.h"
#define CONFIG_SELECTION 1

/********************************************/

#define ATTR_FG(attrib) (attrib & 0x0F & vga.attr.data[0x12])
#define ATTR_BG(attrib) (attrib >> 4)

extern Boolean have_focus;
extern int use_bitmap_font;

struct text_system
{
   /* function to draw a string in text mode using attribute attr */
   void (*Draw_string)(int x, int y , unsigned char *s, int len, Bit8u attr);
   void (*Draw_line)(int x, int y , int len);
   void (*Draw_cursor)(int x, int y, Bit8u attr, int first, int last, Boolean focus);
   void (*SetPalette) (DAC_entry *color, int index);
   void (*lock)(void);
   void (*unlock)(void);
};

struct RemapObjectStruct;
struct RectArea;

int register_text_system(struct text_system *text_system);
struct RectArea draw_bitmap_cursor(int x, int y, Bit8u attr, int start,
    int end, Boolean focus, struct bitmap_desc dst_image);
struct RectArea draw_bitmap_line(int x, int y, int len,
    struct bitmap_desc dst_image);
void blink_cursor(void);
void reset_redraw_text_screen(void);
void dirty_text_screen(void);
int text_is_dirty(void);
void update_cursor(void);
void init_text_mapper(int image_mode, int features, ColorSpaceDesc *csd);
void done_text_mapper(void);
struct RectArea convert_bitmap_string(int x, int y, unsigned char *text,
      int len, Bit8u attr, struct bitmap_desc dst_image);
int update_text_screen(void);
void text_redraw_text_screen(void);
void text_gain_focus(void);
void text_lose_focus(void);
void text_blit(int x, int y, int width, int height,
    struct bitmap_desc dst_image);

#ifdef CONFIG_SELECTION
/* for selections */
t_unicode* end_selection(void);
void clear_if_in_selection(void);
void start_selection(int col, int row);
void start_extend_selection(int col, int row);
void clear_selection_data(void);
void extend_selection(int col, int row);

int x_to_col(int x, int w_x_res);
int y_to_row(int y, int w_y_res);

#endif
