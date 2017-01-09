#include"Jcompress.h"

int heap_min_adjust(HuffmanNode **huffman_node_array, long data_start, long data_end){
    /**
     ** check error for argument
     */
    if(huffman_node_array==NULL || data_start<0 || data_end<0 || data_end<data_start){
        printf("heap_min_adjust: argument error\n");
        exit(0);
    }

    if(data_end==data_start) return 1;
    /**
     ** the top of heap-min indicated by index data_start is the only element
     ** which need to be adjusted to make a min-heap
     */
    HuffmanNode * current_data_tobe_adjust=huffman_node_array[data_start];
    long current_indexof_data=data_start;

    for(long cur=2*data_start;cur<=data_end;cur=2*cur){
        if(cur<data_end){
            /** compare the left child and right child to find the min one */
            if(((huffman_node_array[cur])->data_8bit_count)>((huffman_node_array[cur+1])->data_8bit_count)){
                cur+=1;
            }
        }
        if((current_data_tobe_adjust->data_8bit_count)<=((huffman_node_array[cur])->data_8bit_count))
            break;
        huffman_node_array[current_indexof_data]=huffman_node_array[cur];
        current_indexof_data=cur;
    }
    huffman_node_array[current_indexof_data]=current_data_tobe_adjust;
    /**
     ** return 1 means everything is ok
     */
    return 1;
}

int heap_min_construct(HuffmanNode **huffman_node_array, long array_size){
    /** valid data start from index 1 not 0 */

    /**
     ** check error for argument
     */
     if(huffman_node_array==NULL || array_size<=0 || array_size>256){
        printf("heap_min_construct: argument error\n");
        exit(0);
     }

     for(long HeapRoot=array_size/2; HeapRoot>=1;HeapRoot--){
        heap_min_adjust(huffman_node_array,HeapRoot,array_size);
     }
     return 1;
}

/**
 ** when construct huffman tree, we need to select the 2 min element using heap_min_get2min() func.
 **/
int heap_min_get2min(HuffmanNode **huffman_node_array, HuffmanNode **min_first, HuffmanNode **min_second, long *heap_size){

    /**
     ** check argument
     **/
     if(huffman_node_array==NULL){
        printf("heap_min_get2min: argument error\n");
        exit(0);
     }

    *min_first=huffman_node_array[1];
    huffman_node_array[1]=huffman_node_array[*heap_size];
    (*heap_size)-=1;
    /** after we get the min data, we should again adjust the heap to make a min-heap */
    heap_min_adjust(huffman_node_array,1,*heap_size);

    *min_second=huffman_node_array[1];
    huffman_node_array[1]=huffman_node_array[*heap_size];
    (*heap_size)-=1;

    /** the same as above*/
	if(*heap_size>0){
		heap_min_adjust(huffman_node_array,1,*heap_size);
	}

    return 1;
}

int huffman_tree(HuffmanNode **huffman_tree_root, HuffmanNode **huffman_node_array, long array_size){
    /**
     ** check error for argument
     */
     if(huffman_node_array==NULL || array_size<1 || array_size>256){
        printf("huffman_tree:argument error\n ");
        exit(0);
     }

     if(array_size==1){
        printf("huffman_tree: \nnode size: %ld too small to construct huffman tree\ntry a bigger file\n",array_size);
        exit(0);
     }

     long heap_size=array_size;
     HuffmanNode *huffman_parent_node=(HuffmanNode *)malloc(sizeof(HuffmanNode)*(array_size-1+1));
     for(long i=1;i<=array_size-1;i++){
        (huffman_parent_node+i)->data_8bit=0x00;
        (huffman_parent_node+i)->data_8bit_count=0;
        (huffman_parent_node+i)->left_child=NULL;
        (huffman_parent_node+i)->right_child=NULL;
        (huffman_parent_node+i)->parent=NULL;
     }

     heap_min_construct(huffman_node_array,array_size);

     HuffmanNode *min1;
     HuffmanNode *min2;

     for(long i=1;i<=array_size-1;i++){
        heap_min_get2min(huffman_node_array,&min1,&min2,&heap_size);

        huffman_parent_node[i].data_8bit=0x00;
        huffman_parent_node[i].data_8bit_count=min1->data_8bit_count+min2->data_8bit_count;
        huffman_parent_node[i].left_child=min1;
        huffman_parent_node[i].right_child=min2;
        huffman_parent_node[i].parent=NULL;

        min1->parent=&(huffman_parent_node[i]);
        min2->parent=&(huffman_parent_node[i]);

        /** insert parent node and again make min heap*/
        heap_size+=1;
        huffman_node_array[heap_size]=&(huffman_parent_node[i]);
        heap_min_construct(huffman_node_array,heap_size);

     }
     if(huffman_parent_node[array_size-1].parent==NULL){

        *huffman_tree_root=&(huffman_parent_node[array_size-1]);
     }
     else{
        printf("huffman tree construct failed\n");
        exit(0);
     }
    return 1;
}

