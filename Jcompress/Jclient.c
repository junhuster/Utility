#include "Jcompress.h"
int main(int argc, char *argv[]){
	if(argc!=3){
		printf("Useage: program-name num filename\n");
		printf("program-name is the app you run\n");
		printf("num means the operate you want, 0 means compress, 1 means Uncompress\n");
		printf("filename is the file you want to compress or Uncompress\n");
		exit(0);
	}

	char operate=*(argv[1]);
	char *file_name=argv[2];
	if(operate=='0'){
		Jcompress(file_name);
	}else if(operate=='1'){
		JUnCompress(file_name);
	}else{
		printf("Unsuporrted operate, try 0 or 1\n");
	}
	return 0;
}
