const unsigned int c=0b100011011;

void KeySchedule(unsigned char **key,unsigned char **w);
void ROTWORD(unsigned char *a,unsigned int i);
void Sub_Word(unsigned char *a,unsigned int i);
unsigned char SubBytes(unsigned char a);
unsigned char inv(unsigned char a);
unsigned char mult(unsigned char a,unsigned char b);
unsigned char ele_in_field(unsigned int a);
unsigned int num_bit(unsigned int a);
void aes(unsigned char **s,unsigned char **w);
void shift(unsigned char **s);
void Mix_Column(unsigned char **s);
void Add_Round_Key(unsigned char **s,unsigned char **w,unsigned int k);
void des(unsigned char **s,unsigned char **w);
void inv_Add_Round_Key(unsigned char **s,unsigned char **w,unsigned int k);
void inv_Shift_Rows(unsigned char **s);
unsigned char inv_SubBytes(unsigned char a);
void inv_Mix_Column(unsigned char **s);

void KeySchedule(unsigned char **key,unsigned char **w)
{
	unsigned int i,j,k;
	unsigned char temp[4];
	unsigned int RCON[10];
	RCON[0]=0x01000000,RCON[1]=0x02000000,RCON[2]=0x04000000,RCON[3]=0x08000000,RCON[4]=0x10000000,RCON[5]=0x20000000,RCON[6]=0x40000000,RCON[7]=0x80000000,RCON[8]=0x1b000000,RCON[9]=0x36000000;
	
	for(j=0;j<4;j++)
		for(i=0;i<4;i++)
			w[i][j]=key[i][j];
	
	for(j=4;j<44;j++)
	{
		for(i=0;i<4;i++)
			temp[i]=w[i][j-1];
		if((j%4)==0)
		{
			ROTWORD(temp,4);	
			Sub_Word(temp,4);
			temp[0]=temp[0]^((RCON[(j>>2)-1]>>24)&(0b11111111));
		}
		for(i=0;i<4;i++)
			w[i][j]=w[i][j-4]^temp[i];
	}
}

void ROTWORD(unsigned char *a,unsigned int i)
{
	unsigned char b[i];
	unsigned int j;
	for(j=0;j<i;j++)
		b[j]=a[(j+1)%i];
	for(j=0;j<i;j++)
		a[j]=b[j];
	return;
}

void Sub_Word(unsigned char *a,unsigned int i)
{
	unsigned int j;
	for(j=0;j<i;j++)
		a[j]=SubBytes(a[j]);
	return;
}

unsigned char SubBytes(unsigned char a)
{
	unsigned char z,b,b1=0,c1;
	unsigned int i;

	z=a;
	if(z!=0)
		z=inv(z);
	c1='c';
	a=z;
	for(i=0;i<8;i++){
		b=((a>>i)&1)^((a>>((i+4)%8))&1)^((a>>((i+5)%8))&1)^((a>>((i+6)%8))&1)^((a>>((i+7)%8))&1)^((c1>>i)&1);
		b1 = b1|(b<<i);
	}
	return b1;
}

unsigned char inv(unsigned char a)
{
	unsigned char i;

	for(i=1;i<256;i++)
	{
		if(mult(i,a)==1)
			return i;
	}
}

unsigned char mult(unsigned char a,unsigned char b)
{
	unsigned int i,s1=0;
	for(i=0;b!=0;i++)
	{
		if( (b&1) == 1 )
		{
			s1^=(a<<i);
		}
		b=b>>1;
		
	}
	
	return ele_in_field(s1);
}

unsigned char ele_in_field(unsigned int a)
{
	unsigned int n1,t=num_bit(a);
	
	if(t<9)
		return a;
	else
	{
		n1=t-9;
		a=ele_in_field(a^(c<<n1));
	}
}

unsigned int num_bit(unsigned int a)
{
	unsigned int s=0;
	if(a==0)
		return 1;
	while(a!=0)
	{
		a>>=1;
		s++;
	}
	return s;
}

void aes(unsigned char **s,unsigned char **w)
{
	unsigned int i,j,k;

	for(i=0;i<4;i++)
		for(j=0;j<4;j++)
			s[i][j]=s[i][j]^w[i][j];

	for(k=1;k<=9;k++)
	{
		for(i=0;i<4;i++)
			for(j=0;j<4;j++)
				s[i][j]=SubBytes(s[i][j]);

		shift(s);
		
		Mix_Column(s);

		Add_Round_Key(s,w,k);

	}
	for(i=0;i<4;i++)
		for(j=0;j<4;j++)
			s[i][j]=SubBytes(s[i][j]);

	shift(s);
	
	Add_Round_Key(s,w,10);

}

