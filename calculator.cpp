#include <vector>
#include <string>
#include "lcd_display.h"
#include "keypad.h"
#include "utilities.h"
#include "gpio.hpp"
#include "io.hpp"

using namespace std;

char ADD = 0x2B; // +
char SUB = 0x2D; // -
char MULT = 0x2A; // mult
char DIV = 0xFD; // divide


//-- Program Steps --//
/*
  1. Have system POLL for keypad input. Each input will display on LCD (DO THIS FIRST)
      a. When doing calculator portion: Pressing a computatoinal button (+, -, etc),
         Store value a into addr 0x00. Pressing another operator button again will store the 2nd value into addr 0x01
         Hitting "=" or any other operator will store the value into 0x0A and 0x00 so it can do computations again
      b. SW1 on SJOne Board clears the screen
      c. SW2 on SJOne Board clears screen AND SRAM locations
  2. Do input checking make sure symbols are OK for computation
*/

//-- Helper functions --//
bool check_op_input(char input) {
  if (input == get_lcd_char('+') || input == get_lcd_char('-') || input == 0xFD || input == get_lcd_char('*')) {
    return true;
  }
  else {
    return false;
  }
}

void lcd_print_output(string output) {
  for (char& c : output) {
    write_data_lcd(c);
  }
}

//-- Writes spaces for LCD --//
void write_space(int count) {
  //if first row is filled completely, 24 spaces to next line
  if (count >= 16) {
    for (int i = 0; i < 24; i++){
      write_data_lcd(get_lcd_char(' '));
    }
  }

  // count < 16, adds more spaces to get to next line
  else {
    for (int i = 0; i < 24 + (16 - count); i++){
      write_data_lcd(get_lcd_char(' '));
    }
  }
}
