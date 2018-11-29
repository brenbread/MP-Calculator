#include "keypad.h"
#include "utilities.h"
#include "gpio.hpp"
#include "io.hpp"
#include "lcd_display.h"

//command reg
char CLEAR_DATA = 0x00;
char CMD_SRAM_READ = 0x09; //READ OP 00001001
char CMD_SRAM_WRITE = 0x11; //WRITE OP 00010001
char CMD_KEYPAD = 0x04; //KEYPAD OP 0000 0100

//PORT A
//addresses A0-A7
GPIO A0(P1_29);
GPIO A1(P1_28);
GPIO A2(P1_23);
GPIO A3(P1_22);
GPIO A4(P1_20);
GPIO A5(P1_19);
GPIO A6(P0_30);
GPIO A7(P0_29);

//PORT B
//controls, postfix "L" indicates active low signal
GPIO dir_w(P0_0); //bus direction
GPIO bus_eL(P0_1); //bus enable low
GPIO addr_w(P2_1); //address write enable
GPIO dataOut_w(P2_2); //dataOut write enable
GPIO dataIn_eL(P2_3); //dataIn enable
GPIO cmd_w(P2_4); //command register write enable
GPIO clk(P2_6);

//Initialize the system
void startup() {
  set_cmd(CLEAR_DATA); //clear command register
  portA_in(); //Set address pins to input
  portB_out(); //Set control pins to output
  dir_w.setLow(); //bus direction set B->A
  bus_eL.setHigh(); //disables bus
  addr_w.setLow(); //disable address write
  dataOut_w.setLow(); //disable dataOut write
  dataIn_eL.setHigh(); //disable dataIn enable
  cmd_w.setLow(); //disable command write
  clk.setLow(); //start clock low
}

//address pins set to output
void portA_out(){
  A0.setAsOutput();
  A1.setAsOutput();
  A2.setAsOutput();
  A3.setAsOutput();
  A4.setAsOutput();
  A5.setAsOutput();
  A6.setAsOutput();
  A7.setAsOutput();
}

//address pins set to input
void portA_in(){
  A0.setAsInput();
  A1.setAsInput();
  A2.setAsInput();
  A3.setAsInput();
  A4.setAsInput();
  A5.setAsInput();
  A6.setAsInput();
  A7.setAsInput();
}

//control pins set to output; they never input in this case
void portB_out(){
  dir_w.setAsOutput();
  bus_eL.setAsOutput();
  addr_w.setAsOutput();
  dataOut_w.setAsOutput();
  dataIn_eL.setAsOutput();
  cmd_w.setAsOutput();
  clk.setAsOutput();
}


//keypad function
char kp_check(){
  char buffer[4];
  char output;

  for (int i = 0; i < 4; i++){
    set_cmd(CLEAR_DATA);
    set_dataOut(1 << (3 - i));
    set_cmd(CMD_KEYPAD);
      //printf("%d\n", 1 << (i));
    clk_gen(8);
    //delay_ms(1);
    buffer[i] = get_dataIn();
    set_cmd(CLEAR_DATA);
  }

    //keypad functions edited to support ASCII and LCD
    //row 0
    if (buffer[0]& 1<<0) {
      //Asterisk *
      output = 0x2A;
      return output;
    }
    else if (buffer[0]& 1<<1) {
      //0
      output = 0x30;
      return output;
    }
    else if (buffer[0]& 1<<2) {
      //pound #
      output = 0x23;
      return output;
    }
    else if (buffer[0]& 1<<3) {
      //D
      //output = 0x44; make D = divide or divide symbol
      output = 0xFD;
      return output;
    }

    //row 1
    if (buffer[1]& 1<<0) {
      //7
      output = 0x37;
      return output;
    }
    else if (buffer[1]& 1<<1) {
      //8
      output = 0x38;
      return output;
    }
    else if (buffer[1]& 1<<2) {
      //9
      output = 0x39;
      return output;
    }
    else if (buffer[1]& 1<<3) {
      //C
      output = 0x43;
      return output;
    }

    //row 2
    if (buffer[2]& 1<<0) {
      //4
      output = 0x34;
      return output;
    }
    else if (buffer[2]& 1<<1) {
      //5
      output = 0x35;
      return output;
    }
    else if (buffer[2]& 1<<2) {
      //6
      output = 0x36;
      return output;
    }
    else if (buffer[2]& 1<<3) {
      //B
      //output = 0x42; B as subtract -
      output = get_lcd_char('-');
      return output;
    }

    //row 3
    if (buffer[3]& 1<<0) {
      //1
      output = 0x31;
      return output;
    }
    else if (buffer[3]& 1<<1) {
      //2
      output = 0x32;
      return output;
    }
    else if (buffer[3]& 1<<2) {
      //3
      output = 0x33;
      return output;
    }
    else if (buffer[3]& 1<<3) {
      //A
      //output = 0x41; for Calculator make A = add operator +
      output = get_lcd_char('+');
      return output;
    }

    else {
      //printf("No key pressed\n");
      return 0;
    }
}

