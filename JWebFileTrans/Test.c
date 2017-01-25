#include"JWebFileTrans.h"
void test_http_link_parse(char **url, char **port, char **file_name, char **path);
void test_Http_get_ip_str_from_url(char *url, char **ip_str);
void test_Http_connect_to_server(char *ip, int port, int *socket_fd);
void test_Http_query_file_size(char *path, char *host_ip, char *port, int socket_fd, long long *file_size);
void test_Http_create_breakpoint_file(char *file_name, FILE **fp_breakpoint, int download_size_one_piece,
                                long long file_size, long alread_download_piece_num, long size_of_last_piece,char *download_link);

void test_Http_check_breakpoint_file(char *link, FILE *fp);
void test_Http_create_download_file(char *file_name, FILE **fp_download_file,int part);
void test_Http_restore_orignal_file_name(char *download_part_file_name);
void test_Http_recv_file(int socket_fd, long long range_begin, long long range_end, unsigned char *buffer,long buffer_size,
                                                                                char *path, char *host_ip, char *port);
void test_Update_break_point_file(FILE *fp, int new_download_piece_num);
void test_Save_download_part_of_file(FILE *fp, unsigned char *buffer, long buffer_size, long long file_offset);
void test_JHttp_download_one_piece(int socket_fd,  long long range_begin,long long range_end,FILE *fp_download,FILE *fp_jbp,
                            unsigned char *buffer,long buffer_size,long long offset,char *path, char *host_ip, char *port);
void test_JHttp_download_whole_file(char *link);

int main(){
   
    char *link1="http://mirrors.hust.edu.cn/apache/hbase/stable/hbase-1.2.4-bin.tar.gz";
    char *link2="http://mirrors.hust.edu.cn/apache/hbase/stable/hbase-1.2.4-src.tar.gz";
    char *link3="http://www.flashget.com/apps/flashget3.7.0.1222cn.exe";
 
    /*char *url=NULL;
    char *port=NULL;
    char *file_name=NULL;
    char *path=NULL;
    printf("test_http_link_parse********************************\n");
    test_http_link_parse(&url,&port,&file_name,&path);
    printf("\ntest_Http_get_ip_str_from_url********************************\n");
    char *ip_str=NULL;
    test_Http_get_ip_str_from_url(url,&ip_str);
    printf("ip address: %s\n",ip_str);
    printf("\ntest_Http_get_ip_stest_Http_connect_to_server********************************\n");
    int socket_fd=0;
    test_Http_connect_to_server(ip_str,atoi(port),&socket_fd);
    printf("connect success, socket fd is : %d\n",socket_fd);
    printf("\ntest_Http_query_file_size********************************\n");
    long long file_size=0;

    test_Http_query_file_size(path,ip_str,port,socket_fd,&file_size);
    printf("file size: %lld bytes\n",file_size);
    printf("\ntest_Http_create_breakpoint_file********************************\n");
    FILE *fp_breakpoint=NULL;
    test_Http_create_breakpoint_file(file_name,&fp_breakpoint,1024,10240,2,1000,link);
    printf("\ntest_Http_check_breakpoint_file********************************\n");
    test_Http_check_breakpoint_file(link,fp_breakpoint);
    printf("\ntest_Http_create_download_file********************************\n");
    FILE *fp_download_file=NULL;
    test_Http_create_download_file(file_name,&fp_download_file,0);
    printf("\ntest_Http_restore_orignal_file_name********************************\n");
    //test_Http_restore_orignal_file_name("hbase-1.2.4-bin.tar.gz.part0");
    printf("\ntest_Http_recv_file********************************\n");
    char *buffer=(char *)malloc(sizeof(char)*2017);
    test_Http_recv_file(socket_fd,0,1000,buffer,1500,path,ip_str,port);
    printf("\ntest_Update_break_point_file********************************\n");
    test_Update_break_point_file(fp_breakpoint,6);
    printf("\ntest_Save_download_part_of_file********************************\n");
    test_Save_download_part_of_file(fp_download_file,buffer,1000,0);
    printf("\ntest_JHttp_download_one_piece********************************\n");
    test_JHttp_download_one_piece(socket_fd,0,1000,fp_download_file,fp_breakpoint,buffer,1500,0,path,ip_str,port);*/
    test_JHttp_download_whole_file(link1);
    return 0;
}

void test_http_link_parse(char **url, char **port, char **file_name, char **path){

    char *link="http://www-us.apache.org/dist/hbase/stable/hbase-1.2.4-bin.tar.gz";

    Http_link_parse(link, url, port, path,file_name);
    printf("url: %s\nport: %s\nfile_name: %s\n",*url,*port,*file_name);
    printf("path: %s\n",*path);
    return;

}

void test_Http_get_ip_str_from_url(char *url, char **ip_str){
        Http_get_ip_str_from_url(url,ip_str);
        return;
}

void test_Http_connect_to_server(char *ip, int port, int *socket_fd){
    Http_connect_to_server(ip,port,socket_fd);
    return;
}

void test_Http_query_file_size(char *path, char *host_ip, char *port, int socket_fd, long long *file_size){
    Http_query_file_size(path,host_ip,port,socket_fd,file_size);
    return;
}

void test_Http_create_breakpoint_file(char *file_name, FILE **fp_breakpoint, int download_size_one_piece,
                                long long file_size, long alread_download_piece_num, long size_of_last_piece,char *download_link){
    Http_create_breakpoint_file(file_name,fp_breakpoint,download_size_one_piece,file_size,alread_download_piece_num,
                                size_of_last_piece,download_link);
    return;
}

void test_Http_check_breakpoint_file(char *link, FILE *fp){
    int ret=Http_check_breakpoint_file(link,fp);printf("%s\n",link);
    if(ret==jbp_file_not_correct){
        printf("break point file not correct\n");

    }else if(ret==jbp_file_correct){
        printf("break point file correct\n");
    }else if(ret==jbp_file_empty){
        printf("break point file empty\n");
    }else{
        printf("un recognition ret value faced\n");
    }

    return;
}

void test_Http_create_download_file(char *file_name, FILE **fp_download_file,int part){
    Http_create_download_file(file_name,fp_download_file,part);
}

void test_Http_restore_orignal_file_name(char *download_part_file_name){
    Http_restore_orignal_file_name(download_part_file_name);
    return;
}

void test_Http_recv_file(int socket_fd, long long range_begin, long long range_end, unsigned char *buffer,long buffer_size,
                                                                                char *path, char *host_ip, char *port){

    Http_recv_file(socket_fd,0,1000,buffer,1600,path,host_ip,port);
    char *ptr=strstr(buffer,"\r\n\r\n");
    *(ptr+4)='\0';
    printf("%s\n",buffer);
    return;
}

void test_Update_break_point_file(FILE *fp, int new_download_piece_num){
    Update_break_point_file(fp,new_download_piece_num);
    return;
}

void test_Save_download_part_of_file(FILE *fp, unsigned char *buffer, long buffer_size, long long file_offset){

    Save_download_part_of_file(fp,buffer,buffer_size,file_offset);
    return;
}

void test_JHttp_download_one_piece(int socket_fd,  long long range_begin,long long range_end,FILE *fp_download,FILE *fp_jbp,
                            unsigned char *buffer,long buffer_size,long long offset,char *path, char *host_ip, char *port){

            JHttp_download_one_piece(socket_fd,range_begin,range_end,fp_download,fp_jbp,buffer,buffer_size,offset,path,
                                     host_ip,port);
            return;
}

void test_JHttp_download_whole_file(char *link){
    JHttp_download_whole_file(link);
    return;
}