void shift(unsigned char **s)
{
	unsigned char s1[4][4];
	unsigned int i,j;

	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			s1[i][j]=s[i][(j+i)%4];
		}
	}
	
	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			s[i][j]=s1[i][j];
		}
	}
}

void Mix_Column(unsigned char **s)
{
	unsigned int i,j;
	unsigned char t[4],u[4];
	
	for(j=0;j<4;j++)
	{
		for(i=0;i<4;i++)
		{
			
			t[i]=s[i][j];
		}
		u[0]=(mult(2,t[0]))^(mult(3,t[1]))^(t[2])^(t[3]);
		u[1]=(mult(2,t[1]))^(mult(3,t[2]))^(t[3])^(t[0]);
		u[2]=(mult(2,t[2]))^(mult(3,t[3]))^(t[0])^(t[1]);
		u[3]=(mult(2,t[3]))^(mult(3,t[0]))^(t[1])^(t[2]);
		
		for(i=0;i<4;i++)
		{
			
			s[i][j]=u[i];
		}
	
	}
}

void Add_Round_Key(unsigned char **s,unsigned char **w,unsigned int k)
{
	unsigned int i,j;
	
	for(i=0;i<4;i++)
		for(j=0;j<4;j++)
			s[i][j]=s[i][j]^w[i][(4*k+j)];
	
}

void des(unsigned char **s,unsigned char **w)
{
	unsigned int i,j,k,k1=0;
	unsigned char dec[16];

	inv_Add_Round_Key(s,w,10);
	inv_Shift_Rows(s);

	for(i=0;i<4;i++)
		for(j=0;j<4;j++)
			s[i][j]=inv_SubBytes(s[i][j]);
	
	for(k=1;k<=9;k++)
	{
		inv_Add_Round_Key(s,w,10-k);
		inv_Mix_Column(s);
		inv_Shift_Rows(s);
		for(i=0;i<4;i++)
			for(j=0;j<4;j++)
				s[i][j]=inv_SubBytes(s[i][j]);
	}
	
	inv_Add_Round_Key(s,w,0);

}

void inv_Add_Round_Key(unsigned char **s,unsigned char **w,unsigned int k)
{
	unsigned int i,j;
	
	for(i=0;i<4;i++)
		for(j=0;j<4;j++)
			s[i][j]=s[i][j]^w[i][(4*k+j)];
}

void inv_Shift_Rows(unsigned char **s)
{
	unsigned char s1[4][4];
	unsigned int i,j;

	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			s1[i][(j+i)%4]=s[i][j];
		}
	}
	
	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			s[i][j]=s1[i][j];
		}
	}
}

unsigned char inv_SubBytes(unsigned char a)
{
	unsigned char i,k,b=0;
	unsigned char c1=0x05;
	for(int i=0;i<8;i++)
	{
		k=((a>>((i+2)&7))^(a>>((i+5)&7))^(a>>((i+7)&7))^(c1>>i))&1;
		b|=(k<<i);
	}
	if(b==0)
		return 0;
	else
		return inv(b);
}

void inv_Mix_Column(unsigned char **s)
{
	unsigned int i,j;
	unsigned char t[4],u[4];
	
	for(j=0;j<4;j++){
		for(i=0;i<4;i++)
			t[i]=s[i][j];

		u[0]=(mult(0x0e,t[0]))^(mult(0x0b,t[1]))^(mult(0x0d,t[2]))^(mult(0x09,t[3]));
		u[1]=(mult(0x0e,t[1]))^(mult(0x0b,t[2]))^(mult(0x0d,t[3]))^(mult(0x09,t[0]));
		u[2]=(mult(0x0e,t[2]))^(mult(0x0b,t[3]))^(mult(0x0d,t[0]))^(mult(0x09,t[1]));
		u[3]=(mult(0x0e,t[3]))^(mult(0x0b,t[0]))^(mult(0x0d,t[1]))^(mult(0x09,t[2]));

		for(i=0;i<4;i++)
			s[i][j]=u[i];
	}
}
