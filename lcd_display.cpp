#include "lcd_display.h"
#include "keypad.h"
#include "utilities.h"
#include "gpio.hpp"
#include "io.hpp"

//-- command reg --//
char CMD_LCD_READ = 0x2A; // READ OP 0010 1010
char CMD_LCD_WRITE = 0x02; //WRITE OP 0000 0010
char CMD_LCD_DATA = 0x22; //RS OP 0010 0010. Selects Data Reg

//-- lcd functions, lcd rs = 0 --//
char LCD_FUNC = 0x38;
char LCD_INPUT = 0x06;
char LCD_CLEAR = 0x01;
char LCD_DISP = 0x0F;
char LCD_HOME = 0x02;
char LCD_CURSOR_INC = 0x07;
char LCD_CURSOR_SHIFT_R = 0x14;
char LCD_CURSOR_SHIFT_L = 0x10;

//-- initialize the lcd display --//
void init_lcd() {
//function, input, screen clear, display switch before data
  //init display
  delay_ms(100);
  write_lcd(LCD_FUNC);
  delay_ms(2);

  //input set
  write_lcd(LCD_INPUT);
  delay_ms(2);

  //clear LCD
  write_lcd(LCD_CLEAR);
  delay_ms(2);

  //display init
  write_lcd(LCD_DISP);
  delay_ms(2);

  //printf("LCD display initialized!\n");
}

//write lcd instr
void write_lcd(char byte) {
  //printf("Command: %x\n", byte);
  set_cmd(CMD_LCD_WRITE); //write command
  delay_ms(1);
  set_dataOut(byte);
  clk_gen(5); //generate clock for 5 cycles
  delay_ms(1);
  set_cmd(CLEAR_DATA);
}

//data write to lcd
void write_data_lcd(char byte) {
  //printf("Data: %x\n", byte);
  set_cmd(CMD_LCD_DATA);
  delay_ms(1);
  set_dataOut(byte);
  delay_ms(1);
  clk_gen(5);
  delay_ms(1);
  set_cmd(CLEAR_DATA);
}

//read the lcd and increment the cursor
char read_lcd() {
  char output = 0;
  delay_ms(1);
  set_cmd(CMD_LCD_READ); //read command
  clk_gen(5); //generate clock for 5 cycles
  delay_ms(1);
  output = get_dataIn();
  write_lcd(LCD_CURSOR_INC); //increment cursor after reading current character
  set_cmd(CLEAR_DATA); //clear command register
  return output;
  //printf("LCD Read: 0x%x\n", output);
}