int huffman_encode(HuffmanNode **huffman_node_array, long  huffman_node_array_size,
                                                HuffmanEncode *huffman_encode_array){
    /**
     ** check error for argument
     */
    if(huffman_node_array==NULL || huffman_encode_array==NULL || huffman_node_array_size<1 ||
                                                                 huffman_node_array_size>256){
        printf("huffman_encode: argument error\n");
        exit(0);
    }

    if(huffman_node_array_size==1){
        printf("huffman_encode: node array size too small to construct huffman encode\n");
        exit(0);
    }

    HuffmanNode **huffman_node_array_backup=(HuffmanNode **)malloc((huffman_node_array_size+1)*
                                                                 sizeof(HuffmanNode *));
    if(NULL==huffman_node_array_backup){
        printf("malloc failed\n");
        exit(0);
    }

    /**
     ** later we will use this backup to make huffman encode
     ** note: backup should before invoking huffman_tree(),because this function will
     **       change array huffman_node_array
     */

    for(long i=1;i<=huffman_node_array_size;i++){
        huffman_node_array_backup[i]=huffman_node_array[i];
    }

    HuffmanNode *huffman_tree_root=NULL;
    huffman_tree(&huffman_tree_root, huffman_node_array, huffman_node_array_size);

    /**
     ** construct huffmanencode, traverse huffman tree, left encode 0, right encode 1
     */

     char encode_temp[260];
     long start_temp=259;

     for(long i=1;i<=huffman_node_array_size;i++){
        start_temp=259;

        HuffmanNode *leaf=huffman_node_array_backup[i];
        //printf("node index and count: %d , ", i);
        //printf("%d ",leaf->data_8bit_count);
        while(leaf->parent!=NULL){
            HuffmanNode *parent=leaf->parent;
            if(parent->left_child==leaf){

                encode_temp[start_temp--]='0';//printf("0");

            }else if(parent->right_child==leaf){

                encode_temp[start_temp--]='1';//printf("1");

            }else{

                printf("the huffman tree we reference seems not correct\n");
                exit(0);

            }

            leaf=leaf->parent;
        }

        /** I have make a low level error to use dat_8bit_count which cause error*/
        long huffman_encode_array_index=huffman_node_array_backup[i]->data_8bit;

        huffman_encode_array[huffman_encode_array_index].encode=(char *)malloc((259-start_temp)*sizeof(char));
        if(huffman_encode_array[huffman_encode_array_index].encode==NULL){
            printf("huffman_encode: malloc failed\n");
            exit(0);
        }

        huffman_encode_array[huffman_encode_array_index].length_of_encode=259-start_temp;
        memcpy(huffman_encode_array[huffman_encode_array_index].encode, encode_temp+start_temp+1, 259-start_temp);

     }

     if(huffman_node_array_backup!=NULL){
        free(huffman_node_array_backup);
        huffman_node_array_backup=NULL;
     }

     return 1;
}

int huffman_decode_internal(HuffmanNode **huffman_tree_current_root, long direction){
    /**
     ** check error for argument
     */
    if(huffman_tree_current_root==NULL || *huffman_tree_current_root==NULL || !(direction==0 || direction==1)){
        printf("huffman_decode_internal: argument error\n");
        exit(0);
    }

    if(direction==0){
        (*huffman_tree_current_root)=(*huffman_tree_current_root)->left_child;
        if((*huffman_tree_current_root)->left_child==NULL &&
           (*huffman_tree_current_root)->right_child==NULL){
                return 1;
        }else{
            return 0;
        }
    }else if(direction==1){
        (*huffman_tree_current_root)=(*huffman_tree_current_root)->right_child;
        if((*huffman_tree_current_root)->left_child==NULL &&
           (*huffman_tree_current_root)->right_child==NULL){
                return 1;
        }else{
            return 0;
        }
    }else{
        printf("\ndirection error: %ld\n",direction);
        exit(0);
    }

}

