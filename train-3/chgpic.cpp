#include "picread.h"

void Xprod(BYTE k[16],double pk[16]){
	for(int i = 0;i < 16;i++){
	    pk[i] = ((int)k[i]+0.1)/256;
	}
}

double f(double x){
	double y;
	//y = 8*x*x*x*x-8*x*x+1;
	y = 8*pow(x,4)-8*pow(x,2)+1;
	return y;	
}

void NKProd(double pk[16],double nk[16],int times){
	for(int i = 0;i < times;i++){
		for(int j = 0;j < 16;j++){
			nk[j] = (1 - e) * f(pk[j]) + e * f(pk[(j+1)%16]);
		}
		for(int k = 0;k<16;k++)
		    pk[k] = nk[k];
	}
}

void RtoY(BYTE *BK,double *YUV,int eachtime){
    	int i = 0;
	while(i<eachtime){
		BYTE r,g,b;
		r = BK[i+0];
		g = BK[i+1];
		b = BK[i+2];
		YUV[i+0] = (double)(0.299*r+0.587*g+0.114*b);
		YUV[i+1] = (double)(-0.1687*r-0.3313*g+0.5*b+128);
		YUV[i+2] = (double)(0.5*r-0.418*g-0.0813*b+128);
		i = i+3;	
	}
}

void YtoR(double *YUV,BYTE *BK,int eachtime){
	int i = 0;
	while(i<eachtime){
		double y,u,v;
		BYTE r,g,b;
		y = YUV[i+0];
		u = YUV[i+1];
		v = YUV[i+2];
		r = (unsigned char)(y+1.402*(v-128));
		g = (unsigned char)(y-0.34414*(u-128)-0.71414*(v-128));
		b = (unsigned char)(y+1.772*(u-128));
		if(r<0)
		    r = 0;
		if(r>255)
		    r = 255;
		if(g<0)
		    g = 0;
		if(g>255)
		    g = 255;
		if(b<0)
		    b = 0;
		if(b>255)
		    b = 255;
		BK[i+0] = r;
		BK[i+1] = g;
		BK[i+2] = b;
 		i += 3;
	}
}

void rlProd(double pk[16],double nk[16],double *LR,int eachtime){
	for(int i=0;i<eachtime/16;i++){
		for(int j=0;j<16;j++){
			nk[j] = (1 - e) * f(pk[j]) + e * f(pk[(j+1)%16]); 
		}
		for(int k=0;k<16;k++){
			LR[16*i+k] = nk[k]/3;
			pk[k] = nk[k];
		}
	}
}

void LProd(double *rla,double *L){
    	for(int k=0;k<3;k++){
		for(int i=0;i<8;i++){
			for(int j=0;j<8;j++){
				if(i==0){//这里老是为0
					L[64*k+j] = (1/(2*sqrt(2)))*cos(pi*((2*j+1)*rla[64*k+j])/16);
				}else{
					L[64*k+8*i+j] = 0.5*cos(pi*((2*j+1)*(i+rla[64*k+8*i+j]))/16);
				}
			}
		}
	}
}
void RProd(double *rlb,double *R){
    	for(int k=0;k<3;k++){
		for(int i=0;i<8;i++){
			for(int j=0;j<8;j++){
				if(j==0){
					R[64*k+8*i+j] = (1/(2*sqrt(2)))*cos(pi*((2*i+1)*rlb[64*k+8*i+j])/16);
				}else{
					R[64*k+8*i+j] = 0.5*cos(pi*((2*i+1)*(j+rlb[64*k+8*i+j]))/16);
				}
			}
		}
	}
}

void zsearch(MatrixXi x,BYTE sl[64]){
	int m,n;
	int cp,fx;
	int d;
	m=0,n=0,fx=0;
	d = x(0,0);
	sl[0] = d;	
	for(int k=0;k<8;k++){
		for(int i=0;i<k;i++){
			if(fx)
			    	m--,n++;
			else
				m++,n--;
		    	d = x(m,n);
		    	sl[m*8+n] = d;
		}
		if(m==0)
		    n++,fx = 0;
		if(n==0)
		    fx = 1,m++;
		if(m<8)
		d = x(m,n),sl[8*m+n] = d;
	}
	d = x(7,1);
	sl[8*7+1] = d;
       	m = 7,n = 1;
	fx = 1;
	for(int k = 7;k > 1;k--){
		for(int i = 0;i < k-1;i++){
			if(fx)
			    	m--,n++;
			else
			    	m++,n--;
			d = x(m,n);
			sl[8*m+n] = d;
		}
		if(m==7)
			n++,fx = 1;
		if(n==7)
			m++,fx = 0;
		if(m<8){
			d = x(m,n);
			sl[8*m+n] = d;
		}else{
			m=7,n=7;
			d = x(m,n);
			sl[8*m+n] = d;
		}
	}


}

