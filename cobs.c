#include <stdint.h>
#include <string.h>

/**
 * @brief  COBS 编码
 * @param  input    原始字节流（可含 0x00）
 * @param  in_len   原始数据长度
 * @param  output   编码后输出（无 0x00）
 * @return 编码后长度
 */
int cobs_encode(const uint8_t *input, int in_len, uint8_t *output)
{
    // 编码指针 & 结束指针
    uint8_t *encode_ptr = output;
    // 块头指针（存放当前块到下一个 0x00 的距离）
    uint8_t *block_header_ptr = encode_ptr++;
    uint8_t block_len = 1;

    for (int i = 0; i < in_len; i++)
    {
        // 遇到 0x00，结束当前块
        if (input[i] == 0)
        {
            *block_header_ptr = block_len;
            block_header_ptr = encode_ptr++;
            block_len = 1;
        }
        else
        {
            *encode_ptr++ = input[i];
            block_len++;
        }
    }

    // 写入最后一个块头
    *block_header_ptr = block_len;
    // 返回编码总长度
    return encode_ptr - output;
}

/**
 * @brief  COBS 解码
 * @param  input    编码后数据（无 0x00）
 * @param  in_len   编码数据长度
 * @param  output   解码还原后数据
 * @return 解码后长度，-1=解码失败
 */
int cobs_decode(const uint8_t *input, int in_len, uint8_t *output)
{
    const uint8_t *in_end = input + in_len;
    uint8_t *decode_ptr = output;
    uint8_t block_len;

    while (input < in_end)
    {
        block_len = *input++;
        // 非法长度（越界）
        if (block_len == 0 || input + block_len - 1 > in_end)
            return -1;

        // 复制块内数据
        for (int i = 1; i < block_len; i++)
            *decode_ptr++ = *input++;

        // 不是最后一块 → 补 0x00
        if (input < in_end)
            *decode_ptr++ = 0x00;
    }

    return decode_ptr - output;
}
