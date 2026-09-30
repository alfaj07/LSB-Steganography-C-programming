#include <stdio.h>
#include <string.h>
#include "decode.h"
#include "types.h"
#include "common.h"

/* Read and validate Decode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    // Validate stego image file extension (.bmp)
    if (strstr(argv[2], ".bmp") != NULL)
    {
        decInfo->stego_image_fname = argv[2];
    }
    else
    {
        return e_failure;
    }

    // Optional output secret file name
    if (argv[3] != NULL)
    {
        decInfo->output_secret_fname = argv[3];
    }
    else
    {
        decInfo->output_secret_fname = "output";
    }

    return e_success;
}

/* Open files required for decoding */
Status open_decode_files(DecodeInfo *decInfo)
{
    // Stego Image file
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "r");
    if (decInfo->fptr_stego_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->stego_image_fname);
        return e_failure;
    }

    return e_success;
}
/* Read image data from stego file into buffer */
Status read_image_data(char *buffer, int size, FILE *fptr_stego_image)
{
    if (fread(buffer, size, 1, fptr_stego_image) == 1)
    {
        return e_success;
    }
    return e_failure;
}
/* Decode LSB bit array to a single byte */
Status decode_lsb_to_byte(char *data, char *image_buffer)
{
    *data = 0;
    for (int i = 0; i < 8; i++)
    {
        *data |= ((image_buffer[i] & 1) << i);
    }
    return e_success;
}

/* Decode specified size of data from image */
Status decode_data_from_image(char *data, int size, DecodeInfo *decInfo)
{
    char image_buffer[8];
    for (int i = 0; i < size; i++)
    {
        if (read_image_data(image_buffer, 8, decInfo->fptr_stego_image) != e_success)
        {
            return e_failure;
        }
        decode_lsb_to_byte(&data[i], image_buffer);
    }
    data[size] = '\0';
    return e_success;
}

/* Decode and verify magic string */
Status decode_magic_string(DecodeInfo *decInfo)
{
    char magic_string[10];
    decode_data_from_image(magic_string, strlen(MAGIC_STRING), decInfo);

    printf("Info: Decoded Magic String = %s\n", magic_string);

    if (strcmp(magic_string, MAGIC_STRING) == 0)
    {
        return e_success;
    }
    return e_failure;
}

/* Decode a 32-bit size integer from 32 LSB bits */
Status decode_size_from_lsb(int *size, FILE *fptr_stego_image)
{
    char image_buffer[32];
    if (read_image_data(image_buffer, 32, fptr_stego_image) != e_success)
    {
        return e_failure;
    }
    *size = 0;
    for (int i = 0; i < 32; i++)
    {
        *size |= ((image_buffer[i] & 1) << i);
    }
    return e_success;
}
/* Decode secret file extension size */
Status decode_secret_file_extn_size(DecodeInfo *decInfo)
{
    if (decode_size_from_lsb(&decInfo->extn_size, decInfo->fptr_stego_image) == e_success)
    {
        printf("Info: Decoded secret file extension size = %d\n", decInfo->extn_size);
        return e_success;
    }
    return e_failure;
}
Status decode_secret_file_extn(DecodeInfo *decInfo)
{
    decode_data_from_image(decInfo->extn_secret_file, decInfo->extn_size, decInfo);
    printf("Info: Decoded secret file extension = %s\n", decInfo->extn_secret_file);
    return e_success;
}
Status decode_secret_file_size(DecodeInfo *decInfo)
{
    int size = 0;
    if (decode_size_from_lsb(&size, decInfo->fptr_stego_image) == e_success)
    {
        decInfo->size_secret_file = size;
        printf("Info: Decoded secret file size = %ld bytes\n", decInfo->size_secret_file);
        return e_success;
    }
    return e_failure;
}
Status decode_secret_file_data(DecodeInfo *decInfo)
{
    decInfo->fptr_output_secret = fopen(decInfo->output_secret_fname, "w");
    if (decInfo->fptr_output_secret == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open output file %s\n", decInfo->output_secret_fname);
        return e_failure;
    }
    char secret_data[decInfo->size_secret_file + 1];
    decode_data_from_image(secret_data, decInfo->size_secret_file, decInfo);
    fwrite(secret_data, decInfo->size_secret_file, 1, decInfo->fptr_output_secret);
    fclose(decInfo->fptr_output_secret);
    printf("Info: Secret data written to %s successfully\n", decInfo->output_secret_fname);
    return e_success;
}
Status skip_bmp_header(FILE *fptr_stego_image)
{
    fseek(fptr_stego_image, 54, SEEK_SET);
    return e_success;
}

/* Main decoding workflow */
Status do_decoding(DecodeInfo *decInfo)
{
    // Skip BMP Header (54 bytes)
     if (skip_bmp_header(decInfo->fptr_stego_image) == e_success)
    {
        printf("Info: Skipped 54 bytes BMP header successfully\n");
    }
    else
    {
        printf("Error: Failed to skip BMP header\n");
        return e_failure;
    }
    
    if (decode_magic_string(decInfo) == e_success)
    {
        printf("Info: Magic string decoded successfully\n");
        if (decode_secret_file_extn_size(decInfo) == e_success)
        {
            printf("Info: Secret file extension size decoded successfully\n");
            if (decode_secret_file_extn(decInfo) == e_success)
            {
                printf("Info: Secret file extension decoded successfully\n");
                if (decode_secret_file_size(decInfo) == e_success)
                {
                    printf("Info: Secret file size decoded successfully\n");
                    if (decode_secret_file_data(decInfo) == e_success)
                    {
                        printf("Info: Secret file data decoded successfully\n");
                    }
                    else
                    {
                        printf("Error: Failed to decode secret file data\n");
                        return e_failure;
                    }
                }
                else
                {
                    printf("Error: Failed to decode secret file size\n");
                    return e_failure;
                }
            }
            else
            {
                printf("Error: Failed to decode secret file extension\n");
                return e_failure;
            }
        }
        else
        {
            printf("Error: Failed to decode secret file extension size\n");
            return e_failure;
        }
    }
    else
    {
        printf("Error: Magic string mismatch or decoding failed\n");
        return e_failure;
    }

    return e_success;
}