int main( int argc, char **argv )  
{  
	bmpreader pict(argv[1]);//实例一个bmp读取类，获得相应的信息头和像素数据，并根据不同大小的像素位进行填充图像
	BYTE KeyInput[16];//输入的密钥，传送走的密钥
	double pk[16];//迭代前的各个盒子值
	double nk[16];//迭代后的各个盒子值
	int picH = pict.picH;
	int picW = pict.picW;//原始图的高宽
	int picS = pict.csize;//填充后的整体大小
	int bnum = pict.Bnum;//分块数
	int eachtime = picS/bnum;
	printf("each time = %d Num = %d\n",eachtime,bnum);
	//BYTE RL[eachtime];//每次的随机序列
	//RL 得变成double的了。。方便运算么
	cout<<"Please input a Keyline...(16byte-128bit)"<<endl;
	cout<<"1234567890123456<-Here the ending is.."<<endl;
	cin>>KeyInput;
	//获得x初始盒子，迭代times次，消除。。
	Xprod(KeyInput,pk);
	NKProd(pk,nk,500);
	//然后产生随机序列并映射到（-0.5，0.5）直接利用混沌盒子的值！？yes!!(8x4-8x2+1)~(-1,1)的，直接处理出来就好了
	BYTE BK[eachtime];
	double YUV[eachtime];
	double rla[eachtime];
	double rlb[eachtime];
	double L[eachtime];
	double R[eachtime];
	BYTE CK[eachtime];
	BYTE cky[64];
	BYTE cku[64];
	BYTE ckv[64];
	MatrixXd Lym(8,8);
	MatrixXd Rym(8,8);
	MatrixXd Lum(8,8);
	MatrixXd Rum(8,8);
	MatrixXd Lvm(8,8);
	MatrixXd Rvm(8,8);
	MatrixXd Xym(8,8);
	MatrixXd Xum(8,8);
	MatrixXd Xvm(8,8);
	MatrixXd Yym(8,8);
	MatrixXd Yum(8,8);
	MatrixXd Yvm(8,8);
	MatrixXi Yyi(8,8);
	MatrixXi Yui(8,8);
	MatrixXi Yvi(8,8);
	MatrixXd Lfm(8,8);
	MatrixXd Cfm(8,8);
	Lfm << 16,11,10,16,24,40,51,61,
	       12,12,14,19,26,58,60,55,
	       14,13,16,24,40,57,69,56,
	       14,17,22,29,51,87,80,62,
	       18,22,37,56,68,109,103,77,
	       24,35,55,64,81,104,113,92,
	       49,64,78,87,103,121,120,101,
	       72,92,95,98,112,100,103,99;
	Cfm << 17,18,24,47,99,99,99,99,
	       18,21,26,66,99,99,99,99,
	       24,26,56,99,99,99,99,99,
	       47,66,99,99,99,99,99,99,
	       99,99,99,99,99,99,99,99,
	       99,99,99,99,99,99,99,99,	
	       99,99,99,99,99,99,99,99,
	       99,99,99,99,99,99,99,99;
	MatrixXd Lyinver(8,8),Luinver(8,8),Lvinver(8,8);
	MatrixXd Ryinver(8,8),Ruinver(8,8),Rvinver(8,8);
	FILE *flfp = fopen("fl.bmp","wb");
	pict.info.biSizeImage = 3*picH*picW;
	pict.head.bfSize = pict.info.biSizeImage + 54;
	fwrite(&pict.head,1,14,flfp);
	fwrite(&pict.info,1,40,flfp);

	BYTE ckb[picS];
	int ckn = 0;
	BYTE zma = 0;
	for(int k = 0 ; k < bnum ; k++){
		memcpy(BK,pict.PicS+k*eachtime,eachtime);
		//RGB to YUV
		RtoY(BK,YUV,eachtime);
		//一次产生a b 以计算出L & R
		rlProd(pk,nk,rla,eachtime);
		rlProd(pk,nk,rlb,eachtime);
		//L&R 
		LProd(rla,L);
		RProd(rlb,R);
		for(int i=0;i<8;i++){
		    for(int j=0;j<8;j++){
			//DCT接受的数值是-128-127，所以要-128！？
		    	Xym(i,j) = YUV[3*(8*i+j)+0]-128;
			Xum(i,j) = YUV[3*(8*i+j)+1]-128;
			Xvm(i,j) = YUV[3*(8*i+j)+2]-128;
			Lym(i,j) = L[3*(8*i+j)+0];
			Lum(i,j) = L[3*(8*i+j)+1];
			Lvm(i,j) = L[3*(8*i+j)+2];
			Rym(i,j) = R[3*(8*i+j)+0];
			Rum(i,j) = R[3*(8*i+j)+1];
			Rvm(i,j) = R[3*(8*i+j)+2];
		    }
		}//得到YUV三个分量的X矩阵,3个L&R
		Yym = Lym*Xym*Rym;
		Yum = Lum*Xum*Rum;
		Yvm = Lvm*Xvm*Rvm;
		if(k%500==0)
		    cout<<Xym<<endl<<"============"<<endl;
		//量化
		/*MatrixXd l(8,8),r(8,8);
		l = Lym.inverse();
		r = Rym.inverse();
		Xym = l*Yym*r;
		if(k%500==0)
		    cout<<Xym<<endl<<"------------"<<endl;*/
		for(int c=0;c<8;c++){
			for(int d=0;d<8;d++){
				Yyi(c,d) = (int)(Yym(c,d)/Lfm(c,d));
				Yui(c,d) = (int)(Yum(c,d)/Lfm(c,d));
				Yvi(c,d) = (int)(Yvm(c,d)/Lfm(c,d));
			}
		}
		zsearch(Yyi,cky);
		zsearch(Yui,cku);
		zsearch(Yvi,ckv);
		for(int i=0;i<eachtime/3;i++){
			CK[i] = cky[i];
			CK[64+i] = cku[i];
			CK[128+i] = ckv[i];
		}
		for(int i=0;i<eachtime;i++){
			if(CK[i]!=0){
				ckb[ckn++] = CK[i];
			}else{
				if((i==eachtime-1)|(CK[i+1]!=0)){
					if(zma==0){
						ckb[ckn++] = CK[i];
					}else{
						ckb[ckn++] = 0x88;
						ckb[ckn++] = zma+1;
						zma = 0;
					}
				}else{
					zma++;
				}
			}
		}

		/*for(int c=0;c<8;c++){
			for(int d=0;d<8;d++){
				Yym(c,d) = Yyi(c,d)*Lfm(c,d);
				Yum(c,d) = Yui(c,d)*Cfm(c,d);
				Yvm(c,d) = Yvi(c,d)*Cfm(c,d);
			}
		}*/
		Lyinver = Lym.inverse();
		Luinver = Lum.inverse();
		Lvinver = Lvm.inverse();
		Ryinver = Rym.inverse();
		Ruinver = Rum.inverse();
		Rvinver = Rvm.inverse();
		Xym = Lyinver*Yym*Ryinver;
		Xum = Luinver*Yum*Ruinver;
		Xvm = Lvinver*Yvm*Rvinver;
		int td=0;	
		for(int i=0;i<8;i++){
			for(int j=0;j<8;j++){
				td = Xym(i,j);
				YUV[3*(8*i+j)+0] = td+128;
				td = Xum(i,j);
				YUV[3*(8*i+j)+1] = td+128;
				td = Xvm(i,j);
				YUV[3*(8*i+j)+2] = td+128;
			}
		}
		YtoR(YUV,BK,eachtime);
		fwrite(BK,1,eachtime,flfp);
	}
	fclose(flfp);
	//cout<<ckn<<endl;
	//cout<<picS<<endl;
	pict.head.bfSize = picS;
	pict.info.biSizeImage = ckn;
	FILE *bfp = fopen("packpic.bmp","wb");
	fwrite(&pict.head,1,14,bfp);
	fwrite(&pict.info,1,40,bfp);
	fwrite(ckb,1,ckn,bfp);
	fclose(bfp);
	return 0;  
}
