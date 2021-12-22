#include "../common.h"
#include "base64.h"

static char encoding_table[] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
    'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
    'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
    'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
    'w', 'x', 'y', 'z', '0', '1', '2', '3',
    '4', '5', '6', '7', '8', '9', '+', '/' };
static int mod_table[] = { 0, 2, 1 };

#define _BASE64_BUF_SIZE 1024

unsigned int print_base64_encode(const unsigned char* data, unsigned int input_length)
{

    unsigned int output_length = 4 * ((input_length + 2) / 3);
    unsigned int real_output_len = output_length;

    // char *encoded_data = malloc(*output_length);
    static char tmp_buf[1030];
    tmp_buf[_BASE64_BUF_SIZE] = '\0';
    tmp_buf[_BASE64_BUF_SIZE + 1] = '\0';
    tmp_buf[_BASE64_BUF_SIZE + 2] = '\0';
    tmp_buf[_BASE64_BUF_SIZE + 3] = '\0';
    tmp_buf[_BASE64_BUF_SIZE + 4] = '\0';
    tmp_buf[_BASE64_BUF_SIZE + 5] = '\0';
    int i = 0, j = 0;
    if (data == NULL)
        return 0;

    for (; i < input_length;) {

        uint32_t octet_a = i < input_length ? (unsigned char)data[i++] : 0;
        uint32_t octet_b = i < input_length ? (unsigned char)data[i++] : 0;
        uint32_t octet_c = i < input_length ? (unsigned char)data[i++] : 0;

        uint32_t triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;

        tmp_buf[j++] = encoding_table[(triple >> 3 * 6) & 0x3F];
        tmp_buf[j++] = encoding_table[(triple >> 2 * 6) & 0x3F];
        tmp_buf[j++] = encoding_table[(triple >> 1 * 6) & 0x3F];
        tmp_buf[j++] = encoding_table[(triple >> 0 * 6) & 0x3F];
        if (j >= _BASE64_BUF_SIZE) {
            j -= _BASE64_BUF_SIZE;
            output_length -= _BASE64_BUF_SIZE;
            printf("%s", tmp_buf);
        }
    }

    for (i = 0; i < mod_table[input_length % 3]; i++)
        tmp_buf[output_length - 1 - i] = '=';
    tmp_buf[output_length] = '\0';
    printf("%s\n", tmp_buf);

    return real_output_len;
}

#ifdef TEST_BASE64
int main()
{
    char buf[1024];
    scanf("%s", buf);
    printf("base64 len = %u\n", print_base64_encode(buf, strlen(buf)));
}
#endif