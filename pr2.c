// Lindsee Purnell
//Linux Mint 18.3 32-bit

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>

#define HEADER_SIZE 54

void colorinversion(FILE *copy1ptr, int width, int height, unsigned char pixels_array[height][width], int pixels_size, char *header_array, int header_size);
void contrast(FILE *copy2ptr, int width, int height, unsigned char pixels_array[height][width], int pixels_size, char *header_array, int header_size);
unsigned char bgrvalue(unsigned char current);
void mirror(FILE *copy3ptr, int width, int height, unsigned char pixels_array[height][width], int pixels_size, char *header_array, int header_size);
void flip(FILE *copy3ptr, int width, int height, unsigned char pixels_array[height][width], int pixels_size, char *header_array, int header_size);
void scaledown(FILE *copy4ptr, int width, int height, unsigned char pixels_array[height][width], int pixels_size, char *header_array, int header_size);

int main(int argc, char **argv) {

  char *filename = argv[3];
  // char filename[20] = "parrots.bmp";
  printf("%s\n", optarg);
  int height, width, triple_width, option_index;
  FILE *infile = fopen(filename, "rb");
  FILE *outfile = fopen("outfile.bmp", "wb");
  char header[HEADER_SIZE];

  fread( header, 1 , HEADER_SIZE , infile);

  width = *(int*)&header[18];
  height = *(int*)&header[22];
  triple_width = width * 3;

  printf("W: %d, H: %d\n", triple_width, height);
  unsigned char pixels[height][triple_width];

  fread( pixels, 1 , height * triple_width , infile);



  printf("W: %d, H: %d\n", width, height);

  int pixels_size = sizeof(pixels);

  struct option longopts[] = {
    {"color", optional_argument, NULL, 'c'},
    {"contrast", optional_argument, NULL, 't'},
    {"mirror", optional_argument, NULL, 'm'},
    {"scale", optional_argument, NULL, 's'},
    {0, 0, 0, 0}
  };

  while ((option_index = getopt_long(argc, argv, "c:t:m:s:", longopts, NULL)) != -1) {
    switch(option_index) {
      case 'c':
        printf("Color inversion selected.\n");
        colorinversion(outfile, triple_width, height, pixels, pixels_size, &header[0], HEADER_SIZE);
        printf("Went to color inversion\n");
        break;
      case 't':
        printf("Contrast selected.\n");
        contrast(outfile, triple_width, height, pixels, pixels_size, &header[0], HEADER_SIZE);
        break;
      case 'm':
        printf("Mirroring selected.\n");
        mirror(outfile, triple_width, height, pixels, pixels_size, &header[0], HEADER_SIZE);
        printf("Went to mirroring\n");
        break;
      case 's':
        printf("Scale down selected.\n");
        scaledown(outfile, triple_width, height, pixels, pixels_size, &header[0], HEADER_SIZE);
        printf("Scaling done\n");
        break;
      default:
        printf("Not an option\n");
        return 1;
    }
  }

  // colorinversion(outfile, triple_width, height, pixels, pixels_size, &header[0], HEADER_SIZE);
  // printf("Test\n");
  // mirror(outfile, triple_width, height, pixels, pixels_size, &header[0], HEADER_SIZE);
  // scaledown(outfile, triple_width, height, pixels, pixels_size, &header[0], HEADER_SIZE);

  fwrite(header, sizeof(char), HEADER_SIZE, outfile);
  fwrite(pixels, sizeof(char), height * triple_width, outfile);
  fclose(infile);
  fclose(outfile);
  printf("end oef main\n");
  return 0;

}

//Color inversion method
void colorinversion(FILE *copy1ptr, int width, int height, unsigned char pixels_array[height][width], int pixels_size, char *header_array, int header_size) {

  int h, w;

  for(h = 0; h < height; h++) {
    for(w = 0; w < width; w++) {
      unsigned char temp = pixels_array[h][w];

      temp = ~temp;
      pixels_array[h][w] = temp;
    }
  }

  printf("Wrote out color inversion\n");
}

//Contrast method
void contrast(FILE *copy2ptr, int width, int height, unsigned char pixels_array[height][width], int pixels_size, char *header_array, int header_size) {
  int col_index, row_index;
  unsigned char blue, green, red;

  for(row_index = 0; row_index < height; row_index++) {
    for(col_index = 0; col_index< width; col_index+=3) {
        blue = pixels_array[row_index][col_index];
        pixels_array[row_index][col_index] = bgrvalue(blue);
        green = pixels_array[row_index][col_index+ 1];
        pixels_array[row_index][col_index+ 1] = bgrvalue(green);
        red =  pixels_array[row_index][col_index+ 2];
        pixels_array[row_index][col_index+ 2] = bgrvalue(red);
    }
  }

  printf("Wrote out contrast\n");
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
    printf("Went to mirror\n");
    // [[]] [[]] [[]] [[]] [[]] || [] [] [] [] []


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
          pixels_array[row_index][col_index] = pixels_array[row_index][mirror_index];
          col_index++;
        }
      }
    }

     flip(copy3ptr, width, height, pixels_array, pixels_size, header_array, header_size);


  }

//Partial flip
void flip(FILE *copy3ptr, int width, int height, unsigned char pixels_array[height][width], int pixels_size, char *header_array, int header_size) {
    int row_index, col_index;
    int opposite = 0;
    unsigned char temp_char;


    for(row_index = height - 1; row_index >= height / 2; row_index--) {
      for(col_index = 0; col_index < width; col_index++) {
        temp_char = pixels_array[opposite][col_index];
        pixels_array[opposite][col_index] = pixels_array[row_index][col_index];
        pixels_array[row_index][col_index] = temp_char;
      }
      opposite++;
    }

    printf("Done with mirroring\n");


}

void scaledown(FILE *copy4ptr, int width, int height, unsigned char pixels_array[height][width], int pixels_size, char *header_array, int header_size) {
  unsigned char finished_array[height][width];

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
        finished_array[fheight_index][fwidth_index + width_half] = pixels_array[row_index][col_index];
        finished_array[fheight_index][fwidth_index + width_half + 1] = pixels_array[row_index][col_index + 1];
        finished_array[fheight_index][fwidth_index + width_half + 2] = pixels_array[row_index][col_index + 2];

        finished_array[fheight_index][fwidth_index] = pixels_array[row_index][col_index];
        finished_array[fheight_index][fwidth_index + 1] = 0;
        finished_array[fheight_index][fwidth_index + 2] = 0;

        finished_array[fheight_index + height_half][fwidth_index] = 0;
        finished_array[fheight_index + height_half][fwidth_index + 1] = 0;
        finished_array[fheight_index + height_half][fwidth_index + 2] = pixels_array[row_index][col_index + 2];

        finished_array[fheight_index + height_half][fwidth_index + width_half] = 0;
        finished_array[fheight_index + height_half][fwidth_index + width_half + 1] = pixels_array[row_index][col_index + 1];
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

  memcpy(pixels_array, finished_array, pixels_size);

}
