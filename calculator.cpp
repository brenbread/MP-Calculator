#include <vector>
#include <string>
#include "lcd_display.h"
#include "keypad.h"
#include "utilities.h"
#include "gpio.hpp"
#include "io.hpp"

using namespace std;

char ADD = 0x2B; // +
char SUB = 0xB0; // -, originally 2D
char MULT = 0x2A; // mult
char DIV = 0xFD; // divide
char NEG = 0x2D; // negative char


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
  if (input == get_lcd_char('+') || input == SUB || input == 0xFD || input == get_lcd_char('*')) {
    return true;
  }
  else {
    return false;
  }
}

//-- for each char in the string, write it to the lcd --//
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

//sets memory 0 to 3 locations to 0
void clear_memory() {
  for (int i = 0; i < 4; i++) {
    main_write(i, 0x00);
  }
}

//Reserve 3 locations in SRAM to write output data to
// 0x00 = lower 8-bits
// 0x01 = upper 8-bits (for 16-bit)
// 0x02 + 0x03 = upper 16 bits (for 32-bit number)
void write_to_sram(int num) {
  clear_memory();

  int bytes[4];
  int shifter = 24;

  //split output into 4 8-bit parts, then write it to respective SRAM location
  for (int i = 0; i < 4; i++) {
     bytes[i] = num >> shifter;
     main_write(i, bytes[i]);
     shifter -= 8;
  }
  printf("\n");
}

int read_from_sram() {
  /*
    array locations:
      0 = lower 8-bits
      1 = upper 8-bits for 16-bit, 1 << 8
      2 + 3 = upper 16 bits for 32-bit number 2 << 16, 3 << 24
      use bit shifting and add these together, and convert it to an int for your output
  */

  //read sram vals and put them into mem_0 array
  int mem_0[4] = {};
  int shifter = 0;

  //pop the stack
  for (int i = 3; i >= 0; --i) {
    mem_0[i] = main_read(i); //read SRAM locations
    mem_0[i] = mem_0[i] << shifter; //shift bits accordingly
    shifter += 8;
  }

  //add bits together
  int output = 0;
  for (int i = 3; i >= 0; --i) {
    output += mem_0[i];
  }

  printf("MEM0: %i\n", output);
  return output;

}