//text table for lcd, checks ASCII character read from LCD
//HEX ASCII -> literal char
char char_lcd_table(char input) {
  char output = 0;

  switch (input) {
    //-- SPECIAL CHARACTERS --//
    //space
    case 0x20:
      output = ' ';
      break;
    case 0x21:
      output = '!';
      break;
    case 0x22:
      output = '\"';
      break;
    case 0x23:
      output = '#';
      break;
    case 0x24:
      output = '$';
      break;
      //%
    case 0x25:
      output = '%';
      break;
      //&
    case 0x26:
      output = '&';
      break;
    case 0x27:
      output = '\'';
      break;
    case 0x28:
      output = '(';
      break;
    case 0x29:
      output = ')';
      break;
    case 0x2A:
      output = '*';
      break;
    case 0x2B:
      output = '+';
      break;
    case 0x2C:
      output = ',';
      break;
    case 0x2D:
      output = '-';
      break;
    case 0x2E:
      output = '.';
      break;
    case 0x2F:
      output = '/';

    //-- NUMBERS --//
    case 0x30:
      output = '0';
      break;
    case 0x31:
      output = '1';
      break;
    case 0x32:
      output = '2';
      break;
    case 0x33:
      output = '3';
      break;
    case 0x34:
      output = '4';
      break;
    case 0x35:
      output = '5';
      break;
    case 0x36:
      output = '6';
      break;
    case 0x37:
      output = '7';
      break;
    case 0x38:
      output = '8';
      break;
    case 0x39:
      output = '9';
      break;
    case 0x3A:
      output = ':';
      break;
    case 0x3B:
      output = ';';
      break;
    case 0x3C:
      output = '<';
      break;
    case 0x3D:
      output = '=';
      break;
    case 0x3E:
      output = '>';
      break;
    case 0x3F:
      output = '\?';
      break;
    case 0x40:
      output = '@';
      break;
    case 0x41:
      output = 'A';
      break;
    case 0x42:
      output = 'B';
      break;
    case 0x43:
      output = 'C';
      break;
    case 0x44:
      output = 'D';
      break;
    case 0x45:
      output = 'E';
      break;
    case 0x46:
      output = 'F';
      break;
    case 0x47:
      output = 'G';
      break;
    case 0x48:
      output = 'H';
      break;
    case 0x49:
      output = 'I';
      break;
    case 0x4A:
      output = 'J';
      break;
    case 0x4B:
      output = 'K';
      break;
    case 0x4C:
      output = 'L';
      break;
    case 0x4D:
      output = 'M';
      break;
    case 0x4E:
      output = 'N';
      break;
    case 0x4F:
      output = 'O';
      break;
    case 0x50:
      output = 'P';
      break;
    case 0x51:
      output = 'Q';
      break;
    case 0x52:
      output = 'R';
      break;
    case 0x53:
      output = 'S';
      break;
    case 0x54:
      output = 'T';
      break;
    case 0x55:
      output = 'U';
      break;
    case 0x56:
      output = 'V';
      break;
    case 0x57:
      output = 'W';
      break;
    case 0x58:
      output = 'X';
      break;
    case 0x59:
      output = 'Y';
      break;
    case 0x5A:
      output = 'Z';
      break;
    case 0x5B:
      output = '[';
      break;
    case 0x5D:
      output = ']';
      break;
    case 0x5E:
      output = '^';
      break;
    case 0x5F:
      output = '_';
      break;
    case 0x60:
      output = '`';
    case 0x61:
      output = 'a';
      break;
    case 0x62:
      output = 'b';
      break;
    case 0x63:
      output = 'c';
      break;
    case 0x64:
      output = 'd';
      break;
    case 0x65:
      output = 'e';
      break;
    case 0x66:
      output = 'f';
      break;
    case 0x67:
      output = 'g';
      break;
    case 0x68:
      output = 'h';
      break;
    case 0x69:
      output = 'i';
      break;
    case 0x6A:
      output = 'j';
      break;
    case 0x6B:
      output = 'k';
      break;
    case 0x6C:
      output = 'l';
      break;
    case 0x6D:
      output = 'm';
      break;
    case 0x6E:
      output = 'n';
      break;
    case 0x6F:
      output = 'o';
      break;
    case 0x70:
      output = 'p';
      break;
    case 0x71:
      output = 'q';
      break;
    case 0x72:
      output = 'r';
      break;
    case 0x73:
      output = 's';
      break;
    case 0x74:
      output = 't';
      break;
    case 0x75:
      output = 'u';
      break;
    case 0x76:
      output = 'v';
      break;
    case 0x77:
      output = 'w';
      break;
    case 0x78:
      output = 'x';
      break;
    case 0x79:
      output = 'y';
      break;
    case 0x7A:
      output = 'z';
      break;
    case 0x7B:
      output = '{';
      break;
    case 0x7C:
      output = '|';
      break;
    case 0x7D:
      output = '}';
      break;
    default:
      output = 0x20;
      break;
  }

  return output;
}

