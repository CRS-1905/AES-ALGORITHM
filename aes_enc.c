#include<stdio.h>
#include<stdlib.h>
#include "dorkari.h"

int main()
{
	FILE *fp,*fp1;
	unsigned int i,i1,j,k1,p,q,r;
	unsigned char k[16],**key,**w,st[16],enc[16],**s,m_file[20];
	
	printf("Enter The message File Name:\n");
	scanf("%s",m_file);
	fp=fopen(m_file,"rb");
	fp1=fopen("ciphertext.txt","wb");
	getchar();
	printf("Enter a key string of 16 character:\n");
	
	for(i=0;i<16;i++)
		scanf("%c",&k[i]);

	key=(unsigned char **)malloc(4*sizeof(unsigned char*));	
	for(i=0;i<4;i++)
		key[i]=(unsigned char *)malloc(4*sizeof(unsigned char));

	w=(unsigned char **)malloc(4*sizeof(unsigned char*));	
	for(i=0;i<4;i++)
		w[i]=(unsigned char *)malloc(44*sizeof(unsigned char));

	s=(unsigned char **)malloc(4*sizeof(unsigned char*));	
	for(i=0;i<4;i++)
		s[i]=(unsigned char *)malloc(4*sizeof(unsigned char));	
	k1=0;
	for(j=0;j<4;j++){
		for(i=0;i<4;i++){
			key[i][j]=k[k1];
			k1++;
		}
	}
	KeySchedule(key,w);

	fseek(fp,0,SEEK_END);
	p=ftell(fp);
	q=p/16;
	r=p&15;
	fseek(fp,0,SEEK_SET);

	for(i1=0;i1<q;i1++){
		fread(st,1,16,fp);
		k1=0;
		for(j=0;j<4;j++){
			for(i=0;i<4;i++){
				s[i][j]=st[k1];
					k1++;
			}
		}
		aes(s,w);
		k1=0;
		for(j=0;j<4;j++)
		{
			for(i=0;i<4;i++)
			{
				enc[k1]=s[i][j];
				k1++;
			}
		}
		fwrite(enc,1,16,fp1);
	}
	if(r!=0){
	for(i=0;i<16;i++)
		st[i]=' ';
	fread(st,1,16,fp);
	k1=0;
	for(j=0;j<4;j++){
		for(i=0;i<4;i++){
			s[i][j]=st[k1];
			k1++;
		}
	}
	aes(s,w);
	k1=0;
	for(j=0;j<4;j++)
	{
		for(i=0;i<4;i++)
		{
			enc[k1]=s[i][j];
			k1++;
		}
	}
	fwrite(enc,1,16,fp1);}
	fclose(fp);
	fclose(fp1);
}
