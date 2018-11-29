#ifndef KEYPAD_H
#define KEYPAD_H

#include "io.hpp"
#include "gpio.hpp"
#include <stdio.h>
#include "utilities.h"

extern char CLEAR_DATA;
extern char CMD_SRAM_READ;
extern char CMD_SRAM_WRITE;
extern char CMD_KEYPAD;

//port functions
void startup();
void portA_out();
void portA_in();
void portB_out();
void clk_gen(int n);

//read + write functions
void write(char byte);
char read();
void main_write(char addr, char data);
char main_read(char addr);

void set_cmd(char byte);
void set_addr(char addr);
void set_dataOut(char data);
char get_dataIn();

//keypad functions
char kp_check();

//for debugging
void read_pin();
void debug();

#endif
