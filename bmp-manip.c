// Lindsee Purnell
//Linux Mint 18.3 32-bit

#include <stdio.h>
#include <string.h>
#include <math.h>

#define HEADER_SIZE 54
#define PI 3.14159265

void colorinversion(FILE *copy1ptr, int width, int height, unsigned char pixels_array[height][width], int pixels_size, char *header_array, int header_size);
void contrast(FILE *copy2ptr, int width, int height, unsigned char pixels_array[height][width], int pixels_size, char *header_array, int header_size);
unsigned char bgrvalue(unsigned char current);
void mirror(FILE *copy3ptr, int width, int height, unsigned char pixels_array[height][width], int pixels_size, char *header_array, int header_size);
void flip(FILE *copy3ptr, int width, int height, unsigned char pixels_array[height][width], int pixels_size, char *header_array, int header_size);
void scaledown(FILE *copy4ptr, int width, int height, unsigned char pixels_array[height][width], int pixels_size, char *header_array, int header_size);
void rotate(FILE *copy5ptr, int width, int height, unsigned char pixels_array[height][width], int pixels_size, char *header_array, int header_size);

int main() {

  char filename[20];
  char extension[5] = ".bmp";
  int height = 240;
  int width = 320;


  // printf("Enter the filename: ");
  // scanf("%19s", filename);
  // printf("\nEnter the height and width (in pixels): ");
  // scanf("%d %d", &height, &width);
  // strcat(filename, extension);


  FILE *infile = fopen("tiger.bmp", "rb");
  // FILE *copy1 = fopen("copy1.bmp", "wb");
  // FILE *copy2 = fopen("copy2.bmp", "wb");
  // FILE *copy3 = fopen("copy3.bmp", "wb");
  // FILE *copy4 = fopen("copy4.bmp", "wb");
  FILE *copy5 = fopen("copy5.bmp", "wb");

  char header[HEADER_SIZE];
  unsigned char pixels[height][width * 3];
  int pixels_size = sizeof(pixels);

  fread( header, 1 , HEADER_SIZE , infile);
  fread( pixels, 1 , height * width * 3 , infile);

  // colorinversion(copy1, width * 3, height, pixels, pixels_size, &header[0], HEADER_SIZE);
  // contrast(copy2, width * 3, height, pixels, pixels_size, &header[0], HEADER_SIZE);
  // mirror(copy3, width * 3, height, pixels, pixels_size, &header[0], HEADER_SIZE);
  // scaledown(copy4, width * 3, height, pixels, pixels_size, &header[0], HEADER_SIZE);
  rotate(copy5, width * 3, height, pixels, pixels_size, &header[0], HEADER_SIZE);

  fclose(infile);

  return 0;

}
//Color inversion method
void colorinversion(FILE *copy1ptr, int width, int height, unsigned char pixels_array[height][width], int pixels_size, char *header_array, int header_size) {

  int h, w;
  unsigned char color_array[height][width];
  memcpy(color_array, pixels_array, pixels_size);

  for(h = 0; h < height; h++) {
    for(w = 0; w < width; w++) {
      unsigned char temp = color_array[h][w];

      temp = ~temp;
      color_array[h][w] = temp;
    }
  }
  fwrite(header_array, sizeof(char), header_size, copy1ptr);
  fwrite(color_array, sizeof(char), height * width, copy1ptr);

  fclose(copy1ptr);
}

//Contrast method
void contrast(FILE *copy2ptr, int width, int height, unsigned char pixels_array[height][width], int pixels_size, char *header_array, int header_size) {
  int col_index, row_index;
  unsigned char blue, green, red;
  char temp_array[HEADER_SIZE];

  memcpy(temp_array, header_array, HEADER_SIZE);
  unsigned char contrast_array[height][width];
  memcpy(contrast_array, pixels_array, pixels_size);


  for(row_index = 0; row_index < height; row_index++) {
    for(col_index = 0; col_index< width; col_index+=3) {
        blue = contrast_array[row_index][col_index];
        contrast_array[row_index][col_index] = bgrvalue(blue);
        green = contrast_array[row_index][col_index+ 1];
        contrast_array[row_index][col_index+ 1] = bgrvalue(green);
        red =  contrast_array[row_index][col_index+ 2];
        contrast_array[row_index][col_index+ 2] = bgrvalue(red);
    }
  }

  fwrite(temp_array, sizeof(char), header_size, copy2ptr);
  fwrite(contrast_array, sizeof(char), height * width, copy2ptr);

  fclose(copy2ptr);
}

