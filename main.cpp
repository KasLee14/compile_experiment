#include <stdio.h>
#include "semantics.h"

//����ļ��Ƿ���������
int CheckSrcFile(const char * file_name){

    FILE  * file = NULL;
    if(file_name == NULL || file_name[0] == '\0'){
        fprintf(stderr, "δָ��Դ�ļ�");
        return 0;
    }
    if((file = fopen(file_name, "r")) == NULL){
        fprintf(stderr, "�ļ���ʧ��");
    }
    else fclose(file);
    return 1;
}


int main(int argc, char*argv[]) {

//    if(argc < 2) printf("Cannot detect file");
//    if(CheckSrcFile(argv[1]) == 0) return 1;

    char * file_name = "C:\\Users\\Kas\\Desktop\\test.txt";
    Parser(file_name);


}
