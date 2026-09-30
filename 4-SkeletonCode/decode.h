#ifndef DECODE_H
#define DECODE_H

#include "types.h"// Contains user defined types
#include "common.h"
#include <stdio.h>

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4

/*
 * Structure to store information required for
 * decoding secret file from stego image
 * Info about input image and output file
 * is also stored
 */

typedef struct _DecodeInfo
{
    /* Stego Image Info */
    char *stego_image_fname;
    FILE *fptr_stego_image;

    /* Output Secret File Info */
    char *output_secret_fname;
    FILE *fptr_output_secret;
    char extn_secret_file[MAX_FILE_SUFFIX];
    int extn_size;
    long size_secret_file;

} DecodeInfo;

/* Check operation type */
OperationType check_operation_type(char *argv[]);


/* Read and validate Decode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);


/* Perform the decoding */
Status do_decoding(DecodeInfo *decInfo);


/* Get File pointers for input and output files */
Status open_decode_files(DecodeInfo *decInfo);


/* Decode Magic String */
Status decode_magic_string(DecodeInfo *decInfo);


/* Decode secret file extension size */
Status decode_secret_file_extn_size(DecodeInfo *decInfo);


/* Decode secret file extension */
Status decode_secret_file_extn(DecodeInfo *decInfo);


/* Decode secret file size */
Status decode_secret_file_size(DecodeInfo *decInfo);


/* Decode secret file data */
Status decode_secret_file_data(DecodeInfo *decInfo);


/* Decode data from image */
Status decode_data_from_image(char *data, int size, DecodeInfo *decInfo);


/* Decode byte from LSB of image data */
Status decode_lsb_to_byte(char *data, char *image_buffer);

/* Read image bytes */
Status read_image_data(char *image_buffer, int size, FILE *fptr_stego_image);

/* Skip BMP header */
Status skip_bmp_header(FILE *fptr_stego_image);

#endif
