#include "algo.h"

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#ifdef RL_ALGO_DEBUG
#include <stdio.h>
#include <assert.h>
#endif

#define NULL_VALUE 45

static int is_ascii_letter(char ch)
{
    // [65 (Uppercase A), 90 (Uppercase Z)]
    // [97 (Lowercase a), 122 (Lowercase z)]
    if((ch >= 65 && ch <= 90) || (ch >= 97 && ch <= 122))
        return 1;
    else
        return 0;
}

static int is_ascii_number(char ch)
{
    // [48 ('0'), 57 ('9')]
    if(ch >= 48 && ch <= 57)
        return 1;
    else
        return 0;
}

static char num2asciichar(unsigned int num)
{
    switch (num)
    {
    case 0: return '0';
    case 1: return '1';
    case 2: return '2';
    case 3: return '3';
    case 4: return '4';
    case 5: return '5';
    case 6: return '6';
    case 7: return '7';
    case 8: return '8';
    case 9: return '9';
    default: return 0;
    }
}

static int asciichar2num(char ch)
{
    if(!is_ascii_number(ch))
        return 0;
    else
        return (ch - 48);
}

static int update_ref_value_num_record(int ref_value_num,
                                       char* inout_str,
                                       unsigned int ref_value_num_begin_idx)
{
    if(NULL == inout_str || ref_value_num_begin_idx >= strlen(inout_str)) return 0;

    if(ref_value_num <= 9) // 1 digit
    {
        inout_str[ref_value_num_begin_idx] = num2asciichar(ref_value_num);
        return ref_value_num_begin_idx;
    }
    else if(ref_value_num >= 10 && ref_value_num <= 99) // 2 digits
    {
        inout_str[ref_value_num_begin_idx] = num2asciichar(ref_value_num / 10);
        inout_str[ref_value_num_begin_idx + 1] = num2asciichar(ref_value_num % 10);
        return ref_value_num_begin_idx + 1;
    }
    else if(ref_value_num >= 100 && ref_value_num <= UINT8_MAX) // 3 digits
    {
        inout_str[ref_value_num_begin_idx] = num2asciichar(ref_value_num / 10 / 10);
        inout_str[ref_value_num_begin_idx + 1] = num2asciichar(ref_value_num / 10 % 10);
        inout_str[ref_value_num_begin_idx + 2] = num2asciichar(ref_value_num % 10 % 10);
        return ref_value_num_begin_idx + 2;
    }
    else
        return 0;
}

static int get_number_of_digits(int num)
{
    int ret = 1;
    while(num / 10 != 0)
    {
        ret++;
        num /= 10;
    }
    return ret;
}

static int get_number(const char* in_str, unsigned int len, unsigned int curr_idx)
{
    if(NULL == in_str || 0 == len || strlen(in_str) != len ||
       curr_idx >= strlen(in_str))
        return 0;

    int ret = asciichar2num(in_str[curr_idx]);
    if(0 == ret)
        return 0;

    int temp = 0;
    while(curr_idx < len)
    {
        curr_idx++;
        temp = asciichar2num(in_str[curr_idx]);
        if(0 == temp)
            return ret;
        else
        {
            ret *= 10;
            ret += temp;
        }
    }
    return ret;
}

