#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <sys/types.h>
#include <sys/stat.h>        
#define url_LENGTH 1000
#define Base "www.chitkara.edu.in\""
 int d=1;
struct Node
{
char url[1000];
int depth;
int key;
int visited;
struct Node *next;
struct Node *prev;

};






struct hashtable
{
 struct Node *ptr;
 int count;
};






int chkwhttodo(int k,struct hashtable *hash)
{

if(hash[k].ptr==NULL)
return 1;
else
return 0;

}







int generate_key(char * url)
{
int size=strlen(url);
static int s=3;
static int i=1;
int size_base=strlen(Base);
int ch=(int)url[size-url_LENGTH];
int key=(size+ch);
key=key-s;
key=key*100+i;
i++;
key=key%1877;
s=s%10+8;
return key;
}


int Normalizeurl(char* url) 
{
  int len = strlen(url);
  if (len <= 1 )
    return 0;
  //! Normalize all urls.
  if (url[len - 1] == '/') 
  {
    url[len - 1] = 0;
    len--;
  }
  int i, j;
  len = strlen(url);
  //! Safe check.
  if (len < 2)
    return 0;
  //! Locate the url's suffix.
  for (i = len - 1; i >= 0; i--)
    if (url[i] =='.')
      break;
  for (j = len - 1; j >= 0; j--)
    if (url[j] =='/')
      break;
  //! We ignore other file types.
  //! So if a url link is to a file that are not in the file type of the following
  //! one of four, then we will discard this url, and it will not be in the url list.
  if ((j >= 7) && (i > j) && ((i + 2) < len)) 
  {
    if ((!strncmp((url + i), ".htm", 4))
        ||(!strncmp((url + i), ".HTM", 4))
        ||(!strncmp((url + i), ".php", 4))
        ||(!strncmp((url + i), ".jsp", 4))
        ) 
    {
      len = len; // do nothing.
    } 
    else 
    {
      return 0; // bad type
    }
  }
  return 1;
}



void removeWhiteSpace(char* html) 
{
  int i;
  char *buffer = malloc(strlen(html)+1), *p=malloc (sizeof(char)+1);
  memset(buffer,0,strlen(html)+1);
  for (i=0;html[i];i++) 
  {
    if(html[i]>32)
    {
      sprintf(p,"%c",html[i]);
      strcat(buffer,p);
    }
  }
  strcpy(html,buffer);
  free(buffer); free(p);
}
int GetNexturl(char* html, char* urlofthispage, char* result, int pos) 
{
  char c;
  int len, i, j;
  char* p1;   //!< pointer pointed to the start of a new-founded url.
  char* p2;  //!< pointer pointed to the end of a new-founded url.

  // NEW
  // Clean up \n chars
  if(pos == 0) {
    removeWhiteSpace(html);
  }
  // /NEW

  // Find the <a> <A> HTML tag.
  while (0 != (c = html[pos])) 
  {
    if ((c=='<') &&
        ((html[pos+1] == 'a') || (html[pos+1] == 'A'))) {
      break;
    }
    pos++;
  }
  //! Find the url it the HTML tag. They usually look like <a href="www.abc.com">
  //! We try to find the quote mark in order to find the url inside the quote mark.
  if (c) 
  {  
    // check for equals first... some HTML tags don't have quotes...or use single quotes instead
    p1 = strchr(&(html[pos+1]), '=');
    
    if ((!p1) || (*(p1-1) == 'e') || ((p1 - html - pos) > 10)) 
    {
     // keep going...
      return GetNexturl(html,urlofthispage,result,pos+1);
    }
    if (*(p1+1) == '\"' || *(p1+1) == '\'')
      p1++;

    p1++;    

    p2 = strpbrk(p1, "\'\">");
    if (!p2) 
    {
      // keep going...
      return GetNexturl(html,urlofthispage,result,pos+1);
    }
    if (*p1 == '#') 
    { // Why bother returning anything here....recursively keep going...

      return GetNexturl(html,urlofthispage,result,pos+1);
    }
    if (!strncmp(p1, "mailto:",7))
      return GetNexturl(html, urlofthispage, result, pos+1);
    if (!strncmp(p1, "http", 4) || !strncmp(p1, "HTTP", 4)) 
    {
      //! Nice! The url we found is in absolute path.
      strncpy(result, p1, (p2-p1));
      return  (int)(p2 - html + 1);
    } else {
      //! We find a url. HTML is a terrible standard. So there are many ways to prevsent a url.
      if (p1[0] == '.') {
        //! Some urls are like <a href="../../../a.txt"> I cannot handle this. 
	// again...probably good to recursively keep going..
	// NEW
        
        return GetNexturl(html,urlofthispage,result,pos+1);
	// /NEW
      }
      if (p1[0] == '/') {
        //! this means the url is the absolute path
        for (i = 7; i < strlen(urlofthispage); i++)
          if (urlofthispage[i] == '/')
            break;
        strcpy(result, urlofthispage);
        result[i] = 0;
        strncat(result, p1, (p2 - p1));
        return (int)(p2 - html + 1);
      } else {
        //! the url is a absolute path.
        len = strlen(urlofthispage);
        for (i = (len - 1); i >= 0; i--)
          if (urlofthispage[i] == '/')
            break;
        for (j = (len - 1); j >= 0; j--)
          if (urlofthispage[j] == '.')
              break;
        if (i == (len -1)) {
          //! urlofthis page is like http://www.abc.com/
            strcpy(result, urlofthispage);
            result[i + 1] = 0;
            strncat(result, p1, p2 - p1);
            return (int)(p2 - html + 1);
        }
        if ((i <= 6)||(i > j)) {
          //! urlofthis page is like http://www.abc.com/~xyz
          //! or http://www.abc.com
          strcpy(result, urlofthispage);
          result[len] = '/';
          strncat(result, p1, p2 - p1);
          return (int)(p2 - html + 1);
        }
        strcpy(result, urlofthispage);
        result[i + 1] = 0;
        strncat(result, p1, p2 - p1);
        return (int)(p2 - html + 1);
      }
    }
  }    
  return -1;
}