int huffman_decode(HuffmanNode *huffman_tree_root, HuffmanNode **last_stop_at, EncodeBuffer *encode_buffer,
                                                                                 DecodeBuffer *decode_buffer){

    /**
     ** check error for argument
     */
     if(NULL==last_stop_at || encode_buffer==NULL || decode_buffer==NULL){
        printf("huffman_decode: argument error\n");
        exit(0);
     }

     long  not_full_byte=0;
     long  bits_num_ofLastByte=0;
     if(encode_buffer->bits_num_lastbytes>0){
        not_full_byte=1;
        bits_num_ofLastByte=encode_buffer->bits_num_lastbytes;
     }

     if(*last_stop_at==NULL){
        *last_stop_at=huffman_tree_root;
     }

     if(decode_buffer->size!=0){
        decode_buffer->size=0;
     }

     long buffer_size_real=0;
     buffer_size_real=encode_buffer->size;

     unsigned char * ptr_current_buffer=encode_buffer->buffer;
     unsigned char * ptr_endOf_real_buffer=encode_buffer->buffer+buffer_size_real-1;
     long return_value=-1;
     for(;ptr_current_buffer<=ptr_endOf_real_buffer;ptr_current_buffer++){

        unsigned char temp=*ptr_current_buffer;
        for(long i=0;i<8;i++){
            unsigned char bit_for_test=0x80;
            bit_for_test=bit_for_test>>i;
            long direction;
            if(bit_for_test&temp){ direction=1;}
            else{ direction=0;}
            return_value=huffman_decode_internal(last_stop_at,direction);
            if(return_value==1){
                decode_buffer->buffer[decode_buffer->size]=(*last_stop_at)->data_8bit;
                (decode_buffer->size)+=1;

                (*last_stop_at)=huffman_tree_root;
            }
        }

     }

     if(not_full_byte==1){
        unsigned char temp=(encode_buffer->buffer[encode_buffer->size]);
        for(long i=0;i<bits_num_ofLastByte;i++){
            unsigned char bit_for_test=0x80;
            bit_for_test=bit_for_test>>i;
            long direction;
            if(bit_for_test&temp){direction=1;}
            else{direction=0;}
            return_value=huffman_decode_internal(last_stop_at,direction);
            if(return_value==1){
                decode_buffer->buffer[decode_buffer->size]=(*last_stop_at)->data_8bit;
                (decode_buffer->size)+=1;

                (*last_stop_at)=huffman_tree_root;
            }
        }
     }
    return 1;
}

int bit_set(unsigned char *ch, long index, long flag){
    if(flag==1){
        (*ch)|=(0x80>>index);
    }else if(flag==0){
        (*ch)&=~(0x80>>index);
    }else{
        printf("bit_set: flag not correct\n");
        exit(0);
    }

    return 1;
}

int open_file(char *filename, FILE **fp){

    /**
     ** check argument
     **/
    if(filename==NULL || fp==NULL){
        printf("open_file: argument error\n");
        exit(0);
     }

    if(access(filename,0)!=0){
        printf("open_file: file %s not exist\n",filename);
        exit(0);
    }

    (*fp)=fopen(filename,"rb");
    if(NULL==(*fp)){
        printf("open file %s error\n",filename);
        exit(0);
    }
    return 0;
}
int create_Uncompress_file(char *filename, FILE **fp){
    /**
     ** check argument error
     **/
    if(filename==NULL || fp==NULL){
        printf("create_Uncompress_file: argument error\n");
        exit(0);
     }

    if(access(filename,0)==0){
        long temp=strlen(filename);
        char *filename_new=(char *)malloc(sizeof(char)*(temp+2));
        memcpy(filename_new,filename,temp);
        long j=0;
        long k=0;
        for(long i=0;i<temp;i++){
            if(filename[i]=='.' && k==0){
                k++;
                filename_new[j++]='1';
                filename_new[j++]='.';
            }else{
                filename_new[j++]=filename[i];
            }
        }
        filename_new[j]='\0';
        filename=filename_new;
    }
    if((*fp=fopen(filename,"wb"))==NULL)
    {
        printf("create_Uncompress_file:create file failed!\n");
        exit(0);
    }

    return 1;
}

