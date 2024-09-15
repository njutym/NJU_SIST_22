 // your code here
#include <iostream>
using namespace std;

class Vector
{
	friend class Matrix;
    int p_data[105];
    int num;
public:
    Vector():num(0){}
    int &operator[](int i)
    {
        return p_data[num+i];
    }
    int operator[](int i) const
    {	
        return p_data[num+i];
    }
};

 class Matrix
{
    friend class Vector;
    int l;
    int r;
    Vector pdata;
public:
	Matrix()
	{
		l = r = 0;
	}
    Matrix(int i,int j):l(i),r(j){}
    Matrix& operator = (const Matrix& m)
    {
    	l = m.l;
    	r = m.r;
        for(int i = 0;i < l*r;i++)
        {
            pdata.p_data[i] = m.pdata.p_data[i];
        }
    }

    Vector &operator [](int i)
    {
        pdata.num = i*r;
        return pdata;
    }
    
    int &operator ()(int i,int j)
	{
		return pdata.p_data[i*r+j]; 
	}

  
};
 
 int main(){
    Matrix m1(3,4);
    int i, j;
    for(i = 0; i < 3; ++i){
        for(j = 0; j < 4; j++){
            m1[i][j] = i * 4 + j;
        }
    }
    for(i = 0; i < 3; ++i){
        for(j = 0; j < 4; j++){
            cout << m1(i,j) << ", ";
        }
        cout << endl;
    }
    cout << "next" << endl;
    Matrix m2;
    m2 = m1;
    for(i = 0; i < 3; ++i){
        for(j = 0; j < 4; j++){
            cout << m2[i][j] << ",";
            m2[i][j] += 1;
        }
        cout << endl;
    }
    for(i = 0; i < 3; ++i){
        for(j = 0; j < 4; j++){
            cout << m1(i,j) << ", ";
        }
        cout << endl;
    }
    for(i = 0; i < 3; ++i){
        for(j = 0; j < 4; j++){
            cout << m2(i,j) << ", ";
        }
        cout << endl;
    }
    return 0;
}

/*

0,1,2,3,
4,5,6,7,
8,9,10,11,
next
0,1,2,3,
4,5,6,7,
8,9,10,11,
next
0,1,2,3,
4,5,6,7,
8,9,10,11,
next
1,2,3,4,
5,6,7,8,
9,10,11,12,

*/