int checkdepth(char *a)
{
if(a[0]>=49&&a[0]<=54&&a[1]=='\0')
{
return 1;
}
else
{
  return 0;
}
}

int checkseed(char *a)
{
int i=0;int flag=0;

  
 while(a[i]!='\0'&&a[i]!='/')
{
   if(a[i]!=Base[i])
{
   flag=1;
   break;
}
i++;
}
if(flag==0)
{
return 1;
}
else
{
return 0;
}
   

 
}
int valid(char *a)
{
char a1[500]="wget --spider ";
strcat(a1,a);
if(!system(a1)){
//printf("Valid url");
return 1;
}
else
{
//printf("Invalid url");
return 0;
}
}

void checkdir(char *dir)
{
  struct stat statbuf;
  if ( stat(dir, &statbuf) == -1 ) 
  {
    fprintf(stderr, "-----------------\n");
    fprintf(stderr, "Invalid directory\n");
    fprintf(stderr, "-----------------\n");
    exit(1);
  }

  //Both check if there's a directory and if it's writable
  if ( !S_ISDIR(statbuf.st_mode) ) 
  {
    fprintf(stderr, "-----------------------------------------------------\n");
    fprintf(stderr, "Invalid directory entry. Your input isn't a directory\n");
    fprintf(stderr, "-----------------------------------------------------\n");
    exit(1);
  }

  if ( (statbuf.st_mode & S_IWUSR) != S_IWUSR ) 
  {
    fprintf(stderr, "------------------------------------------\n");
    fprintf(stderr, "Invalid directory entry. It isn't writable\n");
    fprintf(stderr, "------------------------------------------\n");
    exit(1);
  }
}
void fetchdata(char *url)
{
char urlbuffer[url_LENGTH+1000]={0}; 
strcat(urlbuffer, "wget -O");
strcat(urlbuffer,"/home/chetan/Crawlerdir/temp.txt ");

strcat(urlbuffer, url);

system(urlbuffer);
}
void create_path(char * path,char *url)
{
static int file_id=1;
char urlbuffer[]="Crawlerdir/test";

char id[50];
sprintf(id,"%d",file_id);
char type[]=".txt";
strcat(path,urlbuffer);

strcat(path,id);
strcat(path,type);

printf("%s",path);
file_id++;
}

int transferdata(char * path,char *url,char *depth)
{
int size=0;
FILE *fp,*ft;
fp=fopen("Crawlerdir/temp.txt","r");
ft=fopen(path,"w");
fprintf(ft,url);
putc('\n',ft);
fprintf(ft,depth);
char ch;
while(1)
{ 
 ch=getc(fp);
 if(ch==EOF)
  break;
 else
  {putc(ch,ft);
size++;
}

}

fclose(ft);
fclose(fp);

fp=fopen("Crawlerdir/temp.txt","w");
fclose(fp);

return size;
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
  {html[i]=ch;

i++;
}
 
}
  html[i]='\0';
}

void findurl(char *html,char *buffer,char *result[100],int pos)
{

int p=pos;
char a[500]={0};
int f,g,v;
 for(int i=0;i<50;i++)
 {

     p=GetNexturl(html,buffer,a,p);
     
     // v=valid(a);
     
     if(p==-1)
     {
         break;
     }
     f=Normalizeurl(a);
      
    if(f==1)
    {
     for(int j=0;j<i;j++)
      {
       if(strcmp(result[j],result[i])==0)
       {
          f=0;
          break;
        }
      } 
    
    }
     if(f==1)
      {
       
      strcpy(result[i],a);
         }
else
{
i--;
}

 }

}