int creat_file(char *filename, FILE **fp){
    /**
     ** check argument
     **/
    if(filename==NULL){
        printf("creat_file: argument error\n");
        exit(0);
    }

    if(access(filename,0)==0){
        if(remove(filename)!=0){
            printf("error occur when delete file\n");
            exit(0);
        }
    }
    if((*fp=fopen(filename,"wb"))==NULL)
    {
        printf("can not open file!\n");
        exit(0);
    }
    return 1;
}

int make_uchar_weight(FILE *fp, HuffmanNode **huffman_node, long *size){

    /**
     ** check error for argument
     */
    if(fp==NULL || huffman_node==NULL || size==NULL){
        printf("make_uchar_weight: argument error\n");
        exit(0);
    }

    *size=0;
    const long buffer_size=40*1024*1024;
    unsigned char *buffer=(unsigned char *)malloc(buffer_size*sizeof(unsigned char));
    if(buffer==NULL){
        printf("make_uchar_weight: malloc failed\n");
        exit(0);
    }

    HuffmanNode *huffman_node_array0=(HuffmanNode *)malloc(257*sizeof(HuffmanNode));
    if(huffman_node_array0==NULL){
        printf("make_uchar_weight: malloc failed\n");
        exit(0);
    }

    for(long i=0;i<257;i++){
        huffman_node_array0[i].data_8bit=0x00;
        huffman_node_array0[i].data_8bit_count=0;
        huffman_node_array0[i].parent=NULL;
        huffman_node_array0[i].left_child=NULL;
        huffman_node_array0[i].right_child=NULL;
    }

    long read_bytes=0;
    while((read_bytes=fread(buffer,sizeof(unsigned char),buffer_size,fp))!=0){
        unsigned char *ptr=buffer;
        unsigned char *ptr_end=buffer+read_bytes;
        for(;ptr!=ptr_end;ptr++){
            huffman_node_array0[(long)(*ptr)].data_8bit=*ptr;
            huffman_node_array0[(long)(*ptr)].data_8bit_count+=1;
        }
    }

    for(long i=0;i<257;i++){
        if(huffman_node_array0[i].data_8bit_count>0){
            (*size)++;
            huffman_node[*size]=&(huffman_node_array0[i]);
        }
    }

    if(buffer!=NULL){
        free(buffer);
        buffer=NULL;
    }

    return 1;

}