//Helper method to find contrast
unsigned char bgrvalue(unsigned char current) {
  float ratio = 2.9695;
  int bgrchar;
  bgrchar = current - 128;
  bgrchar *= ratio;
  bgrchar += 128;

  if(bgrchar > 255) {
    bgrchar = 255;
  } else if (bgrchar < 0) {
    bgrchar = 0;
  }
  return (unsigned char) bgrchar;
}


  void mirror(FILE *copy3ptr, int width, int height, unsigned char pixels_array[height][width], int pixels_size, char *header_array, int header_size) {
    // [[]] [[]] [[]] [[]] [[]] || [] [] [] [] []
    unsigned char mirror_array[height][width];
    memcpy(mirror_array, pixels_array, pixels_size);

    char temp_array[HEADER_SIZE];
    memcpy(temp_array, header_array, HEADER_SIZE);

    int start_pnt = width / 2;
    if(start_pnt % 2 == 1) {
      start_pnt += 1;
    }

    int row_index, col_index, mirror_index;
    //        mirr                                  col
    // r   g   b   r   g   b   | r   g   b   r   g   b
    // 255 0   0   120 0   30   120  0   30 255  0   0
    // row_index = 0
    for(row_index = 0; row_index < height; row_index++) {
      for(col_index = start_pnt; col_index < width; 0) {
        int current = width - col_index - 3;
        for(mirror_index = current; mirror_index < current + 3; mirror_index++) {
          mirror_array[row_index][col_index] = mirror_array[row_index][mirror_index];
          col_index++;
        }
      }
    }

     flip(copy3ptr, width, height, mirror_array, pixels_size, temp_array, header_size);

        // fwrite(temp_array, sizeof(char), header_size, copy3ptr);
        // fwrite(mirror_array, sizeof(char), height * width, copy3ptr);
        //
        // fclose(copy3ptr);

  }

//Partial flip
void flip(FILE *copy3ptr, int width, int height, unsigned char pixels_array[height][width], int pixels_size, char *header_array, int header_size) {
    int row_index, col_index;
    int opposite = 0;
    unsigned char temp_char;
    char temp_array[HEADER_SIZE];
    memcpy(temp_array, header_array, HEADER_SIZE);


    for(row_index = height - 1; row_index >= height / 2; row_index--) {
      for(col_index = 0; col_index < width; col_index++) {
        temp_char = pixels_array[opposite][col_index];
        pixels_array[opposite][col_index] = pixels_array[row_index][col_index];
        pixels_array[row_index][col_index] = temp_char;
      }
      opposite++;
    }

    fwrite(temp_array, sizeof(char), header_size, copy3ptr);
    fwrite(pixels_array, sizeof(char), height * width, copy3ptr);

    fclose(copy3ptr);

}