//get lcd ASCII value
// literal char -> HEX
char get_lcd_char(char input) {
  char output = 0;

  switch (input) {
    //-- SPECIAL CHARACTERS --//
    //space
    case ' ':
      output = 0x20;
      break;
    case '!':
      output = 0x21;
      break;
    case '\"':
      output = 0x22;
      break;
    case '#':
      output = 0x23;
      break;
    case '$':
      output = 0x24;
      break;
      //%
    case '%':
      output = 0x25;
      break;
      //&
    case '&':
      output = 0x26;
      break;
    case '\'':
      output = 0x27;
      break;
    case '(':
      output = 0x28;
      break;
    case ')':
      output = 0x29;
      break;
    case '*':
      output = 0x2A;
      break;
    case '+':
      output = 0x2B;
      break;
    case ',':
      output = 0x2C;
      break;
    case '-':
      output = 0x2D;
      break;
    case '.':
      output = 0x2E;
      break;
    case '/':
      output = 0x2F;

    //-- NUMBERS --//
    case '0':
      output = 0x30;
      break;
    case '1':
      output = 0x31;
      break;
    case '2':
      output = 0x32;
      break;
    case '3':
      output = 0x33;
      break;
    case '4':
      output = 0x34;
      break;
    case '5':
      output = 0x35;
      break;
    case '6':
      output = 0x36;
      break;
    case '7':
      output = 0x37;
      break;
    case '8':
      output = 0x38;
      break;
    case '9':
      output = 0x39;
      break;
    case ':':
      output = 0x3A;
      break;
    case ';':
      output = 0x3B;
      break;
    case '<':
      output = 0x3C;
      break;
    case '=':
      output = 0x3D;
      break;
    case '>':
      output = 0x3E;
      break;
    case '\?':
      output = 0x3F;
      break;
    case '@':
      output = 0x40;
      break;
    case 'A':
      output = 0x41;
      break;
    case 'B':
      output = 0x42;
      break;
    case 'C':
      output = 0x43;
      break;
    case 'D':
      output = 0x44;
      break;
    case 'E':
      output = 0x45;
      break;
    case 'F':
      output = 0x46;
      break;
    case 'G':
      output = 0x47;
      break;
    case 'H':
      output = 0x48;
      break;
    case 'I':
      output = 0x49;
      break;
    case 'J':
      output = 0x4A;
      break;
    case 'K':
      output = 0x4B;
      break;
    case 'L':
      output = 0x4C;
      break;
    case 'M':
      output = 0x4D;
      break;
    case 'N':
      output = 0x4E;
      break;
    case 'O':
      output = 0x4F;
      break;
    case 'P':
      output = 0x50;
      break;
    case 'Q':
      output = 0x51;
      break;
    case 'R':
      output = 0x52;
      break;
    case 'S':
      output = 0x53;
      break;
    case 'T':
      output = 0x54;
      break;
    case 'U':
      output = 0x55;
      break;
    case 'V':
      output = 0x56;
      break;
    case 'W':
      output = 0x57;
      break;
    case 'X':
      output = 0x58;
      break;
    case 'Y':
      output = 0x59;
      break;
    case 'Z':
      output = 0x5A;
      break;
    case '[':
      output = 0x5B;
      break;
    case ']':
      output = 0x5D;
      break;
    case '^':
      output = 0x5E;
      break;
    case '_':
      output = 0x5F;
      break;
    case '`':
      output = 0x60;
    case 'a':
      output = 0x61;
      break;
    case 'b':
      output = 0x62;
      break;
    case 'c':
      output = 0x63;
      break;
    case 'd':
      output = 0x64;
      break;
    case 'e':
      output = 0x65;
      break;
    case 'f':
      output = 0x66;
      break;
    case 'g':
      output = 0x67;
      break;
    case 'h':
      output = 0x68;
      break;
    case 'i':
      output = 0x69;
      break;
    case 'j':
      output = 0x6A;
      break;
    case 'k':
      output = 0x6B;
      break;
    case 'l':
      output = 0x6C;
      break;
    case 'm':
      output = 0x6D;
      break;
    case 'n':
      output = 0x6E;
      break;
    case 'o':
      output = 0x6F;
      break;
    case 'p':
      output = 0x70;
      break;
    case 'q':
      output = 0x71;
      break;
    case 'r':
      output = 0x72;
      break;
    case 's':
      output = 0x73;
      break;
    case 't':
      output = 0x74;
      break;
    case 'u':
      output = 0x75;
      break;
    case 'v':
      output = 0x76;
      break;
    case 'w':
      output = 0x77;
      break;
    case 'x':
      output = 0x78;
      break;
    case 'y':
      output = 0x79;
      break;
    case 'z':
      output = 0x7A;
      break;
    case '{':
      output = 0x7B;
      break;
    case '|':
      output = 0x7C;
      break;
    case '}':
      output = 0x7D;
      break;
    default:
      output = 0x20;
      break;
  }

  return output;
}

//main loop for this LCD program
void lcd_data_loop() {
  /*
  //owo whats this
  write_data_lcd(0x4F); //O
  write_data_lcd(0x77); //w
  write_data_lcd(0x4F); //O

  write_data_lcd(0x20); //space

  write_data_lcd(0x77); //w
  write_data_lcd(0x68); //h
  write_data_lcd(0x61); //a
  write_data_lcd(0x74); //t
  write_data_lcd(0x27); //'
  write_data_lcd(0x73); //s

  write_data_lcd(0x20); //space

  write_data_lcd(0x74); //t
  write_data_lcd(0x68); //h
  write_data_lcd(0x69); //i
  write_data_lcd(0x73); //s
  write_data_lcd(0x3F); //?
  */
  //CMPE 127
  write_data_lcd(0x43); //C
  write_data_lcd(0x4D); //M
  write_data_lcd(0x50); //P
  write_data_lcd(0x45); //E
  write_data_lcd(0x20); //space
  write_data_lcd(0x31); //1
  write_data_lcd(0x32); //2
  write_data_lcd(0x37); //7

  //return cursor to home
  write_lcd(LCD_HOME);

  //read LCD 7 times for CMPE 127
  for (int i = 0; i < 8; i++) {
    read_lcd();
  }
}