//writes byte to SRAM
void write(char byte) {
  bus_eL.setHigh(); //disable bus
  dir_w.setHigh(); //direction A->B

  /* GPIO Output */
  portA_out();
  if (byte & (1 << 7)) A7.setHigh();
  else A7.setLow();

  if (byte & (1 << 6)) A6.setHigh();
  else A6.setLow();

  if (byte & (1 << 5)) A5.setHigh();
  else A5.setLow();

  if (byte & (1 << 4)) A4.setHigh();
  else A4.setLow();

  if (byte & (1 << 3)) A3.setHigh();
  else A3.setLow();

  if (byte & (1 << 2)) A2.setHigh();
  else A2.setLow();

  if (byte & (1 << 1)) A1.setHigh();
  else A1.setLow();

  if (byte & (1 << 0)) A0.setHigh();
  else A0.setLow();

  bus_eL.setLow(); //enable bus, then write

}

//read byte function
char read() {
  char byte;
  bus_eL.setHigh(); //disable bus
  dir_w.setLow(); //direction B->A
  portA_in();

  bus_eL.setLow(); //enable bus, then read

  byte = A0.read();
  byte = byte | (A1.read() << 1);
  byte = byte | (A2.read() << 2);
  byte = byte | (A3.read() << 3);
  byte = byte | (A4.read() << 4);
  byte = byte | (A5.read() << 5);
  byte = byte | (A6.read() << 6);
  byte = byte | (A7.read() << 7);

  //read_pin(); //outputs binary value being read

  return byte;
}


//sets address
void set_addr(char byte) {
  bus_eL.setHigh(); //disable bus
  dataIn_eL.setHigh(); //disable dataIn register output
  cmd_w.setLow(); //disable command register write
  addr_w.setLow(); //disable address register write
  dataOut_w.setLow(); //disable dataOut register write
  write(byte); //writes byte inputted
  addr_w.setHigh(); //address register write enabled
  delay_ms(1);
  addr_w.setLow(); //latch address value
  bus_eL.setHigh(); //bus disabled
}

//sets data to be in the dataOut register
void set_dataOut(char byte) {
  bus_eL.setHigh(); //bus disabled
  dataIn_eL.setHigh(); //dataIn register disabled
  cmd_w.setLow(); //command register disabled
  addr_w.setLow(); //address register write disabled
  dataOut_w.setLow(); //dataOut write disabled
  write(byte); //address byte set
  dataOut_w.setHigh(); //dataOut register write enabled
  delay_ms(1);
  dataOut_w.setLow(); //latch dataOut value
  bus_eL.setHigh(); //bus disabled
}

//gets data from dataIn register to be read
char get_dataIn() {
  char byte; //byte output
  bus_eL.setHigh(); //bus disabled
  addr_w.setLow(); //address register write disabled
  cmd_w.setLow(); //command register write disabled
  dataOut_w.setLow(); //dataOut register write disabled
  dataIn_eL.setLow(); //bus enabled
  delay_ms(1);
  byte = read(); //read byte
  dataIn_eL.setHigh(); //dataIn register enabled
  bus_eL.setHigh(); //bus disabled
  return byte; //output
}

//OP code bytes written
void set_cmd(char byte){
  bus_eL.setHigh(); //bus disabled
  dataIn_eL.setHigh(); //dataIn register disabled
  cmd_w.setLow(); //command register write disabled
  addr_w.setLow(); //address register write disabled
  dataOut_w.setLow(); //dataOut regiser write disabled
  write(byte); //command byte written
  cmd_w.setHigh(); //command register write enabled
  delay_ms(1);
  cmd_w.setLow(); //latch command register op code
  bus_eL.setHigh(); //bus disabled
}

//main write function
void main_write(char addr, char data){
  set_cmd(CLEAR_DATA); //clears command register
  delay_ms(1);
  set_addr(addr); //sets address
  set_dataOut(data); //sets data being written
  set_cmd(CMD_SRAM_WRITE); //write op-code
  clk_gen(6); //generates clock for state machine
}

//main read function
char main_read(char addr){
  set_cmd(CLEAR_DATA); //clears command register
  delay_ms(1);
  set_cmd(CMD_SRAM_READ); //read op-code
  set_addr(addr); //sets address
  clk_gen(6); //generates clock for state machine
  char byte = get_dataIn(); //get data from dataIn register and return read data
  return byte;
}

//reads and prints binary data from address pins
void read_pin(){

  printf("%d", A7.read());
  printf("%d", A6.read());
  printf("%d", A5.read());
  printf("%d", A4.read());
  printf("%d", A3.read());
  printf("%d", A2.read());
  printf("%d", A1.read());
  printf("%d\n", A0.read());
}


//generates clock for n ticks
void clk_gen(int n) {
  for (int i = 0; i < n; i++) {
    clk.setHigh();
    delay_ms(1);
    clk.setLow();
    delay_ms(1);
  }
}