void scaledown(FILE *copy4ptr, int width, int height, unsigned char pixels_array[height][width], int pixels_size, char *header_array, int header_size) {
  unsigned char scaledown_array[height][width];
  unsigned char finished_array[height][width];
  memcpy(scaledown_array, pixels_array,pixels_size);

  int row_index, col_index;
  int height_half = height / 2;
  int width_half = width / 2;
  int pixel_width = width / 3;
  int pixel_count = 0;
  int fwidth_index = 0;
  int fheight_index = 0;

// sd_arr                               finished_arr
// [r0 g0 b0]  [r1 g1 b1] [r2 g2 b2]     [r1 g1 b1] [r3 g3 b3]
// [r3 g3 b3]  [r4 g4 b4] [r5 g5 b5]
  for(row_index = 0; row_index < height; row_index++) {
    for(col_index = 0; col_index < width; col_index+=3) {
      if(row_index % 2 == 1 && col_index % 2 == 1) {
        finished_array[fheight_index][fwidth_index + width_half] = scaledown_array[row_index][col_index];
        finished_array[fheight_index][fwidth_index + width_half + 1] = scaledown_array[row_index][col_index + 1];
        finished_array[fheight_index][fwidth_index + width_half + 2] = scaledown_array[row_index][col_index + 2];

        finished_array[fheight_index][fwidth_index] = scaledown_array[row_index][col_index];
        finished_array[fheight_index][fwidth_index + 1] = 0;
        finished_array[fheight_index][fwidth_index + 2] = 0;

        finished_array[fheight_index + height_half][fwidth_index] = 0;
        finished_array[fheight_index + height_half][fwidth_index + 1] = 0;
        finished_array[fheight_index + height_half][fwidth_index + 2] = scaledown_array[row_index][col_index + 2];

        finished_array[fheight_index + height_half][fwidth_index + width_half] = 0;
        finished_array[fheight_index + height_half][fwidth_index + width_half + 1] = scaledown_array[row_index][col_index + 1];
        finished_array[fheight_index + height_half][fwidth_index + width_half + 2] = 0;

        pixel_count++;
        fheight_index = (pixel_count * 2) / pixel_width;
        // printf("width: %d\n", fwidth_index);
        fwidth_index = pixel_count * 3 % width_half;
        // printf("height: %d\n", fheight_index);
        // elements total = 8
        // width = 3
        // [] [] []
        // [] [] []
        // [] []
      }
    }
  }

  fwrite(header_array, sizeof(char), header_size, copy4ptr);
  fwrite(finished_array, sizeof(char), height * width, copy4ptr);

  fclose(copy4ptr);

}

void rotate(FILE *copy5ptr, int width, int height, unsigned char pixels_array[height][width], int pixels_size, char *header_array, int header_size) {
  unsigned char rotate_array[height][width];
  unsigned char finished_array[width][height];
  memcpy(rotate_array, pixels_array, pixels_size);

  char header_copy[HEADER_SIZE];
  memcpy(header_copy, header_array, HEADER_SIZE);

  int i;
  for(i = 0; i < 4; i++) {
    header_copy[18 + i] = header_array[22 + i];
    header_copy[22 + i] = header_array[18 + i];
  }


  // Original picture 2 x 3 pixels
  //                  2 x 9 rgb values
  // (a) (b) (c)
  // (d) (e) (f)
  // Rotated picture 3 x 2 pixels
  //                 3 x 6 rgb values
  // [1] [2] [3] [1] [2] [3]
  // [1] [2] [3] [1] [2] [3]
  // [1] [2] [3] [1] [2] [3]

  // [1] [1] [1]
  // [2] [2] [2]
  // [3] [3] [3]
  // [1] [1] [1]
  // [2] [2] [2]
  // [3] [3] [3]

  double radians = (90 * PI) / 180;
  int finished_sin = sin(radians);
  int finished_cos = cos(radians);

  double point_x = 0.5 * (width - 1);
  double point_y = 0.5 * (height - 1);

  int row_index, col_index, fcol_index;
  for(row_index = 0; row_index < width; row_index++) {

    for(col_index = 0; col_index < height; col_index++) {
      long double curr_row = row_index - point_x;
      long double curr_col = col_index - point_y;
      int rotate_row = (int) (+curr_row * finished_cos - curr_col * finished_sin + point_x);
      int rotate_col = (int) (+curr_row * finished_sin + curr_col * finished_cos + point_y);

      if(rotate_row >=0 && rotate_row < width && rotate_col >=0 && rotate_col < height) {
        finished_array[row_index][(col_index * height + row_index) * 3] = rotate_array[row_index][(rotate_col * height + rotate_row) * 3];
        // finished_array[row_index][(col_index * height + row_index) * 3 + 1] = rotate_array[row_index][(rotate_col * height + rotate_row) * 3 + 1];
        // finished_array[row_index][(col_index * height + row_index) * 3 + 2] = rotate_array[row_index][(rotate_col * height + rotate_row) * 3 + 2];
      }
        printf("%d, %d\n", row_index, col_index);
    }

  }


  fwrite(header_copy, sizeof(char), header_size, copy5ptr);
  fwrite(finished_array, sizeof(char), height * width, copy5ptr);

  fclose(copy5ptr);


}
