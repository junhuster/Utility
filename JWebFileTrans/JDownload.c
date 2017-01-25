#include"JWebFileTrans.h"
int main(int argc, char *argv[]){
	
	if(argc!=2){
		printf("argument error\n");
		exit(0);
	}
	
	/** parse protocol **/
	char *link=argv[1];
	char *ptr=NULL;
	ptr=strstr(link,"http://");
	if(ptr!=NULL){
		printf("download link:\n%s\n",link);
		JHttp_download_whole_file(link);
		printf("download success\n");
		return 0;
	}
	
	ptr=strstr(link,"ftp://");
	if(ptr!=NULL){
		printf("ftp download will be supported later\n");
		return 0;
	}else{
		
		printf("not support protocol, download not execute\n");
		return 0;
	}
	
	return 0;
}