#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <sys/types.h>
#include <sys/stat.h>  
#define url_LENGTH 1000
struct hashtable
{
 struct Node *ptr;
 int count;

};
struct Node
{
char word[50];
struct Node *prev;
struct Node *next;
int key;
struct fileno *ref;
};
struct fileno
{
 int count;
 int id;
 struct fileno *next;


};

void printhash(struct hashtable *hash)
{

  printf("----------------------------------------TABLE IS GOING TO PRINT  ------------------------------------------------\n");

for(int i=0;i<1877;i++)
{

if(hash[i].ptr!=NULL)
 
printf("___%d___%d____%d___\n",i,hash[i].ptr,hash[i].count);

}
 
}

int chkwhttodo(int k,struct hashtable *hash)
{

if(hash[k].count==0)
return 1;
else
return 0;

}



int generate_key(char * url)
{
int size=strlen(url);
if(size==0)
return 0;
else
return (int)url[0];

}


void to_the_list(char *links[1000],int s,struct Node ** head_ref,struct hashtable *hash,int page)
{
//printf("%d",page);
struct Node* last=*head_ref;
struct Node* node;
struct fileno* head; 
 
int flag=0;
 for(int i=0;i<s;i++)
 {

 flag=0;
      if(links[i]=="")
	continue;
     last=*head_ref;
     node=(struct Node*)malloc(sizeof(struct Node));
     
   
     strcpy(node->word,links[i]);
     
     node->key=generate_key(links[i]);
     
     int toappend=0;
     int toinsert=0;
     int todo=chkwhttodo(node->key,hash);
     if(todo==1)
      toappend=1;
     else
      toinsert=1;
     if(toappend)
       {
       //printf("%s\n",links[i]);
       head=(struct fileno*)malloc(sizeof(struct fileno));
       head->count=1;
	head->id=page;
	head->next=NULL;
       
       node->ref=head;
       // printf("%s\n%d\n%d\n",links[i],head->id,head->count);
       while(last->next!=NULL)
         last=last->next;
       last->next=node;
       node->prev=last;
       node->next=NULL;
       hash[node->key].ptr=node;
       hash[node->key].count=1;
       }

else if(toinsert)
{

int f=0;
   int k=0;
    
       struct Node *addr= hash[node->key].ptr;
       int totrace=hash[node->key].count;
       
       while(totrace!=1)
        { 
           if(strcmp(addr->word,links[i])==0){
          //  printf("%s %s\n",addr->word,links[i]);
           flag=1;
           break;
           }
           addr=addr->next;
          totrace--;
        }
     if(flag==1)
     {
        //printf("%s\n",links[i]);
         struct fileno *temp=addr->ref;
         while(addr->ref->next!=NULL){
	if(addr->ref->id==page){f=1;break;}
	 addr->ref=addr->ref->next;
   	}
	if(addr->ref->id==page||f==1)
	{
         //printf("%s\n",links[i]);
         

         //printf("%s %s\n",links[i],last->word);
         addr->ref->count++;
         addr->ref=temp;
        //printf("%d\n",addr->ref->count);
	}


        
	  else
         {
          head=(struct fileno*)malloc(sizeof(struct fileno));
       head->count=1;
	head->id=page;
	head->next=NULL;
	
         
        
          
         addr->ref->next=head;
	 addr->ref=temp;
          }	
	
     }
            
     else
     {
       //printf("%s\n",links[i]);
       
        head=(struct fileno*)malloc(sizeof(struct fileno));
        head->count=1;
	head->id=page;
	head->next=NULL;
        node->ref=head;
         if(addr->next!=NULL)
         {
            struct Node *temp=addr->next;
            addr->next=node;
            node->next=temp;
            node->prev=addr;
            temp->prev=node;
            (hash[node->key].count)++;  
          }
	else{
            node->next=NULL;
            
            addr->next=node;
            node->prev=addr;
           }
       }

      
     }

      }
  
  
 

  }


















