# 介绍

一个简单Pascal语言编译器的课程设计。  
项目在vscode上完成编写和运行  
由于课设时间比较赶，本人也没有太多时间去完善这个课设，在词法部分的单词定位存在一些因为写法上的bug就是定位不准确，REDME里列出来的first集和follow集，是我们组手动找的不确定是不是就是完全正确，语法上可能少了一些语法功能的实现。当然也验收通过了就暂且不在花费时间去修改了，在代码编写的过程中也是参考的网上一些学长学姐的代码，在此表示感谢。  

## 产生式

<程序>—> PROGRAM<标识符>;<分程序>  
<分程序>—><变量说明>BEGIN<语句表>END.  
<变量说明>—>VAR<变量说明表>  
<变量说明表>—><变量表>:<类型>{;<变量说明表>}  
<类型>—>INTEGER  
<变量表>—><变量>{,<变量表>}  
<语句表>—><语句>{;<语句表>}  
<语句>—><赋值语句>|<条件语句>|<WHILE语句>|<复合语句>  
<赋值语句>—><变量> := <算术表达式>  
<条件语句>—>IF<关系表达式>THEN<语句>ELSE<语句>  
<WHILE语句>—>WHILE<关系表达式>DO<语句>  
<复合语句>—>BEGIN<语句表>END  
<算术表达式>—><项>{+ <项>| - <项>}  
<项>—><因式>{* <因式>|/<因式>}  
<因式>—><变量>|<常数>| (<算术表达式>)  
<关系表达式>—><算术表达式><关系符><算术表达式>  
<变量>—><标识符>  
<标识符>—><字母>{<数字>|<字母>}  
<关系符>—> = | <= | > | >= | <> | <

## First集

first(<程序>) = {'PROGRAM'}  
first(<分程序>) = {'VAR','BEGIN'}  
first(<变量说明>) = {'VAR'}  
first(<变量说明表>) = {字母}  
first(<类型>) = {'INTEGER'}  
first(<变量表>) = {字母}  

First(<语句表>)={字母，IF，While，BEGIN}  
First(<语句>)={字母，IF，While，BEGIN}  
First(<赋值语句>)={字母}  
First(<条件语句>)={IF}  
First(<While语句>)={While}  
First(<复合语句>)={BEGIN}  

first(<算术表达式>) = {字母,数字,'('}  
first(<项>) = {字母,数字,'('}  
first(<因式>) = {字母,数字,'('}  
first(<关系表达式>) = {字母,数字,'('}  
first(<变量>) = {字母}  
first(<标识符>) = {字母}

## Follow集

follow(<程序>) = {#}  
follow(<分程序>) = {#}  
follow(<变量说明>) = {#}  
follow(<变量说明表>) = {#}  
follow(<类型>) = {#}  
follow(<变量表>) = {#}  

Follow(<语句表>)={'END',#}  
Follow(<语句>)={'ELSE',# }  
Follow(<赋值语句>)={#}  
Follow(<条件语句>)={#}  
Follow(<while语句>)={#}  
Follow(<复合语句>)={#}  

follow(<算术表达式>) = {<>,>=,<=,>,<>,=}  
follow(<项>) = {#}  
follow(<因式>) = {#}  
follow(<关系表达式>) = {'DO','THEN'}  
follow(<变量>) = {':=',#}  
follow(<标识符>) = {';',#}  
