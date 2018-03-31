# BMP Manipulation

BMP Manipulation takes a bitmap file and creatively modifies it based on
specifications given by the user.

## Option flags
Select any number of options to manipulate the image with.

`--color or -c` Will invert the color of the image.
`--contrast or -t` Will return a low contrast version of the image.
`--flip or -f` Will rotate the image upside down.
`--mirror or -m` Will reflect the image vertically.
`--scale or -s` Will separate the image into quadrants based on the RGB values.

### Compile and run
Follow this procedure to run the program.
```
gcc -std=gnu90 -o bitmap bit-manip.c

```
*Note: you can have any number of options.*
```
./bitmap <option> <imagename.bmp>

```