rl_encoding_status_t encode_str_rl(char* inout_str, unsigned int* inout_len)
{
    if(NULL == inout_str || NULL == inout_len || 0 == *inout_len)
        return RL_ENC_INVALID_ARGS;

    unsigned int len = strlen(inout_str);
    if(len != *inout_len)
        return RL_ENC_INVALID_ARGS;
    else if(len > MAX_RL_ENCODING_STR_LEN)
        return RL_ENC_BAD_STR_LEN;
    else if(len <= 2)
        return RL_ENC_NOT_COMPRSD;

    // Current reference value:
    char ref_value = inout_str[0];
    // Number of occurances of the current reference value yet discovered:
    unsigned int ref_value_num = 1;
    // Index of the first digit of 'ref_value_num' within 'inout_str':
    unsigned int ref_value_num_begin_idx = 1;
    // Index of the last digit of 'ref_value_num' within 'inout_str':
    unsigned int ref_value_num_end_idx = 1;
    // Current value under processing:
    char curr_value;
    // Index of the fisrt digit within the sub-array of nulled chars:
    unsigned int null_begin_idx = 1;

    // Before launching the algo, check the first reference value:
    if(!is_ascii_letter(ref_value))
        return RL_ENC_BAD_STR_FRMT;

    for(unsigned int i = 1; i < len; i++)
    {
        curr_value = inout_str[i];
        if(!is_ascii_letter(curr_value))
            return RL_ENC_BAD_STR_FRMT;

        if(curr_value == ref_value)
        {
            ref_value_num++;
            inout_str[i] = NULL_VALUE;
            ref_value_num_end_idx = update_ref_value_num_record(
                        ref_value_num, inout_str, ref_value_num_begin_idx);
            null_begin_idx = ref_value_num_end_idx + 1;
        }
        else
        {
            inout_str[i] = NULL_VALUE;
            inout_str[null_begin_idx] = curr_value;
            ref_value = curr_value;
            ref_value_num = 1;
            ref_value_num_begin_idx = null_begin_idx + 1;
            ref_value_num_end_idx = ref_value_num_begin_idx;
            null_begin_idx++;
        }
    }

    // 'null_begin_idx' is the resulting array length:
    if(null_begin_idx == len)
        return RL_ENC_NOT_COMPRSD;

    *inout_len = null_begin_idx;
    return RL_ENC_OK_COMPRSD;
}

rl_decoding_status_t decode_str_rl(char* in_str, char** out_str,
                                   unsigned int* inout_len)
{
    if(NULL == in_str || NULL == inout_len || 0 == *inout_len ||
       NULL == out_str || NULL != *out_str)
        return RL_DEC_INVALID_ARGS;

    unsigned int bytes_to_allocate = 0;

    unsigned int i = 0;
    while(i < *inout_len)
    {
        if(is_ascii_letter(in_str[i]))
        {
            bytes_to_allocate++;
            i++;
        }
        else if(is_ascii_number(in_str[i]))
        {
            int num = get_number(in_str, *inout_len, i);
            if(0 == num)
                return RL_DEC_BAD_STR_FRMT;
             bytes_to_allocate += (num - 1);
             i += get_number_of_digits(num);
        }
        else
            return RL_DEC_BAD_STR_FRMT;
    }

    if(bytes_to_allocate == *inout_len)
        return RL_DEC_NOT_DCOMPRSD;

    *out_str = malloc(sizeof(char) * bytes_to_allocate);

    i = 0;
    char curr_char;
    char* out_ptr = *out_str;

    while(i < *inout_len)
    {
        curr_char = in_str[i];
        int num = get_number(in_str, *inout_len, i + 1);
        if(0 == num)
        {
            // There is only one instance of curr_char:
            *out_ptr = curr_char;
            out_ptr++;
            i++;
        }
        else
        {
            memset(out_ptr, curr_char, num);
            out_ptr += num;
            i += (get_number_of_digits(num) + 1);
        }
    }

    *inout_len = bytes_to_allocate;
    return RL_DEC_OK_DCOMPRSD;
}

#ifdef RL_ALGO_DEBUG
static void print_array(FILE* fp, char* arr, unsigned int len)
{
    if(NULL == fp || NULL == arr)
        return;
    for(unsigned int i = 0; i < len; i++)
        fprintf(fp, "%c | ", arr[i]);
}

