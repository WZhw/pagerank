#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#define maxrownum 0
#define maxcolnum 0
#define maxurllen 200
#define maxcoosize 5000000
#define a 0.15
#define e 0.001

typedef struct{
    int row[maxcoosize],col[maxcoosize];
    double value[maxcoosize];
    int totalnum;
}Triple;

Triple G1;
int    g[maxrownum][maxcolnum]={{0,},};
double G[maxrownum][maxcolnum]={{0,},};
int indegree[500000]={0};
int outdegree[500000]={0};
double V[500000]={0};
double Vtemp[500000]={0};
char urls[500000][maxurllen];
double l=0;
void topTen(char* wfilename,int N);

int main(int argc, char** argv)
{



	memset(&G1,0,sizeof(G1));
	int row_num=0;
	int edges_num=0;
    FILE *fpu;

    if(argc<3)
    {
        printf("parameter error!\n");
        return 0;
    }
	if((fpu=fopen(argv[1],"r"))==NULL)
	{
		printf("file %s open failed!\n",argv[1]);
		return 0;
	}

	int flag=0;
	char line[200];
	char url[maxurllen];
	int url_num;
	int i,j,col;
	while(!feof(fpu))
	{
        if(flag==0)
		{
 			fgets(line,200,fpu);
			if(strcmp(line,"\n")!=0)
			{
				sscanf(line,"%s %d",url,&url_num);
				strcpy(urls[url_num],url);
				row_num++;
			}
			else
				flag=1;
		}
		else
		{
		    fgets(line,200,fpu);
		    if(strcmp(line,"\n")==0)
                continue;
			sscanf(line,"%d %d",&i,&j);
			col=indegree[i];

                G1.row[G1.totalnum]=i;
                G1.col[G1.totalnum]=j;
                G1.value[G1.totalnum]=1;
                G1.totalnum++;

            indegree[i]+=1;
            outdegree[j]+=1;
            edges_num+=1;
		}
	}
    for(i=0;i<row_num;i++)
    {
            V[i]=(double)indegree[i]/edges_num;
            Vtemp[i]=V[i];
    }


	int offset=0;
	for(i=0;i<G1.totalnum;i++)
    {
                G1.value[i]=G1.value[i]/outdegree[G1.col[i]];
                G1.value[i]=(1-a)*G1.value[i];

    }


	double E=1,Etemp;
	int num=0;
	while(E>e)
	{
		num++;
		for(i=0;i<row_num;i++)
		{
			Vtemp[i]=V[i];
			V[i]=0;
		}
		E=0;
		double sum=0;
		offset=0;

		for(i=0;i<row_num;i++)
        {
                sum+=a/row_num*Vtemp[i];
        }
       	for(i=0;i<row_num;i++)
        {
                V[i]+=sum;
        }

		for(i=0;i<G1.totalnum;i++)
        {
            V[G1.row[i]]+=G1.value[i]*Vtemp[G1.col[i]];
        }
       	for(i=0;i<row_num;i++)
        {
            Etemp=fabs(V[i]-Vtemp[i]);
            if(Etemp>E)
                E=Etemp;
        }
        printf("%d:%.10f\n",num,E);
	    }
  	    for(i=0;i<row_num;i++)
        {l+=V[i];
        }
    	for(i=0;i<row_num;i++){
		V[i]=V[i]/l;
	    }
        printf("power: %d\n",num);
        printf("You can open the top10.txt to check the result.\n");
        topTen(argv[2],row_num);
	    return 0;
        }

void topTen(char* wfilename,int N)
{
    FILE *fpt;
    if((fpt=fopen(wfilename,"w"))==NULL)
	{
		printf("file %s open failed!\n",wfilename);
		return;
	}

    int i,j,top;
    double top_value;
    for(i=0;i<10;i++)
    {
        top_value=0;
        for(j=0;j<N;j++)
        {
            if(V[j]>top_value)
            {
                top=j;
                top_value=V[j];
            }
        }
        fprintf(fpt,"%-80s %.10f\n",urls[top],top_value);
        V[top]=0;
    }
    fclose(fpt);
}
