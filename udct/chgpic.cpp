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
		YUV[i+0] = (double)((r*0.256789 + g*0.504129 + b*0.097906) + 16);
		YUV[i+1] = (double)((r*(-0.148223) + g*(-0.290992) + b*0.439215) + 128);
		YUV[i+2] = (double)((r*0.439215 + g*(-0.367789) + b*(-0.071426)) + 128);
		i = i+3;	
	}
}

void rlProd(double pk[16],double nk[16],double *LR,int eachtime){
	for(int i=0;i<eachtime/16;i++){
		for(int j=0;j<16;j++){
			nk[j] = (1 - e) * f(pk[j]) + e * f(pk[(j+1)%16]); 
		}
		for(int k=0;k<16;k++){
			LR[16*i+k] = nk[k]/2;
			if((nk[k]/2 <-0.5)|(nk[k]/2)>0.5)
			    cout<<"有超范围的映射"<<endl;
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
					R[64*k+8*i+j] = (1/(2*sqrt(2)))*cos(pi*((2*j+1)*rlb[64*k+j])/16);
				}else{
					R[64*k+8*i+j] = 0.5*cos(pi*((2*j+1)*(i+rlb[64*k+8*i+j]))/16);
				}
			}
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
		    	Xym(i,j) = YUV[3*(8*i+j)+0];
			Xum(i,j) = YUV[3*(8*i+j)+1];
			Xvm(i,j) = YUV[3*(8*i+j)+2];
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
		/*if(k%100 == 0){
		    	cout<<Lym<<endl<<Lum<<endl<<Lvm<<endl<<Rym<<endl<<Rum<<endl<<Rvm<<endl;
			cout<<k<<"::"<<endl<<"yym:"<<endl<<Yym<<endl<<"yum:"<<endl<<Yum<<endl<<"yvm:"<<endl<<Yvm<<endl;
		}*/

	}
	
	return 0;  
}
