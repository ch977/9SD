#include <iostream>
using namespace std;
template<typename T>  //模板声明，其中T为类型参数
T max(T a,T b,T c) //定义一个通用函数，用T作虚拟的类型名
{
	if(b>a) a=b;
	if(c>a) a=c;
	return a;
}

int main( )
{
	int i1=185,i2=-76,i3=567,i;
	double d1=56.87,d2=90.23,d3=-3214.78,d;
	long g1=67854,g2=-912456,g3=673456,g;
	i=max(i1,i2,i3); //调用模板函数，此时T被int取代
	d=max(d1,d2,d3); //调用模板函数，此时T被double取代
	g=max(g1,g2,g3); //调用模板函数，此时T被long取代
	cout<<"i_max="<<i<<endl;
	cout<<"f_max="<<d<<endl;
	cout<<"g_max="<<g<<endl;
	return 0;
}

