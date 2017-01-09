#include"Jcompress.h"
#include <time.h>
void test_heap_min_construct(void);
void test_heap_min_get2min(void);
void test_huffman_tree(void);
void test_in_traverse_tree(HuffmanNode *huffman_tree_root, long level);
void test_pre_traverse_tree(HuffmanNode *huffman_tree_root);
void test_Jcompress(void);
void test_traverse_print_hfmtree(HuffmanNode * huffman_tree_root, long flag);
void test_JparseHead(void);
void test_JUnCompress(void);
void test_2_file(void);
/**
 ** this function test for huffman_encode, huffman_decode_long longernal, huffman_decode
 ** restore_huffman_tree
 */
void test_huffman_encode_and_decode(void);
void test_uchar_weight(void);

int main(int argc, char *argv[])
{
    if(argc!=3){
        printf("argument num error\n");
        exit(0);
    }

    char ch=*(argv[1]);
    char *file_name=argv[2];

    //test_heap_min_construct();
    //test_heap_min_get2min();
    //test_huffman_tree();
    //test_huffman_encode_and_decode();
    //test_uchar_weight();
    if(ch=='0'){
        Jcompress(file_name);
    }

    if(ch=='1'){
        JUnCompress(file_name);
    }
    //test_JparseHead();
    //test_2_file();

    return 0;
}


 void test_heap_min_construct(void){

     const long  data_num=16;
     long  data_max=100;
     long  data_array[data_num+1];
     srand((long)time(0));

     HuffmanNode * huffman_data[data_num+1];
     for(long index=1;index<=data_num;index++){
        huffman_data[index]=(HuffmanNode *)malloc(sizeof(HuffmanNode));
        huffman_data[index]->data_8bit_count=rand()%data_max;
        data_array[index]=huffman_data[index]->data_8bit_count;
     }

    printf("original data\n");
    for(long i=1;i<=data_num;i++){
        if(i%9==0) printf("\n");
        printf("%-3ld, ",data_array[i]);
    }

    heap_min_construct(huffman_data,data_num);
    for(long HeapSize=data_num;HeapSize>1;HeapSize--){
        HuffmanNode *temp_node=huffman_data[1];
        huffman_data[1]=huffman_data[HeapSize];
        huffman_data[HeapSize]=temp_node;
        heap_min_adjust(huffman_data,1,HeapSize-1);
    }
    for(long index=1;index<data_num;index++){
        if((huffman_data[index]->data_8bit_count)<(huffman_data[index+1]->data_8bit_count)){
            printf("sort error\n");
            exit(0);
        }
    }
    printf("\n");
    printf("sort success\n");
    printf("sorted data\n");
    for(long index=data_num;index>=1;index--){
        /** %8 not %9 due to reverted order*/
        if(index%8==0) printf("\n");
        printf("%-3lld, ",huffman_data[index]->data_8bit_count);

    }

    for(long index=1;index<=data_num;index++){
        free(huffman_data[index]);
    }
    return;
 }

 void test_heap_min_get2min(void){
     const long data_num=16;
     long data_max=100;
     long data_array[data_num+1];
     long heap_size=data_num;
     srand((long)time(0));
     HuffmanNode * huffman_data[data_num+1];

     for(long index=1;index<=data_num;index++){
        huffman_data[index]=(HuffmanNode *)malloc(sizeof(HuffmanNode));
        huffman_data[index]->data_8bit_count=rand()%data_max;
        data_array[index]=huffman_data[index]->data_8bit_count;
     }
     printf("orginal data\n");
     for(long i=1;i<=data_num;i++){
        if(i%9==0) printf("\n");
        printf("%-3ld, ",data_array[i]);
     }
     heap_min_construct(huffman_data,data_num);
     HuffmanNode *min1=NULL;
     HuffmanNode *min2=NULL;
     heap_min_get2min(huffman_data,&min1,&min2,&heap_size);
     for(long i=1;i<=data_num;i++){
        if(min1->data_8bit_count==data_array[i]||
           min2->data_8bit_count==data_array[i]) continue;
        if((min1->data_8bit_count>data_array[i])||
           (min2->data_8bit_count>data_array[i])) {
            printf("get2min not correct\n");
            exit(0);
        }
     }

     printf("\n");
     printf("min1:%lld, min2:%lld \n",min1->data_8bit_count,min2->data_8bit_count);
     heap_min_get2min(huffman_data,&min1,&min2,&heap_size);
     return;
 }

