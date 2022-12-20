#include <stdio.h>
#include "parse.h"

//检查文件是否能正常打开
int CheckSrcFile(const char * file_name){

    FILE  * file = NULL;
    if(file_name == NULL || file_name[0] == '\0'){
        fprintf(stderr, "未指定源文件");
        return 0;
    }
    if((file = fopen(file_name, "r")) == NULL){
        fprintf(stderr, "文件打开失败");
    }
    else fclose(file);
    return 1;
}


int main(int argc, char*argv[]) {

    if(argc < 2) printf("Cannot detect file");
    if(CheckSrcFile(argv[1]) == 0) return 1;

    Parser(argv[1]);


}
