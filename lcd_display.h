#ifndef LCD_DISPLAY_H
#define LCD_DISPLAY_H

extern char LCD_HOME;
extern char LCD_CURSOR_INC;
extern char LCD_CURSOR_SHIFT_R;
extern char LCD_CURSOR_SHIFT_L;
extern char LCD_INPUT;

void init_lcd();
char read_lcd();
void write_lcd(char byte);

char char_lcd_table(char input);
char get_lcd_char(char input);
void write_data_lcd(char byte);
void lcd_data_loop();



#endif
