#include <stdio.h>
#include <stdlib.h>

#define MAX 500
#define TEST 0
int b[MAX],r[MAX];
int c[MAX][MAX];

int p[MAX];//pairing red->blue
bool v[MAX];//red[?]‚ª‚·‚Å‚ÉV‚µ‚¢Š„‚è“–‚Äæ‚Ì‚È‚¢‚±‚Æ‚ªŒˆ‚Ü‚Á‚Ä‚¢‚éê‡true

int gcd(int x,int y)
{
	while(y!=0)
	{
		int r=x%y;
		x=y;
		y=r;
	}
	return x;
}

int match(int bl,int n,bool v[MAX])
{
	if(bl<0)
	{
		puts("error bl<0");
		exit(1);
	}
	for(int i=0;i<n;++i)
	{
		if(!c[bl][i] || v[i])continue;
		v[i]=true;
		int ano=p[i];
		if(p[i]<0)
		{
			p[i]=bl;
			return 1;
		}
		int sub=match(p[i],n,v);
		if(sub>0)
		{
			p[i]=bl;
			return sub;
		}
	}
	return 0;
}

int main(void)
{
	int m,n;
while(scanf("%d%d",&m,&n)==2 && (m||n))
{
	int count=0;
	for(int i=0;i<m;++i)
	{	
		scanf("%d",b+i);
	}
	for(int j=0;j<n;++j)
	{
		scanf("%d",r+j);
		p[j]=-1;
	}
	for(int i=0;i<m;i++)
	{
		for(int j=0;j<n;j++)
		{
			c[i][j]=gcd(b[i],r[j])!=1?1:0;
		}
	}
	for(int i=0;i<m;++i)
	{
		for(int j=0;j<n;j++)
		{
			v[j]=false;
		}
		int res=match(i,n,v);
		count+=res;
	}
	printf("%d\n",count);
	if(TEST)
	{
		for(int j=0;j<n;j++)
		{
			printf("(%d,%d)\n",p[j],j);
		}
	}
}
	return 0;
}