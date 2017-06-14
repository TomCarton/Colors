// Colors
// main.c
//
// Created by Thomas CARTON
//

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#include "../libraries/image/image.h"


bool control(Image image)
{
    bool result = true;

    char val[32768];
    int min = 32767, max = 0;

    for (unsigned int i = 0; i < 32768; ++i)
        val[i] = 0;

    for (unsigned int y = 0; y < image.height; ++y)
    {
        for (unsigned int x = 0; x < image.width; ++x)
        {
            unsigned char *pix = image.data + ((y * image.width) + x) * SizeForPixelFormat(kPixelFormatRGB24);

            unsigned short c = ((unsigned int)*pix++ * 31 / 255) << 10;
            c += ((unsigned int)*pix++ * 31 / 255) << 5;
            c += ((unsigned int)*pix++ * 31 / 255);

            if (c < min) min = c;
            if (c > max) max = c;

            val[c] = 1;
        }
    }

    printf("min %d\nmax %d\n\n", min, max);

    for (unsigned int i = 0; i < 32768; ++i)
    {
        if (val[i] == 0)
        {
            printf("missing color 0x%04X\n", i);

            result = false;
        }
    }
    
    return result;
}

void all15bits(Image image)
{
    unsigned char *p = image.data;
    for (unsigned int k = 0; k < 32768; ++k)
    {
        *p++ = (((k >> 10) & 0b11111) * 255) / 31;
        *p++ = (((k >> 5) & 0b11111) * 255) / 31;
        *p++ = ((k & 0b11111) * 255) / 31;
    }
}

void all15bits2(Image image)
{
    unsigned char *p = image.data;
    for (unsigned int k = 0; k < 32768; ++k)
    {
        unsigned char r = (k & 0b000000000000100) >> 2;
        r |= (k & 0b000000000100000) >> 4;
        r |= (k & 0b000000100000000) >> 6;
        r |= (k & 0b000100000000000) >> 8;
        r |= (k & 0b100000000000000) >> 10;
        unsigned char g = (k & 0b000000000000010) >> 1;
        g |= (k & 0b000000000010000) >> 3;
        g |= (k & 0b000000010000000) >> 5;
        g |= (k & 0b000010000000000) >> 7;
        g |= (k & 0b010000000000000) >> 9;
        unsigned char b = (k & 0b000000000000001);
        b |= (k & 0b000000000001000) >> 2;
        b |= (k & 0b000000001000000) >> 4;
        b |= (k & 0b000001000000000) >> 6;
        b |= (k & 0b001000000000000) >> 8;

        *p++ = (r * 255) / 31;
        *p++ = (g * 255) / 31;
        *p++ = (b * 255) / 31;
    }
}

void randomize(Image image)
{
    srand(time(NULL));

    unsigned char *p = image.data;
    for (unsigned int y = 0; y < image.height; ++y)
    {
        for (unsigned int x = 0; x < image.width; ++x)
        {
            *p++ = (unsigned char)((double)rand() * 255 / RAND_MAX);
            *p++ = (unsigned char)((double)rand() * 255 / RAND_MAX);
            *p++ = (unsigned char)((double)rand() * 255 / RAND_MAX);
        }
    }
}

void diffu(Image image)
{
    unsigned char *p = image.data;
    for (unsigned int y = 0; y < image.height; ++y)
    {
        for (unsigned int x = 0; x < image.width; ++x)
        {
            *p++ = (x * 255) / image.width;
            *p++ = (y * 255) / image.height;
            *p++ = (unsigned char)((double)rand() * 255 / RAND_MAX);
        }
    }
}

void rain(Image image)
{
    srand(time(NULL));

    for (unsigned int k = 0; k < 32768; ++k)
    {
        unsigned int x, y;
        unsigned char *pix;

        while (1)
        {
            x = (unsigned int)((double)rand() * image.width / RAND_MAX);
            y = (unsigned int)((double)rand() * image.height / RAND_MAX);
            pix = image.data + ((y * image.width) + x) * 3;

            if ((pix[0] | pix[1] | pix[2]) == 0)
                break;
        }

        *pix++ = (((k >> 10) & 0b11111) * 255) / 31;
        *pix++ = (((k >> 5) & 0b11111) * 255) / 31;
        *pix++ = ((k & 0b11111) * 255) / 31;
    }
}

void disk(Image image)
{
    int side = image.width > image.height ? image.width >> 1 : image.height >> 1;
    int dmax = sqrt(2 * side * side) * 0.99;
    unsigned char *pix;

    for (unsigned int y = 0; y < image.height; ++y)
    {
        for (unsigned int x = 0; x < image.width; ++x)
        {
            pix = image.data + ((y * image.width) + x) * 3;

            int xx = x - (image.width / 2);
            int yy = y - (image.height / 2);
            int d = sqrt(xx * xx + yy * yy) * 32767 / dmax;

            if (d > 32767) d = 32767;
            d = 32767 - d;

            unsigned char r = (d & 0b000000000000100) >> 2;
            r |= (d & 0b000000000100000) >> 4;
            r |= (d & 0b000000100000000) >> 6;
            r |= (d & 0b000100000000000) >> 8;
            r |= (d & 0b100000000000000) >> 10;
            unsigned char g = (d & 0b000000000000010) >> 1;
            g |= (d & 0b000000000010000) >> 3;
            g |= (d & 0b000000010000000) >> 5;
            g |= (d & 0b000010000000000) >> 7;
            g |= (d & 0b010000000000000) >> 9;
            unsigned char b = (d & 0b000000000000001);
            b |= (d & 0b000000000001000) >> 2;
            b |= (d & 0b000000001000000) >> 4;
            b |= (d & 0b000001000000000) >> 6;
            b |= (d & 0b001000000000000) >> 8;

            *pix++ = (r * 255) / 31;
            *pix++ = (g * 255) / 31;
            *pix++ = (b * 255) / 31;
        }
    }
}

unsigned char radius(int x, int y, int xc, int yc, int rc)
{
    int dx = x - xc;
    int dy = y - yc;
    int d = sqrt(dx * dx + dy * dy);
    if (d > rc) d = rc;
    return (d * d) >> 8;
}

void disks(Image image)
{
    int side = image.width > image.height ? image.width >> 1 : image.height >> 1;
    int dmax = sqrt(2 * side * side) * 0.99;

    int rx = 0, ry = 0, rr = 255;
    int gx = image.width >> 1, gy = image.height >> 1, gr = 255;
    int bx = 0, by = image.height, br = 255;

    unsigned char *pix;
    for (unsigned int y = 0; y < image.height; ++y)
    {
        for (unsigned int x = 0; x < image.width; ++x)
        {
            pix = image.data + ((y * image.width) + x) * 3;

            *pix++ = 255 - radius(x, y, rx, ry, rr);
            *pix++ = 255 - radius(x, y, gx, gy, gr);
            *pix++ = 255 - radius(x, y, bx, by, br);
        }
    }
}

int main(int argc, char *argv[])
{
    Image image;
    image.width = 256;
    image.height = 256;
    image.data = (unsigned char *)malloc(image.width * image.height * 3);

    Error err;
    disks(image);

//    if (control(image) == false)
//        printf("image not correct\n\n");

//    Error err = ImageSaveTarga(image, "colors.tga");
//    printf("Write tga, result = 0x%04X\n", err);
    err = ImageSavePNG(image, "colors.png");
    printf("Write png, result = 0x%04X\n", err);


    return 0;
}
