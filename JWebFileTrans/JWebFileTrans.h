#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

#include <unistd.h>

#define jbp_file_not_correct 0
#define jbp_file_correct 1
#define jbp_file_empty 2

#define max_download_thread 8
#define download_one_piece_size 1024*200

/**
 ** several fix laster will be added such as:
 ** download process bar
 ** check recv buffer "content-length"
 ** real break point support
 */

typedef struct break_point{

    int download_size_one_piece;
    long long file_size;
    long  alread_download_piece_num;
    long size_of_last_piece;

}break_point;

int Http_link_parse(char *link, char **url, char **port, char **path, char **file_name);
int Http_get_ip_str_from_url(char *url, char **ip_str);
int Http_connect_to_server(char *ip, int port, int *socket_fd);
int Http_query_file_size(char *path, char *host_ip, char *port, int socket_fd, long long *file_size);

int Http_create_breakpoint_file(char *file_name, FILE **fp_breakpoint, int download_size_one_piece,
                                long long file_size, long alread_download_piece_num, long size_of_last_piece,char *download_link);
int Http_create_download_file(char *file_name, FILE **fp_download_file,int part);
int Http_restore_orignal_file_name(char *download_part_file_name);
int Http_check_breakpoint_file(char *link,FILE *fp);

int Http_recv_file(int socket_fd, long long range_begin, long long range_end, unsigned char *buffer,long buffer_size,
                                                                                char *path, char *host_ip, char *port);
int Save_download_part_of_file(FILE *fp, unsigned char *buffer, long buffer_size, long long file_offset);
int Update_break_point_file(FILE *fp, int new_download_piece_num);

int JHttp_download_one_piece(int socket_fd,  long long range_begin,long long range_end,FILE *fp_download,FILE *fp_jbp,
                            unsigned char *buffer,long buffer_size,long long offset,char *path, char *host_ip, char *port);


int JHttp_download_whole_file(char *link);
