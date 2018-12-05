[Full Board](images/full-board.jpg)
# 32-bit Integer µP Calculator
A calculator that supports positive and negative 32-bit integer addition, subtraction, multiplication, and division.
This project was designed, and built for a microprocessors class using the [SJOne Board](http://socialledge.com/sjsu/index.php/SJ_One_Board) and [SJSU-Dev](https://github.com/kammce/SJSU-Dev) packages.
State machines were designed and built for each component using multiple logic gates.

## Description of Functionality
`INSERT VIDEO DEMO HERE`
On startup, the board initializes the GPIO pins on the SJOne board, clears its 7-segment display, initializes the LCD display, and begins to poll for input.
|Button (Keypad)|Function|Button (SJOne)|Function|
|:-:|:-:|:-:|:-:|
|A|Add, '+'| SW1 | Retrieve latest computation from SRAM and write to LCD |
|B|Subtraction,'-'|	SW2 | Clear LCD Display and SRAM memory |
|*|Multiplication, 'x'|	SW3 | Negative operator
|D|Divide, '/'|
|C|Clear LCD display|
|#|Compute, '='|
|0-9|Number input|

### Computation
#### Addition
![Addition](/images/add.jpg)
#### Subtraction
![Subtraction](/images/sub.jpg)
#### Multiplication
![Multiplication](/images/mult.jpg)
#### Division
![Division](/images/div.jpg)

### Storing/Retrieving Output to/from Memory
Once a computation is successfully completed, the output number is split up to 4 8-bit parts and stores each part from memory locations `0x00` to `0x03` like a stack.
For example, for the value `2147483647`, or in hexadecimal `0x7FFFFFFF`:
|Memory Location (Hex) |Content (Hex)|
|:-:|:-:|
|`0x00`|`0x7F`|
|`0x01`|`0xFF`|
|`0x02`|`0xFF`|
|`0x03`|`0xFF`|
Retrieving this value from memory stores values from all 4 locations into an array, bit shifting them a certain amount of bits, and adding it all together for the respective value.

## File Hierarchy
Write something about the files here.

## Parts List
  - More detailed parts list and description is listed in their respective folders under `/Components`
  - Jameco 169245 4x4 16 Button Keypad
  - Cypress CY62256N 256K (32K x 8) SRAM
  - Arducam 1602 LCD Display (Using the HD44780 controler)

## Requirements
* [SJSU-Dev](https://github.com/kammce/SJSU-Dev) - Development framework for the SJOne board

### Installation
Follow the installation instructions listed in [SJSU-Dev](https://github.com/kammce/SJSU-Dev).

### TODOs
 - Support floating point input and output
 - Add remainder for division
 - Add more memory locations for SRAM
