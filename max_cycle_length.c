#include "stdio.h"
#include "stdlib.h"
unsigned int B = 10;

void fullAdder(int a, int b, int c, int* s, int* carry)
{ 
	unsigned int sum = a + b + c;
	*carry = sum/B;
	*s = sum - (*carry)*B;
	return;
}

void intMult(int a, int b, int* s, int* carry)
{ 
	unsigned int prod = a*b; 
	*carry = prod/B; 
	*s = prod - (*carry)*B;
	return;
}

void mult(int* a,int n)
{ 
	int i = 0;
	int carry = 0, c=0, d=0, cprev = 0;
	for (i = 0; i < n; i++)
	{ 	
		intMult(a[i],3,&d,&c);
		fullAdder(d, cprev, carry, &a[i], &carry); cprev = c;
	}
	d = 0;
	fullAdder(d, cprev, carry, &a[n], &carry);
	return;
}

void split(int* a,int n,int* a1,int* a0)
{ 
	int i = 0;
	int k = n/2;
	for (i = 0; i < k; i++) a0[i] = a[i];
	for (i = 0; i < n - k; i++) a1[i] = a[k+i];
	return;
}

void addAt(int* a,int n,int ak, int k)
{ 
	int carry = 0,i=0;
	fullAdder(a[k], ak, carry, &a[k], &carry);
	for (i = k+1; i <(n+1); i++)
	fullAdder(a[i],0, carry, &a[i], &carry);
	return;
}

void Karatsuba(int* a,int n,int n0)
{ 
	if(n0 < 4) return;
	if (n < n0)
	{
		mult(a,n);return;
	}
	int k = n/2;
	int i = 0;
	int a0[k], a1[n - k];
	split(a,n,a1,a0);
	Karatsuba(a1,n-k,n0);
	Karatsuba(a0,k,n0);
	for (i = 0; i <k; i++) a[i] = a0[i];
	for (i = k; i < n+1; i++) a[i] = a1[i - k];
	addAt(a,n,a0[k],k);
	return;
}

void addOne(int *a,int l)
{
	int carry = 0,i=0;
	fullAdder(a[0],1, carry, &a[0], &carry);
	for (i =1; i <=l; i++)
	fullAdder(a[i], 0, carry, &a[i], &carry);
	return;
}		

int checkForOne(int* a, int l)
{
	if(a[0]==1 && l==1) return(1);
	else return(0);
}

void carryAdder(int q,int rprev,int* d)
{
	*d=q+(rprev*B/2);
}

void intDiv(int a, int b, int* q, int* carry)
{ 
	*q = a/b;
	*carry = a%b; return;
}

void divide(int *a, int n,int* carry)
{
	int i = 0;
	int r=0, q=0, rprev = 0;
	for (i = 0; i < n; i++)
	{ 	
		intDiv(a[n-1-i],2,&q,&r);
		carryAdder(q, rprev,&a[n-i-1]);
		rprev=r;
	}
	*carry=r;
	return;
}

void divideByTwo(int* a,int n,int* carry,int n0)
{
	if(n0 < 4) return;
	if (n < n0)
	{
		divide(a,n,carry);return;
	}
	int k = n/2;
	int i = 0;
	int a0[k], a1[n - k];
	int carry1=0;
	int carry0=0;
	split(a,n,a1,a0);
	divideByTwo(a1,n-k,&carry1,n0);
	divideByTwo(a0,k,&carry0,n0);
	for (i = 0; i <k; i++) a[i] = a0[i];
	a[k-1]+=carry1*(B/2);
	for (i = k; i < n; i++) a[i] = a1[i-k];
	*carry=carry0;
	return;
}

int lArray(int* a,long int n,int B)
{
	int noOfDigits=1,i=0;
	for(i=0;i<10;i++)
	{	
		a[i]=n%B; n=n/B;
		if(n!=0) noOfDigits++;
		else break;
	}
	return(noOfDigits);
}
	
int main(int argc, char** argv)
{
	if( argc<3 ) 
	{
		printf("give inputs i j\n"); return(0);
	}
	long int value=0,i=atoi(argv[1]), j=atoi(argv[2]);
	int carry=0,n0=6, max_cycle_length=0, count=1, max=0;
	if(i<1 || j>100000000 || j<i) 
	{
		printf("inputs i or j are not proper or exceeding limits\n"); return(0);
	}
	for(value=i;value<=j;value++)
	{	
		int l=0,flag=0;		
		count=1;
		if(value!=1)		
		{
			int a[10];
			l=lArray(a, value, B);
			while(flag!=1)
			{
				if(a[0]%2==0)
				{
					count++;			
					divideByTwo(a,l,&carry,n0); 
					if(a[l-1]==0) l--;
				}
				else
				{
					count++;
					Karatsuba(a, l, n0);
					addOne(a,l);
					if(a[l]!=0) l++;
				}
				flag=checkForOne(a,l);		
			}
		}
		if(count > max_cycle_length) 
		{
			max_cycle_length = count;
			max=value;
		}
	}
	printf("%lu %lu %d\n",i,j,max_cycle_length);	
}

			
	

		
		

