#include <stdio.h>
#include <vector>
#include <string>
#include <math.h>
#include "utilities.h"
#include "gpio.hpp"
#include "io.hpp"
#include "keypad.h"
#include "lcd_display.h"
#include "calculator.h"

using namespace std;

int main(void)
{
	printf("Calculator Lab\n");

	//-- startup sequence --//
	startup();
	LD.clear();
	delay_ms(100);
	init_lcd();
	printf("Waiting for input...\n");

	//-- kp input --//
	char input = 0; //from keypad
	char output = 0; //to console

	//---- Status Variables ----//
	//-- row counter for row 1, max 16 --//
  int count = 0;

	// -- Row Status --//
	//-- 0 = row 1, input row --//
	//-- 1 = row 2, output row --//
  int row_status = 0;

	//-- Check if Operator is active --//
	//-- 1 = active --//
	int op_status = 0;

	//-- Check op type --//
	//-- use ASCII to set type --//
	char op_type = ' ';

	//-- Counts current value --//
	int op_count = 0;

	//-- Computational values --//
	vector<char> op_a_in; //helper vector for operator a
	int op_a = 0;
	vector<char> op_b_in; //helper vector for operator b
	int op_b = 0;
	vector<char>to_lcd_out;
	int op_output = 0;

	//-- Polling infinitely --//
	 while(1){
		input = kp_check(); //outputs ASCII hex for input to LCD
 	  output = char_lcd_table(input); //converts ASCII hex to readable char

		//-- Clear the LCD Display --//
 	  if (input == get_lcd_char('C')) {
 	    printf("LCD Display cleared\n");
 	    init_lcd();
			count = 0;
			row_status = 0;
			op_status = 0;
			op_count = 0;
			op_a_in.clear();
			op_a = 0;
			op_b_in.clear();
			op_b = 0;
			to_lcd_out.clear();
			op_output = 0;
  	}

		//-- Polling for Input --//
 	  else if (input != 0) {

 	    /*
				Button # to "equal"
				Gets input of second op and puts it in op_b
				Then perform computation
				write to 2nd line, set a certain amount of spaces, display output
			*/

			if (input == get_lcd_char('#')) {
				//no op asserted, print value
				if (op_status == 0) {
					write_lcd(LCD_HOME); //return cursor to home
					//read the value for op_a
					for (int i = 0; i < op_count; i++) {
						op_a_in.push_back(read_lcd());
					}
					//set LCD back to input set
					write_lcd(LCD_INPUT);
					// write spaces based on current count
					write_space(count);

					//begin output to LCD
					string output(op_a_in.begin(), op_a_in.end());
					lcd_print_output(output);

					row_status = 1;
				}

				//op asserted
				else if (op_status == 1) {
					//read op_b value
					//decrement cursor to the left op_count times
					for (int i = 0; i < op_count; i++) {
						write_lcd(LCD_CURSOR_SHIFT_L);
					}
					for (int i = 0; i < op_count; i++) {
						op_b_in.push_back(read_lcd());
					}

					//op b is not empty, perform calculations
					if (!op_b_in.empty()) {
						//show value of op b in console
						int b_count = pow(10, op_b_in.size()-1);
						printf("b = ");
						for (auto i: op_b_in) {
							//put value of op_b_in into op_b
							printf("%c", i);
							op_b += ((i - '0') * (b_count));
							b_count = b_count / 10;
						}
						printf("\n");

						//print out spaces
						write_space(count);

						//convert op a and b into ints
						if (op_type == ADD) {
							op_output = op_a + op_b;
							lcd_print_output(to_string(op_output));
						}

						else if (op_type == SUB) {
							op_output = op_a - op_b;
							lcd_print_output(to_string(op_output));
						}

						else if (op_type == MULT) {
							op_output = op_a * op_b;
							if (op_output > 2147483647)
								lcd_print_output("ERR: MAX INT"); //max 32 bit error
							else
								lcd_print_output(to_string(op_output));
						}

						else if (op_type == DIV) {
							if (op_b == 0)
								lcd_print_output("ERR:#DIV/0"); //div/0 error
							else {
								op_output = op_a / op_b;
								lcd_print_output(to_string(op_output));
							}
						}

						//edge case for unknown error
						else {
							lcd_print_output("ERR:Unknown");
						}
					}

					//no op b input, print out op A
					else {
						write_space(count);
						string output(op_a_in.begin(), op_a_in.end());
						lcd_print_output(output);
					}

					//TODO: Store output value into SRAM
					printf("Output: %i\n", op_output);
					row_status = 1;
				}
			}

			//-- check if operator is inputted already  --//
			else if (op_status == 1 && check_op_input(input)) {
					printf("Can't input ops in this state\n");
				}

			//-- input operator +, -, mult, div, and set op_status to active --//
			else if (op_status == 0 && check_op_input(input)) {

					op_type = input; //sets op status
					printf("%x\n", op_status);
					write_data_lcd(input);
					//-- READ FIRST INPUT HERE AND PUT IT INTO OP_A --//
					write_lcd(LCD_HOME); //return cursor to home

					for (int i = 0; i < op_count; i++) {
						op_a_in.push_back(read_lcd());
					}

					int a_count = pow(10, op_a_in.size()-1);
					printf("a = ");
					if(!op_a_in.empty()) {
						for (auto i: op_a_in) {
							printf("%c", i);
							op_a += ((i - '0') * (a_count));
							a_count = a_count / 10;
						}
					}
					printf("\n");
					op_count = 0; //reset op_count
					op_status = 1; //set op_status to active
					printf("op_status active\n");
					write_lcd(LCD_CURSOR_SHIFT_R);
					count += 1;
				}


			//-- default write data --//
 	    else if (count <= 16 && row_status == 0) {
 	      write_data_lcd(input);
 	      count += 1;
				op_count += 1;
 	    }


			//-- Max Characters for Row 1 --//
 	    else if (count >= 16 && row_status == 0){
 	      printf("Max characters exceeded for row 1 or can't send input anymore\n");
 	    }
 	  }
		delay_ms(10);
	 }
	 return 0;
 }
