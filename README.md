# 介绍

一个简单Pascal语言编译器的课程设计  

## 产生式

<程序>—> PROGRAM<标识符>；<分程序>  
<分程序>—><变量说明>BEGIN<语句表>END.  
<变量说明>—>VAR<变量说明表>  
<变量说明表>—><变量表>:<类型>|<变量表>:<类型>;<变量说明表>  
<类型>—>INTEGER  
<变量表>—><变量>|<变量>,<变量表>  
<语句表>—><语句>|<语句>;<语句表>  
<语句>—><赋值语句>|<条件语句>|<WHILE语句>|<复合语句>  
<赋值语句>—><变量> := <算术表达式>  
<条件语句>—>IF<关系表达式>THEN<语句>ELSE<语句>  
<WHILE语句>—>WHILE<关系表达式>DO<语句>  
<复合语句>—>BEGIN<语句表>END  
<算术表达式>—><项>|<算术表达式> + <项>|<算术表达式> - <项>  
<项>—><因式>|<项>* <因式>|<项>/ <因式>  
<因式>—><变量>|<常数>| <算术表达式>  
<关系表达式>—><算术表达式><关系符><算术表达式>  
<变量>—><标识符>  