int Jcompress(char *src_name){
    /**
     ** check argument
     **/
    if(src_name==NULL){
        printf("Jcompress: argument error\n");
        exit(0);
    }
    printf("******************Jcompress begin to execute************\n");
    printf("source file name : %s\n",src_name);
    char *dst_name=(char *)malloc(strlen(src_name)+6+1);
    memcpy(dst_name,src_name,strlen(src_name));
    char *cur0=dst_name+strlen(src_name);
    memcpy(cur0,".jcprs",6);
    *(cur0+6)='\0';
    printf("compress file name: %s\n",dst_name);
    FILE *src_fp=NULL;
    FILE *dst_fp=NULL;
    open_file(src_name,&src_fp);
    creat_file(dst_name,&dst_fp);

    HuffmanNode **huffman_node_array=(HuffmanNode **)malloc(257*sizeof(HuffmanNode *));
    if(huffman_node_array==NULL){
        printf("Jcompress: malloc failed\n");
        exit(0);
    }
    long huffman_node_num=0;
    make_uchar_weight(src_fp,huffman_node_array,&huffman_node_num);

    HuffmanEncode *encode_array=(HuffmanEncode *)malloc(257*sizeof(HuffmanEncode));
    if(encode_array==NULL){
        printf("Jcompress: malloc failed\n");
        exit(0);
    }
    for(long i=0;i<257;i++){
        encode_array[i].encode=NULL;
        encode_array[i].length_of_encode=0;
    }
    huffman_encode(huffman_node_array, huffman_node_num, encode_array);

    /**
     ** begin to deal with the header of the compress file, later the uncompress routine
     ** can read the header and restore the huffman tree then uncompress the file
     */


    CompressFileHeader CFH;
    CFH.length_of_author_name=8;
    CFH.author_name="zhangjun";
    CFH.routine_name_length=9;
    CFH.routine_name="Jcompress";
    CFH.suffix_length=5;
    CFH.suffix="jcprs";
    CFH.file_name_length=strlen(src_name);
    CFH.bits_of_lastByte=0x00;

    long length_of_header1=0;
    long length_of_header2=0;
    char *cur=src_name;
    while(cur[length_of_header1++]!='\0');
    length_of_header1+=2*sizeof(long)+9+10+6+1;
    length_of_header2=length_of_header1;

    for(long i=0;i<256;i++){
        if(encode_array[i].length_of_encode==0){
            length_of_header2=length_of_header2+1;
        }
        else if(encode_array[i].length_of_encode>0){
            length_of_header2=length_of_header2+1+
                                        encode_array[i].length_of_encode;
        }
    }

    unsigned char *buffer_for_header=(unsigned char*)malloc(sizeof(unsigned char)*length_of_header2);

    if(buffer_for_header==NULL){
        printf("Jcompress:malloc failed for buffer_for_header\n");
        exit(0);
    }

    unsigned char *ptr=buffer_for_header;
    *((long *)ptr)=length_of_header1;
    *((long *)(ptr+sizeof(long)))=length_of_header2;
    ptr+=2*sizeof(long);
    *(ptr++)=(unsigned char)CFH.length_of_author_name;
    *(ptr++)=(unsigned char)CFH.routine_name_length;
    *(ptr++)=(unsigned char)CFH.suffix_length;
    *(ptr++)=(unsigned char)CFH.file_name_length;
    *(ptr++)=(unsigned char)CFH.bits_of_lastByte;

    char *ch="zhangjunJcompressjcprs";
    memcpy(ptr,ch,22);
    ptr+=22;
    memcpy(ptr,src_name,CFH.file_name_length);
    ptr+=CFH.file_name_length;

    for(long i=0;i<256;i++){
        if(encode_array[i].length_of_encode==0){
            *ptr=0x00;
             ptr+=1;
        }else{
            *ptr=encode_array[i].length_of_encode;
             ptr++;
             memcpy(ptr,encode_array[i].encode,encode_array[i].length_of_encode);
             ptr+=encode_array[i].length_of_encode;
        }
    }

    long temp=fwrite(buffer_for_header,length_of_header2,1,dst_fp);
    if(temp!=1){
        printf("Jcompress: fwrite for compress file header error\n");
        exit(0);
    }

    EncodeBuffer *encode_buffer=(EncodeBuffer *)malloc(sizeof(EncodeBuffer));
    if(encode_buffer==NULL){
        printf("Jcompress: malloc failed\n");
        exit(0);
    }

    encode_buffer->bits_num_lastbytes=0;
    encode_buffer->buffer=NULL;
    encode_buffer->size=0;
    const long fread_buffer_size=4*1024*1024;
    const long encode_buffer_size=32*fread_buffer_size;
    encode_buffer->buffer=(unsigned char *)malloc(encode_buffer_size*sizeof(unsigned char));
    if(encode_buffer->buffer==NULL){
        printf("Jcompress: malloc failed\n");
        exit(0);
    }

    unsigned char * fread_buffer=(unsigned char *)malloc(fread_buffer_size*sizeof(unsigned char));
    if(fread_buffer==NULL){
        printf("Jcompress: malloc failed\n");
        exit(0);
    }



    long read_bytes=0;
    fseek(src_fp,0L, SEEK_SET);
	/**I have made an error define num_ofbits_write as long which cause bigger file compress wrongly*/
    long long num_ofbits_write=-1;
    while((read_bytes=fread(fread_buffer,sizeof(unsigned char),fread_buffer_size,src_fp))!=0){
        unsigned char *ptr=fread_buffer;
        unsigned char *ptr_end=fread_buffer+read_bytes;
        long num_ofbits_write_per_while=-1;
        if(num_ofbits_write>=0){
            num_ofbits_write_per_while+=(num_ofbits_write+1)%8;
        }
        for(;ptr!=ptr_end;ptr++){
            long index=(long)(*ptr);
            if(encode_array[index].length_of_encode<=0){
                printf("Jcompress : encode_array not correct\n");
                exit(0);
            }

            for(long i=0;i<encode_array[index].length_of_encode;i++){
                num_ofbits_write++;
                num_ofbits_write_per_while++;
                long byte_index=num_ofbits_write_per_while/8;
                long bit_index=num_ofbits_write%8;
                long flag=(long)(encode_array[index].encode[i]-'0');
                bit_set((encode_buffer->buffer)+byte_index, bit_index, flag);

            }
        }

        encode_buffer->size=((num_ofbits_write_per_while+1)/8);
        encode_buffer->bits_num_lastbytes=((num_ofbits_write+1)%8);
        long fwrite_size=encode_buffer->size;
        long real_write_num=fwrite(encode_buffer->buffer,sizeof(unsigned char),fwrite_size,dst_fp);
        if(real_write_num!=fwrite_size){
            printf("Jcompress : fwrite error\n");
            exit(0);
        }
        if(encode_buffer->bits_num_lastbytes>0){
            *(encode_buffer->buffer)=*(encode_buffer->buffer+encode_buffer->size);
        }


    }

    if((num_ofbits_write+1)%8!=0){
        long temp=fwrite(encode_buffer->buffer,sizeof(unsigned char),1,dst_fp);
        if(temp!=1){
            printf("Jcompress: fwrite error\n");
            exit(0);
        }
        CFH.bits_of_lastByte=(num_ofbits_write+1)%8;
    }

    /**
     ** now set the correct bits_of_lastByte of the compress file header
     */


    *(buffer_for_header+2*sizeof(long)+4)=CFH.bits_of_lastByte;
    fseek(dst_fp,0L, SEEK_SET);
    long temp0=fwrite(buffer_for_header,1,2*sizeof(long)+5,dst_fp);
    if(temp0!=(2*sizeof(long)+5)){
        printf("Jcompress: fwrite error\n");
        exit(0);
    }

    if(dst_name!=NULL){
        free(dst_name);
        dst_name=NULL;
    }

    if(huffman_node_array!=NULL){
        free(huffman_node_array);
        huffman_node_array=NULL;
    }

    for(int i=0;i<257;i++){
        if(encode_array[i].encode!=NULL){
            free(encode_array[i].encode);
            encode_array[i].encode=NULL;
        }
    }

    if(encode_array!=NULL){
        free(encode_array);
        encode_array=NULL;
    }
    if(buffer_for_header!=NULL){
        free(buffer_for_header);
        buffer_for_header=NULL;
    }


    if(encode_buffer->buffer!=NULL){
        free(encode_buffer->buffer);
        encode_buffer->buffer=NULL;
    }
    if(encode_buffer!=NULL){
        free(encode_buffer);
        encode_buffer=NULL;
    }
    if(fread_buffer!=NULL){
        free(fread_buffer);
        fread_buffer=NULL;
    }

    if(fclose(src_fp)!=0){
        printf("Jcompress: fclose src_fp error\n");
    }
    if(fclose(dst_fp)!=0){
        printf("Jcompress: fclose src_fp error\n");
    }

    printf("Jcompress: succeed\n");

    return 1;
}

