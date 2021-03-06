#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <map>
#include <set>
#include <queue>
#include <stack>
#include <algorithm>
#include "Lexical.cpp"
using namespace std;

#define BEGIN 1
#define END 2
#define VAR 3
#define INT 4
#define WHILE 5
#define IF 6
#define THEN 7
#define ELSE 8
#define DO 9
#define EQUAL 10//赋值
#define PLUS 11
#define MUL 12
#define LP 13
#define RP 14//右括号
#define LT 15
#define LE 16
#define GT 17
#define GE 18
#define NE 19
#define EQ 20//等号
#define POINT 21//间隔符
#define COLON 22//冒号
#define SEMICOLON 23//分号
#define COMMA 24//逗号
#define ID 25//标识符
#define CONSTANT 26//常数
#define PROGRAM 27
#define MINUS 28//减号
#define DIV 29//除号
#define N 30//四元式数组大小

int tmp_id = 0;
int index ;             //当前二元式流所在的下标
int NXQ = 0;            //四元式的标号
int error_occur = 0;
struct Quaternary
{
    char res[12];
    char ag1[12];
    char ag2[12];
    char op[12];
}four[100];

int Program();
int Procedure();
int Var_table();
int Var_list();
int Statement_list();
int Statement();
int Singel_stament();
int Compound();
int Condition();
int Whiled();
int Relation();
char* Expression();
char* Term();
char* Factor();
void Gen(char* op, char* ag1, char* ag2, char* res);
char* NewTemp();


int main()
{
    int lex_res = Scanner();
    if(lex_res!=0){
        printf("----------<%d个词法错误>---------\n",lex_res);
    }
    else{
        printf("----------<词法分析成功>----------\n\n");
        int syn_res = Program();
        if(syn_res == 1){
            printf("----------<发现语法错误>----------\n\n");
        }
        else{
            printf("----------<语法分析成功>----------\n\n");
            printf("----------<输出四元式>----------\n");
            FILE * wf = fopen("Quaternary.txt","w");
            for(int i=0;i<NXQ;i++){
                fprintf(wf,"%d:  (%s , %s , %s , %s)\n",i,four[i].op,four[i].ag1,four[i].ag2,four[i].res);
                printf("%d:  (%s , %s , %s , %s)\n",i,four[i].op,four[i].ag1,four[i].ag2,four[i].res);
            }
            fclose(wf);
        }
    }
    return 0;
}

int Program(){          //<程序> --> <变量说明>BEGIN<语句表>END.
    index = 0;
    if(dual[index].dual_type == PROGRAM){
        index++;
        if (dual[index].dual_type == ID)
        {
            index++;
            if (dual[index].dual_type == SEMICOLON)
            {
                index++;
                return Procedure();
            }
            else{
                printf("line %d: 缺少 ';'\n",dual[index].x);
                return 1;
            }
        }
        else{
            printf("line %d: 缺少程序名\n",dual[index].x);
            return 1;
        }
    }
    else{
        printf("line %d: 缺少关键字 'PROGRAM'\n",dual[index].x);
        return 1;
    }
}

//<分程序>—><变量说明>BEGIN<语句表>END.
int Procedure(){            //分程序
    if(dual[index].dual_type != BEGIN && dual[index].dual_type != VAR){
        printf("line %d: 程序缺少语句段关键字 'BEGIN' 或者 'VAR' 变量定义",dual[index].x);
        return 1;
    }
    else if(dual[index].dual_type == VAR){
        index++;
         return Var_table();
    }
    else{
        index++;
        return Statement_list();
    }
}

