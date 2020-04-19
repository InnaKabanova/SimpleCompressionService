#ifndef SIMPLE_COMPR_SERVICE_ALGORITHMS
#define SIMPLE_COMPR_SERVICE_ALGORITHMS

// In-place run-length encoding:

#define MAX_RL_ENCODING_STR_LEN UINT8_MAX
#define RL_ALGO_DEBUG

typedef enum rl_encoding_status
{
    RL_ENC_INVALID_ARGS,
    RL_ENC_BAD_STR_LEN,
    RL_ENC_BAD_STR_FRMT,
    RL_ENC_NOT_COMPRSD,
    RL_ENC_OK_COMPRSD
} rl_encoding_status_t;

typedef enum rl_decoding_status
{
    RL_DEC_INVALID_ARGS,
    RL_DEC_BAD_STR_FRMT,
    RL_DEC_NOT_DCOMPRSD,
    RL_DEC_OK_DCOMPRSD
} rl_decoding_status_t;

rl_encoding_status_t encode_str_rl(char *inout_str,
                                   unsigned int* inout_len);
rl_decoding_status_t decode_str_rl(char* in_str, char** out_str,
                                   unsigned int* inout_len);
#ifdef RL_ALGO_DEBUG
void test_rl_encoding(const char* dump_file);
void test_rl_decoding();
#endif

#endif /* SIMPLE_COMPR_SERVICE_ALGORITHMS */
