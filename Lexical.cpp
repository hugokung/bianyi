//#include <bits/stdc++.h>
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

#define ERROR_WORD -8
#define MISMATCH -7
#define OUT_OF_RANGE -6
#define OUT_OF_LIMIT_LEN -5
#define UNDEFINE_VAR -4


char cache[1000][100];
char program[1000];
map<string,int> key_word;
map<string,int> var_table;
int error_num;
int V_flag = -1;
int total_size;
struct DUAL
{
    int dual_type;
    union
    {
        char lexme_text[10];
        int lexme_num;
    }lexme;
    int x,y;
}dual[1000];

void init()
{
    key_word["BEGIN"]=1;key_word["begin"]=1;
    key_word["END"]=2;key_word["end"]=2;
    key_word["VAR"]=3;key_word["var"]=3;
    key_word["INTEGER"]=4;key_word["integer"]=4;
    key_word["WHILE"]=5;key_word["while"]=5;
    key_word["IF"]=6;key_word["if"]=6;
    key_word["THEN"]=7;key_word["then"]=7;
    key_word["ELSE"]=8;key_word["else"]=8;
    key_word["DO"]=9;key_word["do"]=9;
}
bool isDelimiter(char s)
{
    if(s==';'||s==','||s=='+'||s=='-'||s=='('||s==')'||s=='{'||s=='}'||s=='*'||s=='/'||s=='<'||s=='>'||s==':'||s=='.') return true;
    else return false;
}
int Scanner()
{
    init();
    error_num=0;
    int pos = 0;
    int row=1,col=1;
    FILE * msg = fopen("message.txt","w");
    FILE * fp = fopen("data.txt","r");
    if(fp==NULL){
        printf("failed to open file");
        return -1;
    }
    while (!feof(fp))
    {
        char now  = getc(fp);
        if(now == EOF) break;
        bool flag=0;
        while (now == '\n')
        {
            flag = 1;
            row++;
            col=1;
            now = getc(fp);
        }
        while (now == '\t'||now == ' ')
        {
            flag = 1;
            if(now=='\t') col = col + 4;
            else col++;
            now = getc(fp);
        }
        if(isalpha(now)){                               //开头是字母则是标识符或者保留字
            if(col!=1&&row!=1&&flag==0) col++; 
            dual[pos].x=row;dual[pos].y=col;
            dual[pos].lexme.lexme_text[0]=now;
            int index = 1;
            dual[pos].dual_type = ID;
            if (islower(now))
            {
                dual[pos].dual_type = ERROR_WORD;
            }
            now = getc(fp);
            col++;
            int word_len = 1;
            while (now != ' '&&now != '\t'&&(!isDelimiter(now))&&now != '\n')
            {
                dual[pos].lexme.lexme_text[index++] = now;
                word_len++;
                if(ispunct(now)){
                    dual[pos].dual_type = ERROR_WORD;
                }
                now = getc(fp);
                col++;
            }
            if (now=='\n')
            {
                row++;col=1;
            }
            if(now == '\t') col+=4;
            if(now == ' ') col++;
            if(isDelimiter(now)) fseek(fp,-1,1);
            if(dual[pos].dual_type != ERROR_WORD){          //没有错误，先赋值为标识符
                dual[pos].dual_type = ID;
                if(strcmp(dual[pos].lexme.lexme_text,"PROGRAM")==0||strcmp(dual[pos].lexme.lexme_text,"program")==0){
                    dual[pos].dual_type = PROGRAM;
                }
            }
            string tmp = dual[pos].lexme.lexme_text;
            if(key_word[tmp]!=0){                           //判断是否是保留字
                dual[pos].dual_type = key_word[tmp];
            }
            if(word_len > 8){                               //判断是否长度超过规定
                dual[pos].dual_type = OUT_OF_LIMIT_LEN;
            }
            dual[pos].lexme.lexme_text[index] = '\0';
            int word_type = dual[pos].dual_type;
            if(word_type == VAR) V_flag = 1;
            if(word_type == BEGIN) V_flag = 0;
            if (word_type == ID && V_flag == 1)             //判断变量是否定义过
            {
                string tmp = dual[pos].lexme.lexme_text;
                var_table[tmp] = 1;
            }
            else if(word_type == ID && V_flag == 0){
                string tmp = dual[pos].lexme.lexme_text;
                if(var_table[tmp]==0) dual[pos].dual_type = UNDEFINE_VAR;
            }
            
            switch (word_type)                              //输出分析信息
            {
            case ERROR_WORD:
                printf("%s not a legal name. At %d line %d column\n",dual[pos].lexme.lexme_text,dual[pos].x,dual[pos].y);
                fprintf(msg,"%s not a legal name. At %d line %d column\n",dual[pos].lexme.lexme_text,dual[pos].x,dual[pos].y);
                error_num++;
                break;
            case OUT_OF_LIMIT_LEN:
                error_num++;
                printf("%s is out of the maximun length. At %d line %d column\n",dual[pos].lexme.lexme_text,dual[pos].x,dual[pos].y);
                fprintf(msg,"%s is out of the maximun length. At %d line %d column\n",dual[pos].lexme.lexme_text,dual[pos].x,dual[pos].y);
                break;
            case UNDEFINE_VAR:
                error_num++;
                printf("%s is undefine, at %d line %d column\n",dual[pos].lexme.lexme_text,dual[pos].x,dual[pos].y);
                fprintf(msg,"%s is undefine, at %d line %d column\n",dual[pos].lexme.lexme_text,dual[pos].x,dual[pos].y);
                break;
            default:
                printf("(%s , %d),at %d line %d column\n",dual[pos].lexme.lexme_text, dual[pos].dual_type, dual[pos].x,dual[pos].y);
                fprintf(msg,"(%s , %d),at %d line %d column\n",dual[pos].lexme.lexme_text,dual[pos].dual_type,dual[pos].x,dual[pos].y);
                break;
            }
            pos++;                                          //位置加1，保存下一个字符串
        }
        else if (now<='9'&&now>='0')                              //开头是数字，则是整数
        {
            if(col!=1&&row!=1&&flag==0) col++;
            int sum = 0;
            dual[pos].dual_type = CONSTANT;
            dual[pos].lexme.lexme_text[0] = now;
            dual[pos].x = row;dual[pos].y = col;
            int index = 1;
            now = getc(fp);
            col++;
            while (now!=' '&&now!='\t'&&(!isDelimiter(now))&&now!='\n')      //读入并拼接字符
            {
                if(isalpha(now)){
                    dual[pos].lexme.lexme_text[index++] = now;
                    dual[pos].dual_type = ERROR_WORD;
                }
                else {
                    dual[pos].lexme.lexme_text[index++] = now;
                }
                now = getc(fp);
                col++;
            }
            if(now=='\n'){
                row++;col=1;
            }
            if(now == '\t') col+=4;
            if(now == ' ') col++;
            if(isDelimiter(now)) fseek(fp,-1,1),col--;

            dual[pos].lexme.lexme_text[index] = '\0';

            if(dual[pos].dual_type != ERROR_WORD){              //如果数字的格式没有错误，则计算它的值
                for(int i=0;i<index;i++){
                    sum = sum*10+(dual[pos].lexme.lexme_text[i]-'0');
                }
                if(sum>65536){                                  //判断是否超出范围
                    dual[pos].dual_type = OUT_OF_RANGE;
                }
                else
                {
                    //dual[pos].lexme.lexme_num = sum;
                }
            }
            int  word_type = dual[pos].dual_type;
            switch (word_type)                                  //输出分析信息
            {
            case ERROR_WORD:
                error_num++;
                fprintf(msg,"%s is illegal number,at %d line %d column\n",dual[pos].lexme.lexme_text,dual[pos].x,dual[pos].y);
                printf("%s is illegal number,at %d line %d column\n",dual[pos].lexme.lexme_text,dual[pos].x,dual[pos].y);
                break;
            case OUT_OF_RANGE:
                error_num++;
                fprintf(msg,"%s is out of range, at %d line %d column\n",dual[pos].lexme.lexme_text,dual[pos].x,dual[pos].y);
                printf("%s is out of range, at %d line %d column\n",dual[pos].lexme.lexme_text,dual[pos].x,dual[pos].y);
                break;
            default:
                fprintf(msg,"(%s , %d),at %d line %d column\n",dual[pos].lexme.lexme_text,dual[pos].dual_type,dual[pos].x,dual[pos].y);
                printf("(%s , %d),at %d line %d column\n",dual[pos].lexme.lexme_text,dual[pos].dual_type, dual[pos].x,dual[pos].y);
                break;
            }
            pos++;                                      //位置加一
        }
        else if(isDelimiter(now))                   //判断是否是分界符
        {
            if(col!=1&&row!=1&&flag==0) col++;
            bool zhushi=false;
            dual[pos].x = row;dual[pos].y = col;
            dual[pos].lexme.lexme_text[0] = now;
            int index = 1;
            switch (now)
            {
            case '<':
                now = getc(fp);
                col++;
                if(now == '>'){
                    dual[pos].dual_type = NE;
                    dual[pos].lexme.lexme_text[index++] = now;
                }
                else if(now == '=')
                {
                    dual[pos].dual_type = LE;
                    dual[pos].lexme.lexme_text[index++] = now;
                }
                else{
                    dual[pos].dual_type = LT;
                    fseek(fp,-1,1);                 //如果是单分界符则回退一个字符
                    col--;
                }
                break;
            case '>':
                now = getc(fp);
                col++;
                if(now == '='){
                    dual[pos].dual_type = GE;
                    dual[pos].lexme.lexme_text[index++] = now;
                }
                else
                {
                    dual[pos].dual_type = GT;
                    fseek(fp,-1,1);
                    col--;
                }
                break;
            case '=':
                dual[pos].dual_type = EQ;
                break;
            case ':':
                now = getc(fp);
                col++;
                if(now =='='){
                    dual[pos].dual_type = EQUAL;
                    dual[pos].lexme.lexme_text[index++] = now;
                }
                else{
                    dual[pos].dual_type = COLON;
                    fseek(fp,-1,1);
                    col--;
                }
                break;
            case '+':
                dual[pos].dual_type = PLUS;
                break;
            case '-':
                dual[pos].dual_type = MINUS;
                break;
            case '*':
                dual[pos].dual_type = MUL;
                break;
            case '/':
                dual[pos].dual_type = DIV;
                break;
            case ';':
                dual[pos].dual_type = SEMICOLON;
                break;
            case ',':
                dual[pos].dual_type = COMMA;
                break;
            case '{':                   //注释符号
                zhushi = true;
                do
                {
                    now = getc(fp);
                    col++;
                    if(now == '\n'){
                        row++;col=1;
                    }
                } while (now!='}'&&now!=EOF);
                if(now != '}'){
                    fclose(fp);
                    error_num++;
                    fprintf(msg,"lack of '}' at %d line %d column\n",dual[pos].x,dual[pos].y);
                    printf("lack of '}' at %d line %d column\n",dual[pos].x,dual[pos].y);
                    fclose(msg);
                    return error_num;
                }
                break;
            case '(':
                dual[pos].dual_type = LP;
                break;
            case ')':
                dual[pos].dual_type = RP;
                break;
            case '.':
                dual[pos].dual_type = POINT;
                break;
            default:
                break;
            }
            if (!zhushi)
            {
                fprintf(msg,"(%s , %d),at %d line %d column\n",dual[pos].lexme.lexme_text,dual[pos].dual_type,dual[pos].x,dual[pos].y);
                printf("(%s , %d),at %d line %d column\n",dual[pos].lexme.lexme_text,dual[pos].dual_type, dual[pos].x,dual[pos].y);
                pos++;
            }
        }
        else
        {
            if(!isgraph(now)) {
                
                if(now == '\n') {row++;col=1;}
                else if(now == '\t') col = col + 4;
                else col++;
                continue;
            }
            dual[pos].lexme.lexme_text[0] = now;
            dual[pos].lexme.lexme_text[1] = '\0';
            dual[pos].dual_type = ERROR_WORD;
            dual[pos].x = row;
            dual[pos].y = col++;
            fprintf(msg,"%s is a illegel string, at %d line %d column\n",dual[pos].lexme.lexme_text,dual[pos].x,dual[pos].y);
            printf("%s is a illegel string, at %d line %d column\n",dual[pos].lexme.lexme_text,dual[pos].x,dual[pos].y);
        }
    }
    fclose(fp);
    fclose(msg);
    total_size = pos;
    return error_num;
}

// int main()
// {
//     init();
//     int lex = Scanner();
//     cout<<"error: "<<lex<<endl;
//     return 0;
// }