//<变量说明>—>VAR<变量说明表>
int Var_table(){       //变量说明表
    int syn_type = Var_list();
    if(syn_type == 0){
        if(dual[index].dual_type == COLON){
            index++;
            if(dual[index].dual_type == INT){
                index++;
                if(dual[index].dual_type == SEMICOLON){
                    index++;
                    if(dual[index].dual_type == VAR){
                        index++;
                        return Var_table();
                    }
                    else if(dual[index].dual_type == BEGIN){
                        index++;
                        printf("BEGIN\n");
                        return Statement_list();
                    }
                }
                else{
                    printf("line %d: 缺少分号\n",dual[index].x);
                    return 1;
                }
            }
            else{
                printf("line %d: 变量类型必须为整型\n",dual[index].x);
                return 1;
            }
        }
        else{
            printf("line %d: 缺少冒号\n",dual[index].x);
            return 1;
        }
    }
    else{
        return 1;
    }
    return 0;
}

//<变量说明表>—><变量表>:<类型>{;<变量说明表>}
int Var_list(){        //变量表
    if(dual[index].dual_type != ID && dual[index].dual_type != SEMICOLON){
        printf("line %d: 变量定义格式错误\n");
        return 1;
    }
    else if (dual[index].dual_type == ID){
        index++;
        while (dual[index].dual_type == COMMA)
        {
            index++;
            if(dual[index].dual_type == ID){
                index++;
            }
            else{
                printf("line %d: 逗号后不是标识符\n",dual[index].x);
                return 1;
            }
        }
    }
    return 0;
}

//<语句表>—><语句>{;<语句表>}
int Statement_list(){       //语句表
    int syn_tpye = Statement();
    if(syn_tpye == 0){
        if(dual[index].dual_type == END){
            printf("END\n");
            index++;
            if(dual[index].dual_type == POINT){
                index++;
                if(index == total_size){
                    printf("----------<语法分析结束>----------\n");
                    return 0;
                }
                else{
                    printf("line %d: 程序结尾不明字符错误\n",dual[index].x);
                    return 1;
                }
            }
            else
            {
                printf("%d %s\n",index,dual[index].lexme.lexme_text);
                printf("line %d: END后缺少分隔符 '.'\n",dual[index].x);
                return 1;
            }
        }
        else{
            printf("line %d: 程序段缺少关键字 'END'\n",dual[index].x);
            return 1;
        }
    }
    else{
        return 1;
    }
}


int Statement(){            
    int syn_type = Singel_stament();
    if(syn_type==0){
        while(dual[index].dual_type == SEMICOLON){
            index++;
            int is_error = Singel_stament();
            if(is_error==1) return 1;
        }
        if(dual[index].dual_type != END){
            printf("line %d: 程序段缺少 ';'\n",dual[index].x);
            return 1;
        }
        else return 0;
    }
    else{
        return 1;
    }
}

//<语句>—><赋值语句>|<条件语句>|<WHILE语句>|<复合语句>
int Singel_stament(){       //单条语句
    char *lnum ,*rnum;      //分别保存赋值符的左右操作数

    lnum = (char*)malloc(12);
    rnum = (char*)malloc(12);

    if(dual[index].dual_type == ID){
        strcpy(lnum,dual[index].lexme.lexme_text);
        index++;
        if(dual[index].dual_type == EQUAL){
            index++;
            strcpy(rnum,Expression());
            if(error_occur == 1) return 1;      //算术表达式出错返回

            Gen(":=",rnum,"_",lnum);
            return 0;
        }
        else{
            printf("line %d: 语句错误\n",dual[index].x);
            return 1;
        }
    }
    else if(dual[index].dual_type == IF){
        index++;
        printf("IF\n");
        return Condition();
    }
    else if(dual[index].dual_type == WHILE){
        index++;
        printf("WHILE\n");
        return Whiled();
    }
    else if(dual[index].dual_type == BEGIN){
        index++;
        printf("Compound\n");
        return Compound();
    }
    else if(dual[index].dual_type == END){
        printf("line %d: 'END'前不可有 ';'\n",dual[index].x);
        return 1;
    }
    else{
        printf("line %d: 语法错误！\n",dual[index].x);
        return 1;
    }
    return 0;
}

//<复合语句>—>BEGIN<语句表>END
int Compound(){
    int syn_tpye = Statement();
    if(syn_tpye == 0){
        if(dual[index].dual_type != END){
            printf("line %d: 复合语句错误,缺少关键字 'END'\n",dual[index].x);
            return 1;
        }
        else index++;
    }
    else{
        return 1;
    }
    return 0;
}