void test_huffman_tree(void){
     const long data_num=16;
     long data_max=100;
     long data_array[data_num+1];
     long heap_size=data_num;
     srand((long)time(0));
     HuffmanNode * huffman_data[data_num+1];
     long level=0;

     for(long index=1;index<=data_num;index++){
        huffman_data[index]=(HuffmanNode *)malloc(sizeof(HuffmanNode));
        huffman_data[index]->data_8bit_count=rand()%data_max;
        data_array[index]=huffman_data[index]->data_8bit_count;
        huffman_data[index]->left_child=NULL;
        huffman_data[index]->right_child=NULL;
     }
     printf("orginal data\n");
     for(long i=1;i<=data_num;i++){
        if(i%9==0) printf("\n");
        printf("%-3ld, ",data_array[i]);
     }
     printf("\n");
     HuffmanNode *huffman_tree_root=NULL;
     huffman_tree(&huffman_tree_root,huffman_data,heap_size);
     test_in_traverse_tree(huffman_tree_root,level);

}
void test_in_traverse_tree(HuffmanNode *huffman_tree_root,long level){
    if(huffman_tree_root){
        test_in_traverse_tree(huffman_tree_root->left_child,level+1);
        if(huffman_tree_root->left_child==NULL && huffman_tree_root->right_child==NULL){
            if(huffman_tree_root->parent==NULL){
                printf("error, leaf has no parent\n");
            }
            printf("leaf-1:%-4lld, ",huffman_tree_root->data_8bit_count);
        }
        //printf("[level:%d,value:%lld] ",level,huffman_tree_root->data_8bit_count);
        test_in_traverse_tree(huffman_tree_root->right_child,level+1);

    }
}

void test_huffman_encode_and_decode(void){
     const long data_num=16;
     long data_max=50;
     long data_array[data_num+1];
     long heap_size=data_num;
     srand((long)time(0));
     HuffmanNode * huffman_data[data_num+1];
     HuffmanNode * huffman_data_backup[data_num+1];
     long long level=0;

     for(long index=1;index<=data_num;index++){
        huffman_data[index]=(HuffmanNode *)malloc(sizeof(HuffmanNode));
        huffman_data[index]->data_8bit_count=rand()%data_max;
        data_array[index]=huffman_data[index]->data_8bit_count;
        huffman_data[index]->left_child=NULL;
        huffman_data[index]->right_child=NULL;
        huffman_data[index]->data_8bit=(unsigned char)(index-1);
     }

     for(long i=1;i<=data_num;i++){
        huffman_data_backup[i]=huffman_data[i];
     }

     printf("orginal data\n");
     for(long i=1;i<=data_num;i++){
        if(i%9==0) printf("\n");
        printf("%-3lld, ",huffman_data[i]->data_8bit_count);
     }
     printf("\n");

     HuffmanNode *huffman_tree_root=NULL;
     huffman_tree(&huffman_tree_root,huffman_data,heap_size);

     HuffmanEncode huffman_encode_array[257];
     for(long i=0;i<256;i++){
        huffman_encode_array[i].encode=NULL;
        huffman_encode_array[i].length_of_encode=0;
     }
     /** notice the argument huffman_data_backup */
     huffman_encode(huffman_data_backup,heap_size,huffman_encode_array);
     printf("\n");
     for(long i=0;i<=data_max;i++){
        if(huffman_encode_array[i].encode!=NULL){
            printf("%-2ld: ",i);
            for(long j=0;j<huffman_encode_array[i].length_of_encode;j++){
                char ch=huffman_encode_array[i].encode[j];
                if(!(ch=='0'||ch=='1')){
                    printf("encode not correct: %c\n",ch);
                }
                printf("%c",huffman_encode_array[i].encode[j]);
            }
            printf("\n");
        }

     }
     printf("\nhuffman decode_long longeral test begin\n");

     for(long i=0;i<=data_max;i++){
        HuffmanNode *huffman_tree_root_backup=huffman_tree_root;
        if(huffman_encode_array[i].encode!=NULL){
            for(long j=0;j<huffman_encode_array[i].length_of_encode;j++){
                long direction=(long)(huffman_encode_array[i].encode[j]-'0');
                long temp=huffman_decode_internal(&huffman_tree_root_backup,direction);
                if(temp==1){
                    if(huffman_tree_root_backup->data_8bit==(unsigned char)(i)){
                        printf("encode_array_index:%ld ,",i);
                        printf("encode: ");
                        for(long k=0;k<huffman_encode_array[i].length_of_encode;k++){
                            printf("%c",huffman_encode_array[i].encode[k]);
                        }
                        printf(", original code is : %ld",(long)(huffman_tree_root_backup->data_8bit));
                    }
                }
            }
            printf("\n");
        }

     }

      printf("\nhuffman decode test begin\n");
      printf("the encode is: \n");

      long bit_num=-1;
      EncodeBuffer *en_buffer=(EncodeBuffer *)malloc(sizeof(EncodeBuffer));
      EncodeBuffer *en_buffer1=(EncodeBuffer *)malloc(sizeof(EncodeBuffer));
      en_buffer->buffer=(unsigned char *)malloc(1000*sizeof(unsigned char));
      en_buffer->size=0;
      en_buffer->bits_num_lastbytes=0;

      for(long i=0;i<=data_max;i++){
        if(huffman_encode_array[i].encode!=NULL){
            for(long j=0;j<huffman_encode_array[i].length_of_encode;j++){
                bit_num+=1;
                unsigned char temp=huffman_encode_array[i].encode[j];
                long buffer_index=bit_num/8;
                long bit_offset=bit_num%8;
                bit_set((en_buffer->buffer)+buffer_index,bit_offset,(long)(temp-'0'));
            }

        }
      }
      en_buffer->size=(bit_num+1)/8;
      en_buffer->bits_num_lastbytes=(bit_num+1)%8;
      en_buffer1->bits_num_lastbytes=0;
      en_buffer1->size=2;
      en_buffer1->buffer=en_buffer->buffer;

      en_buffer->size=en_buffer->size-2;
      en_buffer->buffer=(en_buffer->buffer)+2;

      printf("en_buffer_size:%ld\n",en_buffer->size);
      printf("bits_num_lastbytes:%ld\n",en_buffer->bits_num_lastbytes);
      for(long i=0;i<en_buffer->size;i++){
        printf("%02x ",en_buffer->buffer[i]);
      }

      DecodeBuffer *de_buffer=(DecodeBuffer *)malloc(sizeof(DecodeBuffer));
      de_buffer->buffer=(unsigned char *)malloc(sizeof(unsigned char)*1000);
      de_buffer->size=0;
      HuffmanNode *last_stop_at=huffman_tree_root;
      huffman_decode(huffman_tree_root,&last_stop_at,en_buffer1,de_buffer);
      printf("\nthe decode is: \n");
      for(long i=0;i<de_buffer->size;i++){
        printf("%ld, ",(long)(de_buffer->buffer[i]));
      }
      huffman_decode(huffman_tree_root,&last_stop_at,en_buffer,de_buffer);
      printf("\nthe decode is: \n");
      for(long i=0;i<de_buffer->size;i++){
        printf("%ld, ",(long)(de_buffer->buffer[i]));
      }

      /********      restore_huffman_tree test              *******************/

      HuffmanNode *root1;
      printf("\n");
      printf("restore_huffman_tree begin:\n");
      restore_huffman_tree(&root1,huffman_encode_array);
      test_traverse_print_hfmtree(root1,-1);

      /********      restore_huffman_tree test              *******************/

     return;
}

