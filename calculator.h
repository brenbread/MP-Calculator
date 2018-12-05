#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <vector>
#include <string>
#include "io.hpp"
#include "gpio.hpp"
#include <stdio.h>
#include "utilities.h"

using namespace std;

extern char ADD;
extern char SUB;
extern char MULT;
extern char DIV;
extern char NEG;

bool check_op_input(char input);
void write_space(int count);
char set_op_type(char type);
void lcd_print_output(string output);

//sram helper functions
void write_to_sram(int num);
void clear_memory();
int read_from_sram();

#endif
