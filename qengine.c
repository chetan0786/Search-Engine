#include<stdio.h>
#include<string.h>
#include<stdlib.h>
struct node *root=NULL;
struct hash
{
    struct node *first;
	int count;
};
struct fileno
{
	int count;
	int id;
	struct fileno *left;
    struct fileno *right;
};
struct node	
{
    char word[100];
	struct fileno *ref;
    struct node *left; 
    struct node *right;
};
struct query
{
	char word[100];
	struct fileno *ref;
};
struct disp
{
	int count;
	int id;
	int sum;
};

char path[50];
char* getFileName(int i)
{

	
char urlbuffer[]="Crawlerdir/test";

char id[50];
sprintf(id,"%d",i);
char type[]=".txt";
strcpy(path,urlbuffer);
strcat(path,id);
strcat(path,type);
	return path;
}


int main()
{

	struct hash h[51];
   		for(int i=0;i<50;i++)
    	{
			h[i].count=0;
    	}
		char input[1000];
		scanf("%[^\n]%*c",input); 
				
		char arr[10][100];
		int inword=0;
		int z=0;
		int k=0;
		for(int i=0;input[i]!='\0';i++)
		{
			if(input[i]==' ')
			{
				if(inword==1)
				{
					arr[z][k]='\0';
					z++;
					k=0;
				}
				inword=0;
			}
			else
			{
				arr[z][k]=input[i];
				k++;
				inword=1;
			}
		}
		if(inword==1)
   		{
			arr[z][k]='\0';
			z++;
		}

          FILE *reload;
    	   reload=fopen("Crawlerdir/index.txt","r");
   		char ch = getc(reload);
		int m=0;
		
              while(ch!=EOF)
	{
           
	    struct node *temp;
				temp=(struct node*) malloc(sizeof(struct node));
				temp->left=NULL;
				temp->right=NULL;
	
		while(ch!=' ')
		{
		temp->word[m]=ch;
                ch=getc(reload);
                m++;
		}	
		temp->word[m]='\0';
	        m=0;
                struct fileno *base=NULL;
		ch=getc(reload);
		
               
		while(ch!='\n')
		{
                  ch=getc(reload);
			struct fileno *t;
			t=(struct fileno*) malloc(sizeof(struct fileno));
	          	t->left=NULL;
			t->right=NULL;
			t->id=0;
			t->count=0;	
	                
			                      while(ch!='c')
						{
							t->id=t->id*10+(ch-48);
							ch = getc(reload);
						}
						ch = getc(reload);
						while(ch!=' '&&ch!='\n')
						{
							t->count=t->count*10+(ch-48);
							ch = getc(reload);
						}
					if(ch=='\n')
					break;
						ch = getc(reload);
                                                


						 if(base==NULL)
						{
							base=t;
						}
						else
						{
							struct fileno *p=base;
							while(p->right!=NULL)
							{
								p=p->right;
							}
							p->right=t;
							t->left=p;
						}
		

			
								
		
		}
              
                temp->ref=base;
	
		if(root==NULL)
				{
					root=temp;
				}
				else
				{
					struct node *p=root;
					while(p->right!=NULL)
					{
						p=p->right;
					}
					p->right=temp;
					temp->left=p;
				}
	        ch=getc(reload);

	}

       
       
















  	        int it=0;
		struct query q[10];
		struct node *p=root;
		while(p!=NULL)
		{
			for(int i=0;i<z;i++)
			{
				if(strcmp(p->word,arr[i])==0)
				{
					strcpy(q[it].word,p->word);
					q[it].ref=p->ref;
					it++;
				}
			}
			p=p->right;
		}
		
		struct disp u[11];
		for(int z=0;z<11;z++)
		{
			u[z].count=0;
			u[z].id=z;
			u[z].sum=0;
		}
		for(int z=0;z<11;z++)
		{
			for(int i=0;i<it;i++)
			{
				struct fileno *no=q[i].ref;
				while(no!=NULL)
			 	{
					if(no->id==u[z].id)
					{
						u[z].sum+=no->count;
						u[z].count++;
						break;
					}
					no=no->right;
			 	}
			}
		}
		for(int i=1;i<11;i++)
		{
			for(int j=i+1;j<11;j++)
			{
				if(u[i].count<u[j].count)
				{
					struct disp temp=u[i];
					u[i]=u[j];
					u[j]=temp;
				}
			}
		}
		for(int i=1;i<11;i++)
		{
			for(int j=i+1;j<11&&(u[i].count==u[j].count);j++)
			{
				if(u[i].sum<u[j].sum)
				{
					struct disp temp=u[i];
					u[i]=u[j];
					u[j]=temp;
				}
			}
		}
		if(it==0)
		printf("No Data Found");
		else{
		for(int i=1;i<11;i++)
		{
			char *file=getFileName(u[i].id);
			FILE *fp;
    		fp=fopen(file,"r");
   			int ch = getc(fp);
   			while (ch != '\n')
			{
		    	printf("%c",ch);
		    	ch = getc(fp);
			}
			printf("\n");
			fclose(fp);
		}

}}