int findword(char* html,char* result, int i)
{
        while(html[i]!='\0')
	{
		if(html[i]=='<'&&html[i+1]=='t'&&html[i+2]=='i'&&html[i+3]=='t'&&html[i+4]=='l'&&html[i+5]=='e')
		{
			i=i+5;
			while(html[i]!='>')
			{
				i++;
			}
			i++;
			int j=0;
			while(html[i]!='<')
			{
				result[j]=html[i];
				i++;
				j++;
			}
			result[j]='\0';
			return i;
		}
           /*   else if(html[i]=='<'&&html[i+1]=='m'&&html[i+2]=='e'&&html[i+3]=='t'&&html[i+4]=='a')
             {
              i=i+4;
              while(html[i]!='c'&&html[i+1]=='o'&&html[i+2]=='n'&&html[i+3]=='t'&&html[i+4]=='e'&&html[i+5]=='n'&&html[i+6]=='t')
                  {
                    i++;
                   }
                 i=i+9;
                int j=0;
              while(html[i]!='/')
			{
				result[j]=html[i];
				i++;
				j++;
			}
			result[j]='\0';
			return i;

             }
                  */
       else if(html[i]=='<'&&html[i+1]=='h'&&(html[i+2]=='1'||html[i+2]=='2'||html[i+2]=='3'||html[i+2]=='4'||html[i+2]=='5'||html[i+2]=='6'))
		{
			i=i+3;
			while(html[i]!='>')
			{
				i++;
			}
			i++;
			int j=0;
			while(html[i]!='<'&&html[i+1]!='/')
			{
				result[j]=html[i];
				i++;
				j++;
			}
			result[j]='\0';
			return i;
		}

           i++;
	}
	return -1;
}


void printlist(struct Node *new)
{
while(new->next!=NULL)
{
 printf("%s\n%d\n",new->word,new->key);
new=new->next;
}


}


void putinfile(struct Node *new)
{
char a[50];
int i=0;
int num;
FILE *f=fopen("Crawlerdir/index.txt","w");
while(new!=NULL)
{
fprintf(f,new->word);
putc(' ',f);
while(new->ref!=NULL)
{
i=0;
putc('i',f);
num=new->ref->id;
while(num!=0)
{
int rem=num%10;
a[i++]=rem+'0';
num=num/10;
}
a[i]='\0';
int j=i-1;
int k=0;
while(k<j)
{
char temp=a[k];
a[k]=a[j];
a[j]=temp;
k++;
j--;
}
i=0;
while(a[i]!='\0')
{
putc(a[i],f);
i++;
}
putc('c',f);
i=0;
num=new->ref->count;
while(num!=0)
{
int rem=num%10;
a[i++]=rem+'0';
num=num/10;
}
a[i]='\0';
 j=i-1;
 k=0;
while(k<j)
{
char temp=a[k];
a[k]=a[j];
a[j]=temp;
k++;
j--;
}
i=0;
while(a[i]!='\0')
{
putc(a[i],f);
i++;
}

putc(' ',f);
new->ref=new->ref->next;
}
new=new->next;
putc('\n',f);
}
fclose(f);



}













void converttoram(char *path,char *html)
{

FILE  *fp=fopen(path,"r");
int i=0;

char ch;
while(1)
{ 
 ch=getc(fp);
 if(ch==EOF)
  break;
 else
  {
html[i]=ch;

i++;
}
 
}
  html[i]='\0';
}
int getfilesize(char *path)
{
struct stat st;
stat(path,&st);
return st.st_size;

}
int j=0;
int main()
{


struct hashtable *hash=(struct hashtable *)malloc(1877*sizeof(struct hashtable));
struct Node* new=(struct Node*)malloc(sizeof(struct Node));

int h=0;
for(int i=1;i<=10;i++)
{
h=0;
char **words=(char **)malloc(1000*sizeof(char **));
for(int j=0;j<1000;j++)
{
words[j]=(char *)malloc(100*sizeof(char));
}
char path[100];
char urlbuffer[]="Crawlerdir/test";

char id[50];
sprintf(id,"%d",i);
char type[]=".txt";
strcpy(path,urlbuffer);
strcat(path,id);
strcat(path,type);
int s=getfilesize(path);
s=s+2;
char *html=(char *)malloc(s*sizeof(char));
converttoram(path,html);
char *temp[1000];

int g=0;
int k=0;
char a[500];
int pos=0;
while(k!=-1)
{
 temp[j]=(char *)malloc(5000*sizeof(char));
 k=findword(html,temp[j],pos);
 temp[j]+='\0';
 pos=k;
 //printf("%s\n",temp[j]);
 for(int t=0;temp[j][t]!='\0';t++)
 {
 if(temp[j][t]==' ')
  {
   a[g]='\0';
   strcpy(words[h],a);
   words[h]+='\0';
   g=0;
  h++;
  while(temp[j][t]==' ')
  {
  t++;
  }

  t--;
  }
  else
  {
  if((temp[j][t]>=65&&temp[j][t]<=90)||(temp[j][t]>=97&&temp[j][t]<=122)||(temp[j][t]>=48&&temp[j][t]<=57))
  a[g++]=temp[j][t];
  }
      
 }

j++;
}


to_the_list(words,h,&new,hash,i);
for(int u=0;u<h;u++)
free(words[u]);

}

//printlist(new);
//printhash(hash);
 /* for(int b=0;b<h;b++)
{
printf("%s\n",words[b]);
}
*/
putinfile(new);
}






