#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<iostream>
using namespace std;
void GETLINE();
void PROCESSLINE();
void DEFINE();
void EXPAND();


 FILE *expanded;	//pointer to output file
 FILE *input;		//pointer to input file
char cheat[20] = "&OUTDEV";
 char label[50],opcode[50],operand[55],match[55],ans[55];
 char line[80];	//current line
 int namcount=0;	//total no. of names in NAMTAB
 int defcount=0;	//total no. of lines in DEFTAB
 int EXPANDING;
 int curr;
 int match_cur=0;
 int now_cur=0;

 struct namtab
 {
 	char name[20];
 	int start,end;
 	char param_tem[3][9];
 }mynamtab[20];

 struct deftab
 {
        char macroline[70];
  }mydeftab[25];

 struct argtab
 {
       char arg[3][9];
 }myargtab;

//Main Macro Processor

int main()
{
	char option;
	EXPANDING=0;

	input =fopen("in.txt","r");      //opens the input file   in readable mode
	expanded=fopen("expanded.txt","w");	//opens the output file in writable mode
	GETLINE();	//gets the current line in the form of label,opcode and operand

	while(strcmp(opcode,"END")!=0)	//string not equal to END
	{
		PROCESSLINE();
       		GETLINE();
	}

        fprintf(expanded,"%s",line);	//copies the 'line' in expandable file
     	printf("\n*******************************\n\nEXPANDED FILE GENERATED!\n\n*******************************\n");
/*
	printf("\nDo you want to open the expanded file?(Y/N)\n");
	scanf("%c",&option);
     	if(option=='Y'||option=='y')
		system("sh sscript.sh");
	else
		exit(0);*/


  }
  char *str_replace (char *source, char *find,  char *rep){
   // 搜尋文字的長度
   int find_L=strlen(find);
   // 替換文字的長度
   int rep_L=strlen(rep);
   // 結果文字的長度
   int length=strlen(source)+1;
   // 定位偏移量
   int gap=0;

   // 建立結果文字，並複製文字
   char *result = (char*)malloc(sizeof(char) * length);
   strcpy(result, source);

   // 尚未被取代的字串
   char *former=source;
   // 搜尋文字出現的起始位址指標
   char *location= strstr(former, find);

   // 漸進搜尋欲替換的文字
   while(location!=NULL){
       // 增加定位偏移量
       gap+=(location - former);
       // 將結束符號定在搜尋到的位址上
       result[gap]='\0';

       // 計算新的長度
       length+=(rep_L-find_L);
       // 變更記憶體空間
       result = (char*)realloc(result, length * sizeof(char));
       // 替換的文字串接在結果後面
       strcat(result, rep);
       // 更新定位偏移量
       gap+=rep_L;

       // 更新尚未被取代的字串的位址
       former=location+find_L;
       // 將尚未被取代的文字串接在結果後面
       strcat(result, former);

       // 搜尋文字出現的起始位址指標
       location= strstr(former, find);
   }

   return result;

}


void substring(char *dest, const char *src, int start, int length) {
    int i;

    // 确保起始位置和长度在字符串范围内
    for (i = 0; i < length && src[start + i] != '\0'; i++) {
        dest[i] = src[start + i];
    }
    dest[i] = '\0'; // 在目标字符串末尾添加空字符
}

//GETLINE Function

void GETLINE()
{
	char word1[35],word2[35],word3[35],buff[30];
	int count=0,i,j=0;
	if(EXPANDING){
		strcpy(line,mydeftab[curr++].macroline);
		}	//copy the line from deftab to 'line' and increment the value of curr
 	else {fgets(line,40,input);}				//gets till 20 characters from input file into 'line'

        opcode[0]='\0';	//putting null character in the arrays
	label[0]='\0';
	operand[0]='\0';
	word1[0]='\0';
	word2[0]='\0';
	word3[0]='\0';

        for(i=0;line[i]!='\0';i++)	//continue for till line ends
        {
        	if(line[i]!=' ')
                	buff[j++]=line[i];	//gets words in the buffer
                else
                {
                	buff[j]='\0';		//end the buffer
                 	strcpy(word3,word2);	//shifting of words
                    	strcpy(word2,word1);
                       	strcpy(word1,buff);

                     	j=0;
			count++;
                }//end ifelse
        }//end for


        buff[j-1] = '\0';
        strcpy(word3, word2);
        strcpy(word2, word1);
        strcpy(word1, buff);

	switch(count)
        {
        	 case 0:strcpy(opcode,word1);
			break;
                 case 1:{strcpy(opcode,word2);strcpy(operand,word1);}
			break;
                 case 2:{strcpy(label,word3);strcpy(opcode,word2);strcpy(operand,word1);} //sorting the words in the line
			break;
        }

        //end switch
}//end getline



