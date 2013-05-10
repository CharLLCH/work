#include <Eigen/Eigen>
#include <iostream>
using namespace std;
using namespace Eigen;

MatrixXd testc(double *cc,int x){
    	MatrixXd m(8,8);
	for(int i = 0 ; i < x ;i++){
		m(0,i) = cc[i];
	}
	return m;
}
int main(){
	unsigned char cc[8];
	double ch[8];
	for(int i = 0 ; i < 8 ; i++){
		cc[i] = i+2*i;
		ch[i] = (double)((cc[i]*0.8364)+16);
		cout<<ch[i]<<"  "<<i<<"  ";
	}
	cout<<endl;
	Eigen::MatrixXd  m(8,8);
	m(0,0) = 0.403843;
	Eigen::MatrixXd  n(8,8);
	n(0,0) = 0.2;
	std::cout<<m*n<<std::endl;
	n = testc(ch,3);
	cout<<n<<endl;
	return 0;
}