int restore_huffman_tree(HuffmanNode **huffman_tree_root, HuffmanEncode *huffman_encode_array){
    /**
     ** check error for argument
     */
    if(huffman_tree_root==NULL || huffman_encode_array==NULL){
        printf("restore_huffman_tree: argument error\n");
        exit(0);
    }

     long valid_encode_num=0;
     for(long i=0;i<256;i++){
        if(huffman_encode_array[i].length_of_encode>0){
                valid_encode_num+=1;
        }
     }

     HuffmanNode *huffman_nodes=(HuffmanNode *)malloc(sizeof(HuffmanNode)*(valid_encode_num*2-1));

     if(huffman_nodes==NULL){
        printf("\nrestore_huffman_tree: malloc failed\n");
        exit(0);
     }


     for(long i=0;i<valid_encode_num*2-1;i++){
        huffman_nodes[i].left_child=NULL;
        huffman_nodes[i].right_child=NULL;
        huffman_nodes[i].parent=NULL;
     }


     long huffman_nodes_index=0;
     (*huffman_tree_root)=&(huffman_nodes[huffman_nodes_index++]);

     for(long i=0;i<256;i++){
        if(huffman_encode_array[i].length_of_encode>0){
                HuffmanNode *current=(*huffman_tree_root);
            for(long j=0;j<huffman_encode_array[i].length_of_encode;j++){
                if(huffman_encode_array[i].encode[j]=='0'){

                    if(current->left_child!=NULL) current=current->left_child;
                    else{
                        current->left_child=&(huffman_nodes[huffman_nodes_index++]);
                        current->left_child->parent=current;
                        current=current->left_child;
                    }

                }else if(huffman_encode_array[i].encode[j]=='1'){

                    if(current->right_child!=NULL) current=current->right_child;
                    else{
                        current->right_child=&(huffman_nodes[huffman_nodes_index++]);
                        current->right_child->parent=current;
                        current=current->right_child;
                    }

                }else{
                    printf("restore_huffman_tree: encode array error\n");
                    exit(0);
                }
            }
            current->data_8bit=(unsigned char)i;

        }
     }
     return 1;

}