void test_rl_encoding(const char* dump_file)
{
    FILE* fp = fopen(dump_file, "w");
    if(fp == NULL)
        return;

    rl_encoding_status_t ret = RL_ENC_INVALID_ARGS;

    ret = encode_str_rl(NULL, NULL);
    assert(ret == RL_ENC_INVALID_ARGS);

    unsigned int len_0 = 0;
    char inout_arr_0[] = "xx";
    ret = encode_str_rl(inout_arr_0, &len_0);
    assert(ret == RL_ENC_INVALID_ARGS);

    len_0 = 5;
    ret = encode_str_rl(inout_arr_0, &len_0);
    assert(ret == RL_ENC_INVALID_ARGS);

    len_0 = strlen(inout_arr_0);
    ret = encode_str_rl(inout_arr_0, &len_0);
    assert(ret == RL_ENC_NOT_COMPRSD);

    char inout_arr_1[] = "wwwwZZZa";
    unsigned int len_1 = strlen(inout_arr_1);
    fprintf(fp, "\nInput: (len %i)\n", len_1);
    print_array(fp, inout_arr_1, strlen(inout_arr_1));
    ret = encode_str_rl(inout_arr_1, &len_1);
    assert(ret == RL_ENC_OK_COMPRSD);
    fprintf(fp, "\nOutput: (len %i)\n", len_1);
    print_array(fp, inout_arr_1, strlen(inout_arr_1));
    fprintf(fp, "\n______________________________\n");

    char inout_arr_2[] = "wwwwwwwwwwwwaaaa";
    unsigned int len_2 = strlen(inout_arr_2);
    fprintf(fp, "\nInput: (len %i)\n", len_2);
    print_array(fp, inout_arr_2, strlen(inout_arr_2));
    ret = encode_str_rl(inout_arr_2, &len_2);
    assert(ret == RL_ENC_OK_COMPRSD);
    fprintf(fp, "\nOutput: (len %i)\n", len_2);
    print_array(fp, inout_arr_2, strlen(inout_arr_2));
    fprintf(fp, "\n______________________________\n");

    char inout_arr_3[] = "wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww"
                         "wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww"
                         "wwwwwwwwwwwwAAAAz";
    unsigned int len_3 = strlen(inout_arr_3);
    fprintf(fp, "\nInput: (len %i)\n", len_3);
    print_array(fp, inout_arr_3, strlen(inout_arr_3));
    ret = encode_str_rl(inout_arr_3, &len_3);
    assert(ret == RL_ENC_OK_COMPRSD);
    fprintf(fp, "\nOutput: (len %i)\n", len_3);
    print_array(fp, inout_arr_3, strlen(inout_arr_3));
    fprintf(fp, "\n______________________________\n");

    char inout_arr_4[] = "wZaSb";
    unsigned int len_4 = strlen(inout_arr_4);
    fprintf(fp, "\nInput: (len %i)\n", len_4);
    print_array(fp, inout_arr_4, strlen(inout_arr_4));
    ret = encode_str_rl(inout_arr_4, &len_4);
    assert(ret == RL_ENC_NOT_COMPRSD);
    fprintf(fp, "\nOutput: (len %i)\n", len_4);
    print_array(fp, inout_arr_4, strlen(inout_arr_4));
    fprintf(fp, "\n______________________________\n");

    fclose(fp);
}

void test_rl_decoding()
{
    char in_str[] = "W3a5b6";
    char* out_str = NULL;
    unsigned int len_0 = 0;
    assert(RL_DEC_INVALID_ARGS == decode_str_rl(NULL, &out_str, &len_0));
    assert(RL_DEC_INVALID_ARGS == decode_str_rl(in_str, &out_str, NULL));
    assert(RL_DEC_INVALID_ARGS == decode_str_rl(in_str, &out_str, &len_0));
    len_0 = strlen(in_str);
    assert(RL_DEC_INVALID_ARGS == decode_str_rl(in_str, NULL, &len_0));
    out_str = in_str;
    assert(RL_DEC_INVALID_ARGS == decode_str_rl(in_str, &out_str, &len_0));
    out_str = NULL;
    assert(RL_DEC_OK_DCOMPRSD == decode_str_rl(in_str, &out_str, &len_0));
    assert(0 == strncmp("WWWaaaaabbbbbb", out_str, len_0));
    free(out_str);
}
#endif

