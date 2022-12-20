#include <stdio.h>
#include "semantics.h"

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


int main() {



    char * file_name = "C:\\Users\\Kas\\Desktop\\test.txt";
    Parser(file_name);


}