int JparseHead(FILE **fp, HuffmanEncode *huffman_encode_array, char **file_name, long *last_bits){
    /**
     ** check error for argument
     */
    if(fp==NULL || *fp==NULL || huffman_encode_array==NULL || file_name==NULL || last_bits==NULL){
        printf("JparseHead: argument error\n");
        exit(0);
    }
    long sizeof_header1=0;
    long sizeof_header2=0;

    unsigned char *buffer=(unsigned char* )malloc(sizeof(long)*2);
    long read_num=fread(buffer,sizeof(long),2,*fp);
    if(read_num!=2){
        printf("JparseHead: read first two long of header failed\n");
        exit(0);
    }
    sizeof_header1=*((long *)buffer);
    sizeof_header2=*((long *)(buffer+sizeof(long)));

    if(buffer!=NULL){
        free(buffer);
        buffer=NULL;
    }
    /** I have make a silly error to use sizeof(sizeof_header2) which take lots of time to debug*/
    unsigned char *buffer_header=(unsigned char *)malloc(sizeof(unsigned char)*sizeof_header2);

    if(buffer_header==NULL){
        printf("JparseHead: malloc for buffer_header failed\n");
        exit(0);
    }

    fseek(*fp,0L, SEEK_SET);

    read_num=fread(buffer_header,sizeof(unsigned char),sizeof_header2,*fp);

    if(read_num!=sizeof_header2){
        printf("JparseHead: fread failed\n");
        exit(0);
    }

    unsigned char * ptr_header1=buffer_header;
    unsigned char * ptr_header1_end=buffer_header+sizeof_header1;

    unsigned char * ptr_header2=buffer_header+sizeof_header1;
    unsigned char * ptr_header2_end=buffer_header+sizeof_header2;

    ptr_header1+=sizeof(long)*2;

    long length_of_author_name=*(ptr_header1);
    long length_of_routine_name=*(ptr_header1+1);
    long length_of_suffix=*(ptr_header1+2);
    long length_of_file_name=*(ptr_header1+3);
    long length_of_lastbits=*(ptr_header1+4);
    ptr_header1+=5;

    *last_bits=length_of_lastbits;

    printf("\nJcompress author: ");
    unsigned char *ptr=NULL;
    long i=0;
    for(i=1,ptr=ptr_header1;i<=length_of_author_name;ptr++,i++){
        printf("%c",*ptr);
    }
    printf("\ncompress routine is: ");
    for(long i=1;i<=length_of_routine_name;ptr++,i++){
        printf("%c",*ptr);
    }
    printf("\ncompress suffix is: ");
    for(long i=1;i<=length_of_suffix;ptr++,i++){
        printf("%c",*ptr);
    }

    (*file_name)=(char *)malloc(sizeof(char)*(length_of_file_name+1));
    if(file_name==NULL){
        printf("JparseHeader: malloc failed\n");
        exit(0);
    }
    memcpy((*file_name),ptr,length_of_file_name);
    (*file_name)[length_of_file_name]='\0';

    printf("\noriginal file name is: ");
    for(long i=1;i<=length_of_file_name;ptr++,i++){
        printf("%c",*ptr);
    }
    printf("\n");
    long index=-1;
    unsigned char *cur=ptr_header2;
    while(cur<ptr_header2_end){
        index++;
        if(*cur==0x00){
            cur++;
        }else{
            huffman_encode_array[index].encode=(char *)malloc((*cur)*sizeof(char));
            if(huffman_encode_array[index].encode==NULL){
                printf("JparseHead: malloc failed\n");
                exit(0);
            }
            huffman_encode_array[index].length_of_encode=*cur;
            memcpy(huffman_encode_array[index].encode,cur+1,*cur);
            cur=cur+1+huffman_encode_array[index].length_of_encode;

        }

    }

    if(buffer_header!=NULL){
        free(buffer_header);
        buffer_header=NULL;
    }

    return 1;
}