//PROCESSLINE Function
void PROCESSLINE()
{
	int i;
        for(i=0;i<namcount;i++) //till the total no. of macros
        	if(!strcmp(opcode,mynamtab[i].name))	//if opcode matches in the NAMTAB
                {
                	EXPAND();return;		//then expand
                }
                {
                        if(!strcmp(opcode,"MACRO")){	//if "MACRO" keyword is encountered
		                DEFINE();
                        }
		                		//then define
                        else fprintf(expanded,"%s",line);
                        	//otherwise copy it into the expanded file
                }
}//end processline



//DEFINE Function
void DEFINE()
{
	int LEVEL,i=0,j=0,k=0;
        char param[3][9];
        char s[3];
        strcpy(s,"123");

        strcpy(mynamtab[namcount].name,label);		//enter Macro name in NAMTAB
        mynamtab[namcount].start=defcount;		//enter starting pointer
        strcpy(mydeftab[defcount].macroline,line);	//enter the line in the deftab which the starting pointer is pointing

        while(operand[i]!='\0')	//while operand is there
        {
        	if(operand[i]!=','){
                	mynamtab[namcount].param_tem[j][k]=param[j][k]=operand[i];++k;}
                else
                {
                	mynamtab[namcount].param_tem[j][k]=param[j][k]='\0';++j;
                        k=0;
                }
                i++;
        }//end while
        mynamtab[namcount].param_tem[j][k]=param[j][k]='\0';


        LEVEL=1;
        //for nested macro
        while(LEVEL>0)
        	{
                	GETLINE();
           		if(operand[0]!='\0')	//if operand is there
             		{
            			for(i=0;i<3;i++)
			        {
			        	if(strstr(operand,param[i]))
				        {
				            strcpy(match,operand);


						operand[0]='?';		//substituting the positional parameter
            					operand[1]=s[i];
            					operand[2]='\0';
            					strcat(operand,match);



             				}
             				//end if
             			}//end for
             		}//end if
           		if(!strcmp(opcode,"MACRO"))
            			LEVEL++;
           		else if(!strcmp(opcode,"MEND"))
            			LEVEL--;

            		strcpy(mydeftab[defcount].macroline,opcode);
            		//copy the opcode in deftab
            		if(operand[0]!='\0')
	            	{


		            strcat(mydeftab[defcount].macroline," ");	//copy the operand in deftab
        		    strcat(mydeftab[defcount].macroline,operand);

        		    strcat(mydeftab[defcount].macroline,"\0");


        	        }

                    strcat(mydeftab[defcount++].macroline,"\n");

		}

                mynamtab[namcount++].end=defcount;	//get the end pointer to deftab
}//end define



//EXPAND Function

void EXPAND()
{

	int i,end=0,j=0,k=0;
        EXPANDING=1;
        int arg=0;
        int flag=0;
        fprintf(expanded,".%s",line);		//comments the call

        for(int s=0;s<namcount;s++)
        {
        	if(!strcmp(opcode,mynamtab[s].name))	//if opcode matches name available in the NAMTAB
                {   flag=s;
                	curr=mynamtab[s].start;
                        end=mynamtab[s].end;

                        int i=0;
                        while(operand[i]!='\0')		//if operand is there
                        {
                        	if(operand[i]!=','){

                                myargtab.arg[j][k++]=operand[i];

                        	}
                                else
                                {
                                        myargtab.arg[j++][k]='\0';

                                	k=0;
                                }


                                i++;
                        }

                       //endwhile
                        myargtab.arg[j][k]='\0';

                         	//generating the ARGTAB
                 }//end if
        }

        //end for
        while(curr<(end-1)) //continue till end of the DEFTAB is reached
        {
        	GETLINE();
                if(operand[0]=='?'){
                   int now=(int)operand[1]-49;

                       //if positional parameter
                  substring(match,operand,2,35);



   strcpy(ans,str_replace(match, mynamtab[flag].param_tem[now],myargtab.arg[operand[1]-'0'-1]));






strcpy(operand,ans);



                }
		       	//then put its value from ARGTAB in the operand


			fprintf(expanded,"%s %s %s\n",label,opcode,operand); //copy the whole line in expanded file
        }
        EXPANDING=0; //end of expansion
}//end expand