//<条件语句>—>IF<关系表达式>THEN<语句>ELSE<语句>
int Condition(){
    char *tmp;
    int FC, tmp1,tmp2;
    tmp = (char*)malloc(12);
    FC = Relation();                //返回假出口的四元式的编号

    if(error_occur == 1) return 1;

    if(dual[index].dual_type == THEN ){
        printf("THEN\n");
        index++;
        if(Singel_stament()==1){
            return 1;
        }
        if(dual[index].dual_type == ELSE){
            Gen("jumpto","_","_","");
            tmp2 = NXQ -1;              //tmp2记录then后面的最后一条语句的四元式标号
            printf("ELSE\n");
            index++;
            tmp1 = NXQ;                 //tmp1记录假出口的跳转的四元式的标号
            // itoa(tmp1,tmp,10);
            sprintf(tmp,"%d",tmp1);
            strcpy(four[FC].res,tmp);       //回填假出口

            if(Singel_stament()==1){
                return 1;
            }
            tmp1 = NXQ;
            // itoa(tmp1, tmp, 10);
            sprintf(tmp,"%d",tmp1);
            strcpy(four[tmp2].res, tmp);        //记录当为真时的语句执行完之后应该跳转到的四元式标号
        }
        else{
            printf("line %d: 缺少关键字ELSE\n",dual[index].x);
            return 1;
        }
    }
    else{
        printf("line %d: 缺少关键字THEN\n",dual[index].x);
        return 1;
    }
    return 0;
}

//<WHILE语句>—>WHILE<关系表达式>DO<语句>
int Whiled(){
    char *tmp;
    int FC,tmp1,tmp2;
    tmp = (char*) malloc(12);
    tmp2 = NXQ;                             //循环条件的四元式标号

    FC = Relation();                        //假出口的四元式的标号
    if(error_occur == 1 ) return 1;        //关系表示式出错返回


    if(dual[index].dual_type == DO){
        printf("DO\n");
        index++;

        if(Singel_stament() == 1){         //判断语句是否有错误
            return 1;
        }

        // itoa(tmp2,tmp,10);
        sprintf(tmp,"%d",tmp2);
        Gen("jumpto","_","_",tmp);          //跳转回循环语句的条件判断处的四元式

        tmp1 = NXQ;                         //记录假出口的标号
        // itoa(tmp1,tmp,10);
        sprintf(tmp,"%d",tmp1);
        strcpy(four[FC].res,tmp);           //回填假出口
    }
    else{
        printf("line %d: 缺少关键字DO\n",dual[index].x);
        return 1;
    }
    return 0;
}

//<关系表达式>—><算术表达式><关系符><算术表达式> 
int Relation(){
    char *op, *lnum, *rnum, *TC;
    int temp,FC;
    lnum = (char*) malloc(12);
    rnum = (char*) malloc(12);
    op = (char*) malloc(12);
    TC = (char*) malloc(12);

    strcpy(op,"");
    //index++;
    strcpy(lnum,Expression());
    if(error_occur == 0){
        if(dual[index].dual_type >= LT && dual[index].dual_type <= EQ){
            int op_tpye = dual[index].dual_type;
            if(op_tpye == LT){
                strcpy(op,"<");
            }
            else if(op_tpye == LE){
                strcpy(op,"<");
                strcat(op,"=");
            }
            else if(op_tpye == GT){
                strcpy(op,">");
            }
            else if(op_tpye == GE){
                strcpy(op,">");
                strcat(op,"=");
            }
            else if(op_tpye == NE){
                strcpy(op,"<");
                strcat(op,">");
            }
            else{
                strcpy(op,"=");
            }
            index++;

            strcpy(rnum, Expression());
            if(error_occur == 1) return FC;     //算术表达式出错返回


            temp = NXQ+2;
            // itoa(temp, TC, 10);
            sprintf(TC,"%d",temp);
            Gen(op,lnum,rnum,TC);               //真出口
            FC = NXQ;
            Gen("jumpto","_","_","");           //假出口
        }
        else{
            printf("line %d: 关系比较符错误! \n",dual[index].x);
            //return 1;
        }
    }
    
    return FC;
}


