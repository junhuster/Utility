#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <io.h>
#include <unistd.h>
/**
 ** data structure for Huffman algorithm
 */

typedef struct HuffmanNode{
    unsigned char data_8bit;
    unsigned long long data_8bit_count;
    struct HuffmanNode *parent;
    struct HuffmanNode *left_child;
    struct HuffmanNode *right_child;
}HuffmanNode;

typedef struct HuffmanEncode{
    char *encode;
    long length_of_encode;
}HuffmanEncode;

typedef struct EncodeBuffer{
    unsigned char *buffer;
    long size;
    long bits_num_lastbytes;
}EncodeBuffer;

typedef struct DecodeBuffer{
    unsigned char *buffer;
    long size;
}DecodeBuffer;

typedef struct CompressFileHeader{
    long length_of_header1;
	long length_of_header2;

    unsigned char length_of_author_name;
    unsigned char routine_name_length;
    unsigned char suffix_length;
    unsigned char file_name_length;
	unsigned char bits_of_lastByte;

    char *author_name;
    char *routine_name;
    char *suffix;
    char *file_name;

}CompressFileHeader;

/**
 ** function declaration
 */
int heap_min_adjust(HuffmanNode **huffman_node_array, long data_start, long data_end);
int heap_min_construct(HuffmanNode **huffman_node_array, long array_size);
int heap_min_get2min(HuffmanNode **huffman_node_array, HuffmanNode **min_first, HuffmanNode **min_second, long *heap_size);

int huffman_tree(HuffmanNode **huffman_tree_root, HuffmanNode **huffman_node_array, long array_size);
int huffman_encode(HuffmanNode **huffman_node_array, long huffman_node_array_size,
															HuffmanEncode *huffman_encode_array);
int huffman_decode_internal(HuffmanNode **huffman_tree_current_root, long direction);
int huffman_decode(HuffmanNode *huffman_tree_root, HuffmanNode **last_stop_at, EncodeBuffer *encode_buffer,
                                                                                 DecodeBuffer *decode_buffer
                                                                                 );
int bit_set(unsigned char *ch, long index, long flag);
int open_file(char *filename, FILE **fp);
int creat_file(char *filename, FILE **fp);
int make_uchar_weight(FILE *fp, HuffmanNode **huffman_node, long *size);
int create_Uncompress_file(char *filename, FILE **fp);

int JparseHead(FILE **fp, HuffmanEncode *huffman_encode_array, char **file_name, long *last_bits);
int Jcompress(char *src_name);
int JUnCompress(char *src_name);
int restore_huffman_tree(HuffmanNode **huffman_tree_root, HuffmanEncode *huffman_encode_array);
