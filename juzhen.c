#include<stdio.h> 
#define M 20 
#define N 20 
float  A[M][N]; //定义了含有400个浮点型数组元素的二维数组
float  B[M][N]; 
float  C[M][N]; 

int i,j,m,n,p,q,k; 


int main() 
{ 
	
    printf("           ###########################################\n"); 
	printf("           ###     欢迎您使用矩阵计算器系统。      ###\n"); 
	printf("           ###     系统功能:                       ###\n"); 
	printf("           ###      计算两个矩阵相加,相减;         ###\n"); 
	printf("           ###        矩阵相乘；矩阵转置;          ###\n"); 
	printf("           ###                                     ###\n"); 
	printf("           #########################################\n\n"); 
	
	int x;
	
	do{
		
		printf("请选择您需要的运算，若退出则选择0后按回车键结束\n"); 
		printf("******************************************************************\n"); 
		printf("0，退出    \n"); 
		printf("1，矩阵相加\n"); 
		printf("2，矩阵相减\n"); 
		printf("3，矩阵相乘\n"); 
		printf("4，矩阵转置\n"); 
		printf("******************************************************************\n"); 
		
		scanf("%d",&x); 
		
		switch (x) 
		{ 
			
		case 0:
			printf("谢谢您使用该系统！");
			break; //退出系统
			
		case 1: //选择加法运算
			{ 
				printf("请输入矩阵A的行数和列数(用逗号隔开):"); 
				scanf("%d,%d",&i,&j); 
				printf("请输入矩阵B的行数和列数(用逗号隔开):") ; 
				scanf("%d,%d",&m,&n); 
				
				while(i!=m||j!=n)//判断是否满足加法运算条件：既两个矩阵的行、列数是否相等 
				{
					printf("***对不起，您输入的两个矩阵不能相加，两个矩阵的行列数必须相等，请重新输入.***\n\n");
					printf("请输入矩阵B的行数和列数(用逗号隔开):") ; 
					scanf("%d,%d",&m,&n); 
				}
				
				printf("请输入矩阵A:\n");//输入矩阵A的元素 
				for(p=0;p<i;p++) 
					for(q=0;q<j;q++) 
						scanf("%f",&A[p][q]); 
					
					printf("输出矩阵A：\n"); //输出矩阵A
					for(p=0;p<i;p++) 
						for(q=0;q<j;q++) 
						{  
							printf("%10.2f",A[p][q]); 
							if((q+1)%j==0) 
								printf("\n");    
						} 
						
						printf("请输入矩阵B：\n"); //输入矩阵B的元素
						for(p=0;p<i;p++) 
							for(q=0;q<j;q++) 
								scanf("%f",&B[p][q]); 
							
							printf("输出矩阵B：\n");//输出矩阵B 
							for(p=0;p<i;p++) 
								for(q=0;q<j;q++) 
								{  
									printf("%10.2f",B[p][q]); 
									if((q+1)%j==0) 
										printf("\n");    
								} 
								printf("矩阵A+矩阵B为:\n"); //进行两个矩阵相加计算 
								for(p=0;p<i;p++) 
									for(q=0;q<j;q++) 
										C[p][q]=A[p][q]+B[p][q]; 
									for(p=0;p<i;p++) 
										for(q=0;q<j;q++) 
										{ 
											printf("%10.2f",C[p][q]); 
											if((q+1)%j==0) 
												printf("\n"); 
										} 
			};break;  
			
		case 2: //选择减法运算
			{ 
				printf("请输入矩阵A的行数和列数(用逗号隔开):"); 
				scanf("%d,%d",&i,&j); 
				printf("请输入矩阵B的行数和列数(用逗号隔开):") ; 
				scanf("%d,%d",&m,&n); 
				
				while(i!=m||j!=n) //判定两个矩阵想减的运算条件：矩阵A和B必须同行数同列数
				{
					printf("***对不起，您输入的两个矩阵不能相减，请重试.***\n\n"); 
					printf("请输入矩阵B的行数和列数(用逗号隔开):") ; 
					scanf("%d,%d",&m,&n); 
				}
				
				printf("请输入矩阵A:\n");//输入矩阵A的元素 
				for(p=0;p<i;p++) 
					for(q=0;q<j;q++) 
						scanf("%f",&A[p][q]); 
					printf("输出矩阵A:\n");//输出矩阵A 
					for(p=0;p<i;p++) 
						for(q=0;q<j;q++) 
						{  
							printf("%10.2f",A[p][q]); 
							if((q+1)%j==0) 
								printf("\n");    
						} 
						
						printf("请输入矩阵B：\n");//输入矩阵B的元素 
						for(p=0;p<i;p++) 
							for(q=0;q<j;q++) 
								scanf("%f",&B[p][q]); 
							printf("输出第矩阵B:\n");//输出矩阵B
							for(p=0;p<i;p++) 
								for(q=0;q<j;q++) 
								{  
									printf("%10.2f",B[p][q]); 
									if((q+1)%j==0) 
										printf("\n");    
								} 
								printf("矩阵A-矩阵B为：\n"); //计算两个矩阵相减 
								for(p=0;p<i;p++) 
									for(q=0;q<j;q++) 
										C[p][q]=A[p][q]-B[p][q]; 
									for(p=0;p<i;p++) 
										for(q=0;q<j;q++) 
										{ 
											printf("%10.2f",C[p][q]); 
											if((q+1)%j==0) 
												printf("\n"); 
										} 
			};break;  
			
		case 3://选择乘法运算 
			{ 
				printf("请输入矩阵A的行数和列数(用逗号隔开):"); 
				scanf("%d,%d",&i,&j); 
				printf("请输入矩阵B的行数和列数(用逗号隔开):") ; 
				scanf("%d,%d",&m,&n); 
				
				while(j!=m)//判定两个矩阵能否相乘：A的列数必须等于B的行数
				{ 
					printf("***对不起，您输入的两个矩阵不能相乘，B矩阵的行数必须和A矩阵的列数相等，请重试.***\n");
					printf("请重新输入矩阵B的行数和列数(用逗号隔开):\n") ; 
					scanf("%d,%d",&m,&n);
				}
				
				printf("请输入矩阵A:\n");//输入矩阵A的元素 
				for(p=0;p<i;p++) 
					for(q=0;q<j;q++) 
						scanf("%f",&A[p][q]); 
					printf("输出矩阵A：\n");//输出矩阵A
					
					for(p=0;p<i;p++) 
						for(q=0;q<j;q++) 
						{  
							printf("%10.2f",A[p][q]); 
							if((q+1)%j==0) 
								printf("\n");    
						} 
						printf("请输入矩阵B：\n");//输入矩阵B的元素 
						for(p=0;p<i;p++) 
							for(q=0;q<j;q++) 
								scanf("%f",&B[p][q]); 
							
							printf("输出矩阵B：\n");//输出矩阵B 
							for(p=0;p<i;p++) 
								for(q=0;q<j;q++) 
								{  
									printf("%10.2f",B[p][q]); 
									if((q+1)%j==0) 
										printf("\n");    
								} 
								printf("矩阵A*矩阵B为:\n");     //计算两个矩阵相乘
								for(p=0;p<i;p++) 
									for(q=0;q<n;q++)
									{
										C[p][q] = 0;
										for( k = 0; k < j; k++ )
											C[p][q]+=A[p][k]*B[k][q]; 
									}
									
									for(p=0;p<i;p++) 
										for(q=0;q<n;q++) 
										{ 
											printf("%10.2f",C[p][q]); 
											if((q+1)%n==0) 
												printf("\n"); 
										} 
			};break;  
			
		case 4://矩阵转置 
			{ 
				printf("请输入矩阵A的行数和列数(用逗号隔开):"); 
				scanf("%d,%d",&i,&j); 
				printf("请输入矩阵A:\n");//输入矩阵A的元素
				for(p=0;p<i;p++) 
					for(q=0;q<j;q++) 
						scanf("%f",&A[p][q]); 
					printf("输出矩阵A\n"); //输出矩阵A
					for(p=0;p<i;p++) 
						for(q=0;q<j;q++) 
						{  
							printf("%10.2f",A[p][q]); 
							if((q+1)%j==0) 
								printf("\n");    
						} 
						for(p=0;p<i;p++)                    //进行转置 
							for(q=0;q<j;q++) 
								B[q][p]=A[p][q]; 
							printf("输出矩阵A的转置矩阵:\n"); 
							for(p=0;p<j;p++) 
								for(q=0;q<i;q++) 
								{  
									printf("%10.2f",B[p][q]); 
									if((q+1)%i==0) 
										printf("\n"); 
								} 
			} 
			
			;break; 
		default:
			printf("error\n");
			break;
 }
}
while(x);//当选择0时程序结束

return 0;
}
