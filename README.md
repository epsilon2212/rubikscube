Introduction

The Rubik’s Cube is a puzzle consisting of movable cubic blocks.
The cube’s faces are divided into colored squares belonging to individual blocks. In the solved state, all squares on a given face are the same color, with each face having a different color.
Each block belongs to three perpendicular layers of the cube. Blocks can be moved by rotating the layers by multiples of a right angle.
The goal of the solver is to bring a randomly scrambled cube to a solved state.
Most commonly, cubes of size 3 × 3 × 3 are used, but larger or smaller cubes also exist.

Task

Write a program that performs a sequence of commands rotating the layers of a cube of size N × N × N, where N is a positive integer. The program starts with a solved cube. Upon request, it prints the current state of the cube.
The symbolic constant N, which defines the cube size, should be set with the directives:

#ifndef N
#define N 5
#endif

This indicates 5 as the default value. A different value can be chosen at compilation with the -DN=value option.

Input format

The program input consists of a sequence of layer rotation and print commands, ending with a period.
The program ignores any input after the terminating period.
The print command is indicated by a newline.
A layer rotation command consists of three parts specifying:
The number of layers,
The cube face,
The rotation angle.
If the number of layers part is empty, it defaults to 1. Otherwise, it is a positive decimal number not exceeding N, the cube size constant.
The face is specified by a letter:
u – up
l – left
f – front
r – right
b – back
d – down
The angle part may be empty, an apostrophe, or a quotation mark:
empty → 90° clockwise
apostrophe (') → -90° (counterclockwise)
quotation mark (") → 180°
Executing a rotation command with W layers, face S, and angle K rotates W layers from the perspective of face S by angle K, clockwise.
Data grammar
Data → CommandSequence Period
CommandSequence → ε | Command CommandSequence
Command → Rotation | Print
Rotation → LayerCount Face Angle
Print → Newline
Face → Letter
LayerCount → One | Many
One → ε
Many → Number
Number → Positive | Number Digit
Angle → Right | MinusRight | Half
Right → ε
MinusRight → Apostrophe
Half → Quotation
Letter → 'u' | 'l' | 'f' | 'r' | 'b' | 'd'
Positive → '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9'
Digit → '0' | Positive
Apostrophe → '\''
Quotation → '"'
Newline → '\n'
Period → '.'
Terminal symbols 'u', 'l', 'f', 'r', 'b', 'd', '0', …, '9', '.', '"' represent characters; '\'' is an apostrophe, and '\n' is a newline.

Output format

The program output reflects the effect of print commands.
The cube’s state is printed with digits 0–5 representing square colors, along with spaces and vertical bars |.
For a solved cube:
Up face → 0
Left face → 1
Front face → 2
Right face → 3
Back face → 4
Down face → 5
The printed layout:
  u
l|f|r|b
  d
Where u, l, f, r, b, d are the up, left, front, right, back, and down faces.
For cube size N, the printed state has 3 * N + 1 lines:
First line is empty
Next N lines → up face
Next N lines → left, front, right, back faces
Last N lines → down face
Up and down face lines: start with N+1 spaces followed by N digits.
Left, front, right, back face lines: N digits per face separated by |.
Digit positions for each face occupy an N × N area.
For left, front, right, back faces, squares are printed in the order seen after rotating the cube around the vertical axis (up–down).
For up and down faces, squares are printed in the order seen after rotating around the horizontal axis (left–right).