//算术表达式的分析
//<算术表达式>—><项>{+ <项>| - <项>}
char* Expression(){
    char *tmp,*num1,*num2,*op;  //tmp是临时变量，num1,num2为操作数,op为操作符
    tmp = (char*)malloc(12);
    num1 = (char*)malloc(12);
    num2 = (char*)malloc(12);
    op = (char*)malloc(12);

    strcpy(num1,Term());
    if(error_occur == 1) return num1;       //项分析出错返回


    while(dual[index].dual_type == PLUS || dual[index].dual_type == MINUS){
        if(dual[index].dual_type == PLUS){
            *op = '+';
        }
        else{
            *op = '-';
        }
        *(op+1) = '\0';
        index++;

        strcpy(num2, Term());
        if(error_occur == 1) return num1;       //项分析出错返回

        strcpy(tmp,NewTemp());
        Gen(op,num1,num2,tmp);
        strcpy(num1,tmp);
    }
    return num1;
}

//<项>—><因式>{* <因式>|/<因式>}
char* Term(){
    char *tmp,*num1,*num2,*op;  //tmp是临时变量，num1,num2为操作数,op为操作符
    tmp = (char*)malloc(12);
    num1 = (char*)malloc(12);
    num2 = (char*)malloc(12);
    op = (char*)malloc(12);

    strcpy(num1,Factor());
    if(error_occur ==  1) return num1;      //因式分析出错返回


    while(dual[index].dual_type == MUL || dual[index].dual_type == DIV){
        if(dual[index].dual_type == MUL){
            *op = '*';
        }
        else{
            *op = '/';
        }
        *(op+1) = '\0';
        index++;

        strcpy(num2, Factor());
        if(error_occur ==  1) return num1;      //因式分析出错返回

        strcpy(tmp, NewTemp());
        Gen(op,num1,num2,tmp);
        strcpy(num1,tmp);
    }
    return num1;
}

//<因式>—><变量>|<常数>| <算术表达式>
char* Factor(){
    char* place;
    place = (char*)malloc(12);
    strcpy(place," ");

    if(dual[index].dual_type == ID || dual[index].dual_type == CONSTANT){
        if(dual[index].dual_type == ID){
            strcpy(place, dual[index].lexme.lexme_text);
        }
        else{
            // int sum = 0;
            // for(int i=0;i!='\0';i++){
            //     sum = sum * 10 + (dual[index].lexme.lexme_text[i]-'0');
            // }
            // itoa(sum,place,10);
            strcpy(place,dual[index].lexme.lexme_text);
        }
        index++;
    }
    else if(dual[index].dual_type == LP){
        index++;
        strcpy(place, Expression());

        if(error_occur == 1) return place;          //出错返回

        if(dual[index].dual_type == RP){
            index++;
        }
        else{
            printf("line %d: 缺少')'\n",dual[index].x);

            error_occur = 1;
        }
    }
    else{
        // printf("%d %s\n",index,dual[index].lexme.lexme_text);
        printf("line %d: 表达式格式错误！\n",dual[index].x);

        error_occur = 1;
    }
    return place;
}


void Gen(char* op, char* ag1, char* ag2, char* res){        //添加四元组
    strcpy(four[NXQ].ag1, ag1);
    strcpy(four[NXQ].ag2, ag2);
    strcpy(four[NXQ].res, res);
    strcpy(four[NXQ].op, op);
    NXQ++;
}

//语义分析中产生临时变量
char* NewTemp(){
    char *newtmp;
    char a[8];
    newtmp = (char*) malloc(8);
    tmp_id++;
    // itoa(tmp_id,a,10);
    sprintf(a,"%d",tmp_id);
    strcpy(newtmp+1,a);
    newtmp[0]='T';
    return newtmp;
}