//----------------------------------------------------------------------------
























/*
void to_the_list(char *links[100],int s,struct Node ** head_ref,int deep,struct hashtable *hash)
{
 struct Node* last=*head_ref;
 struct Node* node;
 for(int i=0;i<s;i++)
 {
  /* int v=valid(links[i]);
  if(v==1)
  {

   
     node=(struct Node*)malloc(sizeof(struct Node));
     int urlSIZE=strlen(links[i]);
   
     strcpy(node->url,links[i]);
     node->visited=0;
     node->key=generate_key(links[i]);
     node->depth=deep;
     int toappend=0;
     int toinsert=0;
     int todo=chkwhttodo(node->key,hash);
     if(todo==1)
      toappend=1;
     else
      toinsert=1;
     if(toappend)
       {
       node->next=NULL;
       while(last->next!=NULL)
         last=last->next;
       last->next=node;
       node->prev=last;
       hash[node->key].ptr=node;
       hash[node->key].count=1;
       }

     else if(toinsert)
       {
       struct Node *addr= hash[node->key].ptr;
       int totrace=hash[node->key].count;
       while(totrace!=1)
        { addr=addr->next;
          totrace--;
         }

         if(addr->next!=NULL)
         {
            struct Node *temp=addr->next;
            addr->next=node;
            node->next=temp;
            node->prev=addr;
            temp->prev=node;
            (hash[node->key].count)++;  
          }else{
            node->next=NULL;
            while(last->next!=NULL)
            last=last->next;
            last->next=node;
            node->prev=last;
           }  
       }
        
  
   free(links[i]);
  }



}

*/





















void appendurl(char *result[100],struct Node** head)
{
int v;
for(int i=0;i<20;i++)
{
 v=valid(result[i]);
  if(v==1)
  {
 struct Node* new=(struct Node*)malloc(sizeof(struct Node));
struct Node *last=(*head);
  strcpy(new->url,result[i]);
  new->depth=d;
  new->visited=0;
  
  new->next=NULL;
    int flag=0;
  while(last->next!=NULL)
   {
    if(strcmp(last->url,new->url)==0)
    {
    flag=1;
    break;
    }
  last=last->next;
    
  }
  
  if(flag==0)
{
  last->next=new;
  new->prev=last;
}

}
}
for(int i=0;i<20;i++)
{
  free(result[i]);
}
}




void printhash(struct hashtable *hash)
{

  printf("----------------------------------------TABLE IS GOING TO PRINT  ------------------------------------------------\n");

for(int i=0;i<1877;i++)
{

if(hash[i].ptr!=NULL)
//cout<<"____"<<i<<"_______"<<hash[i].ptr<<"_______"<<hash[i].count<<endl;
printf("___%d___%d____%d___",i,hash[i].ptr,hash[i].count);

}
 
}


int main(int argc,char *argv[])
{
if(argc>4)
{
printf("EXIT0");
exit(0);
}

int  a=checkdepth(argv[2]);
if(a==0)
{
printf("EXIT2");
 exit(0);
}

 a=checkseed(argv[1]);
if(a==0)
{
printf("EXIT1");
 exit(0);
}
int g=valid(argv[1]);
checkdir(argv[3]);


struct Node* new=(struct Node*)malloc(sizeof(struct Node));

strcpy(new->url,argv[1]);

new->depth=d;
d++;


new->visited=0;
new->prev=NULL;
new->next=NULL;
int count=0;

while(d!=5)
{
fetchdata(new->url);
char path[100]={0};
printf("--------------CTREATS PATHHHHH------------");
create_path(path,new->url);

int s=transferdata(path,new->url,argv[2]);
s=s+2;
char *html=(char *)malloc(s*sizeof(char *));
converttoram(path,html);
char **result=(char **)malloc(50*sizeof(char **));
for(int i=0;i<50;i++)
{
result[i]=(char *)malloc(500*sizeof(char));
}
struct hashtable *hash=(struct hashtable *)malloc(1877*sizeof(struct hashtable));

findurl(html,Base,result,0);
appendurl(result,&new);
printf("---------------------WORKING ");
// to_the_list(result,50,&new,d,hash);
new->visited=1;
new=new->next;


count++;
if(count==20)
{
 d++;
 count=0;
}

free(html);


/*struct Node* c=new;
while(c->next!=NULL)
{
printf("%s\n%d\n%d\n",c->url,c->depth,c->visited);
c=c->next;
}



//   printhash(hash);
*/
}

}