int JUnCompress(char *src_name){
    /**
     ** check error for argument
     */

    if(access(src_name,0)!=0){
        printf("file not exist\n");
        exit(0);
    }
    printf("\n******************UnCompress file begin******************\n");
    printf("compress file name is: %s\n",src_name);
    FILE *ptr_src=NULL;
    open_file(src_name,&ptr_src);
    long compress_file_size=0;
    fseek(ptr_src,0L,SEEK_END);
    compress_file_size=ftell(ptr_src);
    if(compress_file_size==-1){
        printf("compress file too big to display its size(bigger than 2.1G)\nbut Jcompress will run normally\n");
    }else{
        printf("compress file size: %ld bytes\n",compress_file_size);
    }
    fseek(ptr_src,0L, SEEK_SET);

    long last_bits=0;
    char *file_name=NULL;
    HuffmanEncode *huffman_encode_array=(HuffmanEncode *)malloc(sizeof(HuffmanEncode)*256);

    if(huffman_encode_array==NULL){
        printf("JUnCompress: malloc huffman_encode_array failed\n");
        exit(0);
    }

    for(int i=0;i<256;i++){
        huffman_encode_array[i].encode=NULL;
        huffman_encode_array[i].length_of_encode=0;
    }

    JparseHead(&ptr_src,huffman_encode_array,&file_name,&last_bits);
    HuffmanNode *huffman_tree_root=NULL;
    restore_huffman_tree(&huffman_tree_root,huffman_encode_array);

    //test_traverse_prlong long_hfmtree(huffman_tree_root,-1);
    FILE *ptr_dst=NULL;
    create_Uncompress_file(file_name,&ptr_dst);

    EncodeBuffer *encode_buffer=(EncodeBuffer *)malloc(sizeof(EncodeBuffer));
    DecodeBuffer *decode_buffer=(DecodeBuffer *)malloc(sizeof(DecodeBuffer));
    if(encode_buffer==NULL || decode_buffer==NULL){
        printf("JUnCompress: encode/decode buffer malloc failed\n");
        exit(0);
    }

    encode_buffer->bits_num_lastbytes=0;
    encode_buffer->buffer=NULL;
    encode_buffer->size=0;

    const long enbuffer_size=8*1024*1024;
    const long debuffer_size=10*enbuffer_size;
    encode_buffer->buffer=(unsigned char *)malloc(enbuffer_size*sizeof(unsigned char));
    if(encode_buffer->buffer==NULL){
        printf("JUnCompress: EncodeBuffer->buffer malloc failed\n");
        exit(0);
    }

    decode_buffer->size=0;
    decode_buffer->buffer=(unsigned char *)malloc(debuffer_size*sizeof(unsigned char));
    if(decode_buffer->buffer==NULL){
        printf("JUnCompress: EncodeBuffer->buffer malloc failed\n");
        exit(0);
    }

    long long read_compress_file_size=0;
    long read_bytes=0;
    HuffmanNode *last_stop_at=NULL;
    while((read_bytes=fread(encode_buffer->buffer,sizeof(unsigned char),enbuffer_size,ptr_src))!=0){
        read_compress_file_size+=read_bytes;
        if(read_compress_file_size==compress_file_size){
            if(last_bits>0){
                encode_buffer->bits_num_lastbytes=last_bits;
                encode_buffer->size=read_bytes-1;
            }else{
                encode_buffer->bits_num_lastbytes=0;
                encode_buffer->size=read_bytes;
            }
        }else{
            encode_buffer->bits_num_lastbytes=0;
            encode_buffer->size=read_bytes;
        }

        huffman_decode(huffman_tree_root,&last_stop_at,encode_buffer,decode_buffer);
        long write_bytes=fwrite(decode_buffer->buffer,sizeof(unsigned char),decode_buffer->size,ptr_dst);
        if(write_bytes!=decode_buffer->size){
            printf("JUnCompress: fwrite failed\n");
            exit(0);
        }

    }

    printf("UnCompress file succeed\n");
    if(huffman_encode_array!=NULL){
        free(huffman_encode_array);
        huffman_encode_array=NULL;
    }

    if(encode_buffer->buffer!=NULL){
        free(encode_buffer->buffer);
        encode_buffer->buffer=NULL;
    }
    if(encode_buffer!=NULL){
        free(encode_buffer);
        encode_buffer=NULL;
    }
    if(decode_buffer->buffer!=NULL){
        free(decode_buffer->buffer);
        decode_buffer->buffer=NULL;
    }
    if(decode_buffer!=NULL){
        free(decode_buffer);
        decode_buffer=NULL;
    }

	if(fclose(ptr_src)!=0){
		printf("JUnCompress: fclose error\n");
		exit(0);
	}

    return 1;
}
