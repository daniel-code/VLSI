#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    double Y, U, V;
    unsigned int countnum = 0;
    unsigned int x, y;
    unsigned int width, height;
    unsigned char *image = NULL; // source image array
    unsigned char *image_Y = NULL; // target image array
    unsigned char *image_U = NULL; // target image array
    unsigned char *image_V = NULL; // target image array
    unsigned char *image_WB = NULL; // convert back image array
    unsigned char R, G, B;         // color of R, G, B
    int i,j;

    unsigned int file_size;           // file size
    unsigned int rgb_raw_data_offset; // RGB raw data offset
    unsigned short bit_per_pixel;     // bit per pixel
    unsigned short byte_per_pixel;

    unsigned char header[54] =
    {
        0x42,        // identity : B
        0x4d,        // identity : M
        0, 0, 0, 0,  // file size
        0, 0,        // reserved1
        0, 0,        // reserved2
        54, 0, 0, 0, // RGB data offset
        40, 0, 0, 0, // struct BITMAPINFOHEADER size
        0, 0, 0, 0,  // bmp width
        0, 0, 0, 0,  // bmp height
        1, 0,        // planes
        24, 0,       // bit per pixel
        0, 0, 0, 0,  // compression
        0, 0, 0, 0,  // data size
        0, 0, 0, 0,  // h resolution
        0, 0, 0, 0,  // v resolution
        0, 0, 0, 0,  // used colors
        0, 0, 0, 0   // important colors
    };

    FILE *fp = fopen( "mountain256.bmp", "rb" );

    if( fp == NULL )
    {
        printf( "open fail.\n" );
        return -1;
    }
    // move offset to 10 to find rgb raw data offset
    fseek(fp, 10, SEEK_SET);
    fread(&rgb_raw_data_offset, sizeof(unsigned int), 1, fp);
    // move offset to 18    to get width & height;
    fseek(fp, 18, SEEK_SET);
    fread(&width,  sizeof(unsigned int), 1, fp);
    fread(&height, sizeof(unsigned int), 1, fp);
    // get  bit per pixel
    fseek(fp, 28, SEEK_SET);
    fread(&bit_per_pixel, sizeof(unsigned short), 1, fp);
    byte_per_pixel = bit_per_pixel / 8;
    // move offset to rgb_raw_data_offset to get RGB raw data
    fseek(fp, rgb_raw_data_offset, SEEK_SET);

    image = (unsigned char *)malloc((size_t)width * height * byte_per_pixel);
    image_Y = (unsigned char *)malloc((size_t)width * height * byte_per_pixel);
    image_U = (unsigned char *)malloc((size_t)width * height * byte_per_pixel);
    image_V = (unsigned char *)malloc((size_t)width * height * byte_per_pixel);
    image_WB = (unsigned char *)malloc((size_t)width * height * byte_per_pixel);


    fread(image, sizeof(unsigned char), (size_t)(long)width * height * byte_per_pixel, fp);
    for(i=0; i<(size_t)(long)width * height * byte_per_pixel-3; i=i+3)
    {
        R = image[i];
        G = image[i+1];
        B = image[i+2];
        image_Y[i] = (0.299*R + 0.587*G + 0.114*B);
        image_Y[i+1] = (0.299*R + 0.587*G + 0.114*B);
        image_Y[i+2] = (0.299*R + 0.587*G + 0.114*B);
        image_U[i] = (-0.169*R - 0.331*G + 0.5*B + 128);
        image_U[i+1] = (-0.169*R - 0.331*G + 0.5*B + 128);
        image_U[i+2] = (-0.169*R - 0.331*G + 0.5*B + 128);
        image_V[i] = (0.5*R - 0.419*G - 0.081*B + 128);
        image_V[i+1] = (0.5*R - 0.419*G - 0.081*B + 128);
        image_V[i+2] = (0.5*R - 0.419*G - 0.081*B + 128);
    }

    for(i=0; i<(size_t)(long)width * height * byte_per_pixel-3; i=i+3)
    {
        image_WB[i] = (image_Y[i]+1.4021*(image_V[i]-128));
        image_WB[i+1] = (image_Y[i] - 0.34414*(image_U[i]-128) - 0.71414*(image_V[i]-128));
        image_WB[i+2] = (image_Y[i] + 1.7718*(image_U[i]-128));
    }



    // printf("%d\n", (size_t)(long)width * height * byte_per_pixel);
    FILE *fp_YUV = fopen( "YUV_value.txt", "w" );
    FILE *fp_RGB = fopen( "RGB_value.txt", "w" );





    // write to new bmp
    FILE *fp_Y = fopen("mountain256_Y.bmp", "wb");
    FILE *fp_U = fopen("mountain256_U.bmp", "wb");
    FILE *fp_V = fopen("mountain256_V.bmp", "wb");
    FILE *fp_WB = fopen("mountain256_WB.bmp", "wb" );

    // file size
    file_size = width * height * byte_per_pixel + rgb_raw_data_offset;
    header[2] = (unsigned char)(file_size & 0x000000ff);
    header[3] = (file_size >> 8)  & 0x000000ff;
    header[4] = (file_size >> 16) & 0x000000ff;
    header[5] = (file_size >> 24) & 0x000000ff;

    // width
    header[18] = width & 0x000000ff;
    header[19] = (width >> 8)  & 0x000000ff;
    header[20] = (width >> 16) & 0x000000ff;
    header[21] = (width >> 24) & 0x000000ff;

    // height
    header[22] = height &0x000000ff;
    header[23] = (height >> 8)  & 0x000000ff;
    header[24] = (height >> 16) & 0x000000ff;
    header[25] = (height >> 24) & 0x000000ff;

    // bit per pixel
    header[28] = bit_per_pixel;

    // write header
    fwrite(header, sizeof(unsigned char), rgb_raw_data_offset, fp_Y);
    fwrite(header, sizeof(unsigned char), rgb_raw_data_offset, fp_U);
    fwrite(header, sizeof(unsigned char), rgb_raw_data_offset, fp_V);
    fwrite(header, sizeof(unsigned char), rgb_raw_data_offset, fp_WB);
    // write image
    fwrite(image_Y, sizeof(unsigned char), (size_t)(long)width * height * byte_per_pixel, fp_Y);
    fwrite(image_U, sizeof(unsigned char), (size_t)(long)width * height * byte_per_pixel, fp_U);
    fwrite(image_V, sizeof(unsigned char), (size_t)(long)width * height * byte_per_pixel, fp_V);
    fwrite(image_WB, sizeof(unsigned char), (size_t)(long)width * height * byte_per_pixel, fp_WB);

    fclose(fp);
    fclose(fp_Y);
    fclose(fp_U);
    fclose(fp_V);
    fclose(fp_WB);
    fclose(fp_RGB);
    fclose(fp_YUV);

    return 0;
}
