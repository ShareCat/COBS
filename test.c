#include <stdio.h>

void print_hex(const char *name, const uint8_t *data, int len)
{
    printf("%s: ", name);
    for (int i = 0; i < len; i++)
        printf("%02X ", data[i]);
    printf("\n");
}

int main(void)
{
    // 原始数据（包含 0x00）
    uint8_t raw[] = {0x11, 0x00, 0x22, 0x33, 0x00, 0x44};
    int raw_len = sizeof(raw);

    // 编码缓冲区（最大需要 in_len + 1）
    uint8_t encoded[256];
    // 解码缓冲区
    uint8_t decoded[256];

    // 编码
    int enc_len = cobs_encode(raw, raw_len, encoded);
    // 解码
    int dec_len = cobs_decode(encoded, enc_len, decoded);

    print_hex("原始数据", raw, raw_len);
    print_hex("编码数据(无00)", encoded, enc_len);
    print_hex("解码数据", decoded, dec_len);

    return 0;
}