void test_traverse_print_hfmtree(HuffmanNode * huffman_tree_root, long flag){
    if(huffman_tree_root){

        if(huffman_tree_root->left_child==NULL && huffman_tree_root->right_child==NULL){
            printf("%02x: ",huffman_tree_root->data_8bit);
            HuffmanNode *cur=huffman_tree_root->parent;
            HuffmanNode *curtmp=huffman_tree_root;
            while(cur!=NULL){
                if(curtmp==cur->left_child) printf("0");
                else printf("1");
                curtmp=cur;
                cur=cur->parent;
            }printf("\n");
        }

            test_traverse_print_hfmtree(huffman_tree_root->left_child,0);

            test_traverse_print_hfmtree(huffman_tree_root->right_child,1);

    }
    return;
}


void test_uchar_weight(void){
    FILE *fp=NULL;
    open_file("zhangjun.txt",&fp);
    HuffmanNode **huffman_node_array=(HuffmanNode **)malloc(257*sizeof(HuffmanNode *));
    long size;
    make_uchar_weight(fp,huffman_node_array,&size);
    for(long i=1;i<=size;i++){
        if(huffman_node_array[i]->data_8bit_count>0){
            if(i%9==0) printf("\n");
            printf("%02x ",huffman_node_array[i]->data_8bit);
            printf("%3lld, ",huffman_node_array[i]->data_8bit_count);
        }
    }
    return;
}

void test_Jcompress(void){
    char *src_name="王牌特工.mp4";
    char *dst_name="王牌特工.mp4.jcprs";
    Jcompress(src_name);
    return;
}

void test_JparseHead(){
    char *src="APUE.pdf.jcprs";
    FILE *fp=fopen(src,"rb");
	if(fp==NULL) exit(0);
    HuffmanEncode *huffman_encode_array=(HuffmanEncode *)malloc(sizeof(HuffmanEncode)*257);
    for(long i=0;i<256;i++){
        huffman_encode_array[i].encode=NULL;
        huffman_encode_array[i].length_of_encode=0;
    }

    char *file_name=NULL;
    long last_bytes=0;

    JparseHead(&fp,huffman_encode_array,&file_name,&last_bytes);
    printf("\n");

    return;
}
void test_JUnCompress(void){
    char *src_name="王牌特工.mp4.jcprs";
    JUnCompress(src_name);
    return;
}

void test_2_file(void){
    char *src1="ms.rmvb";
    char *src2="ms1.rmvb";
    FILE *fp1=NULL;
    FILE *fp2=NULL;
    fp1=fopen(src1,"rb");
    fp2=fopen(src2,"rb");
    char *buffer1=malloc(sizeof(unsigned char)*1024*1024*10);
    char *buffer2=malloc(sizeof(unsigned char)*1024*1024*10);

    long long num1;
    long long num2;
    long long num=0;
    while((num1=fread(buffer1,sizeof(unsigned char),1024*1024*10,fp1)) &&
    (num2=fread(buffer2,sizeof(unsigned char),1024*1024*10,fp2))){
        long long temp=0;
        for(long long i=0;i<num1;i++){
            num+=1;
            if(buffer1[i]!=buffer2[i]){
                printf("%lld %x %x",num,buffer1[i],buffer2[i]);
                return;
            }
        }

    }
}
