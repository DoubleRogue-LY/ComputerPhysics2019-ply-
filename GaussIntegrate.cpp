#include<iostream>
#include<fstream>
#include"test_function.h"

using namespace std;

//����x��n�η�
double power(double x, int n) {
	double res = 1;
	for (int i = 0; i < n; ++i) {
		res = res * x;
	}
	return res;
}



#define N 2  //���ڵڶ��� N=2,���޸ĵõ����ߴεĸ�˹����

//��Ԫ��������Է�����,�������������b[]��
void Gaussian_elimination(double* A[], double* b,int n) {
	//�Խǻ�
	for (int i = 0; i < n; ++i) {
		if (A[i][i] == 0) { cout << "error"; break; }
		for (int j = i + 1; j < n; ++j) {
			double c = -1*A[j][i] / A[i][i];
			for (int k = i + 1; k < n; ++k) {
				A[j][k] = A[j][k] + c * A[i][k];
			}
			b[j] = b[j] + b[i]*c;
		}
	}
	//�ش����x
	for (int i = (n - 1); i >= 0; --i) {
		for (int j = i + 1; j < n; ++j) {
			b[i] = b[i] - A[i][j] * b[j];
		}
		b[i] = b[i] / A[i][i];
	}
}

//�������ʽ��,������ߴ�ϵ��Ϊ1
class polynomial {
public:
	//����n�ζ���ʽ
	polynomial(int m) {
		coefficient = new double[m + 1];
		coefficient[m] = 1;
		n = m;
	}
	~polynomial() { delete[] coefficient; }
	//��ֵ
	void setvalue(double* a) {
		for (int i = 0; i < n; ++i) {
			coefficient[i] = a[i];
		}
	}
	//��ֵf(x)
	double getvalue(double x) {
		double res = 1;
		for (int i = n - 1; i >= 0; --i) {
			res = res * x + coefficient[i];
		}
		return res;
	}
	//������[a,b]�ö��ַ������
	double zero(double a, double b) {
		double fa = getvalue(a);
		while ((b - a) / 2 > (1e-10)) {
			double c = a + (b - a) / 2;
			double temp = getvalue(c);
			if (temp * fa < 0) {
				b = c;
			}
			else {
				a = c;
				fa = temp;
			}
		}
		return (b + a) / 2;
	}

	//��[0,1]�����ڼ����������
	double* zero_point() {
		double* res = new double[n];
		//�ҿ��ܴ�����������
		double** section = new double*[n];//�洢�����������
		for (int i = 0; i < n; ++i) section[i] = new double[2];
		
		int point_num = 0;
		for (int i = 1; point_num < (n-1); i*=2) {
			point_num = 0;
			double step= 1.0/ (i*(n + 1));
			for (int j = 0; j < (i*(n + 1));++j) {
				if (getvalue(j*step)*getvalue((j + 1)*step) < 0) {
					section[point_num][0] = j * step;
					section[point_num][1] = (j+1) * step;
					point_num++;
				}
			}
		}
		for (int i = 0; i < n; ++i) {
			res[i] = zero(section[i][0], section[i][1]);
		}
		return res;
	}


private:
	double* coefficient;
	int n;
};

//���Ժ�������
void Test(double f_x[], double f_A[], double s_x[], double s_A[], double t_x[], double t_A[], double(*f)(double)) {
	double res = f_A[0] * f(f_x[0]) + f_A[1] * f(f_x[1]);
	cout << "��һ������" << res << endl;
	res = 0;
	for (int i = 0; i <= N; ++i) {
		res += s_A[i] * f(s_x[i]);
	}
	cout << "�ڶ�������" << res << endl;
	res = t_A[0] * f(t_x[0]) + t_A[1] * f(t_x[1]);
	cout << "����������" << res << endl;
}

//������
int main() {
//��һ�ⲿ�֣�
	double f_x[2];
	double f_A[2];
	f_x[0] = (35 - 2 * sqrt(70)) / 63.0;
	f_x[1] = (35 + 2 * sqrt(70)) / 63.0;
	f_A[0] = (1 / 3.0) - (sqrt(70) / 150);
	f_A[1] = (1 / 3.0) + (sqrt(70) / 150);
	cout << "��һ�⣺" << endl << "�ڵ�Ϊ��x0=" << f_x[0] << ", x1=" << f_x[1] << endl;
	cout << "ϵ��Ϊ��A0=" << f_A[0] << ", A1=" << f_A[1] << endl;


//�ڶ��ⲿ�֣�
	double* b = new double[N+1];
	double** A=new double*[N+1];
	//��ʼ������
	for (int i = 0; i <= N; ++i) {
		A[i] = new double[N];
		for (int j = 0; j <= N; ++j) {
			A[i][j] = 1 / (i + j + 1.5);
		}
		b[i] = -1 / (N + i + 2.5);
	}
	//����ϵ��an
	Gaussian_elimination(A, b, N+1);
	//�õ���˹�ڵ㣬������zero������
	polynomial p{ N + 1 };
	p.setvalue(b);
	double* zero = p.zero_point();
    //����ڵ�ϵ��An
	for (int i = 0; i <= N; ++i) {
		for (int j = 0; j <= N; ++j) {
			A[i][j] = power(zero[j]*10, i);
		}
		b[i] = (power(10, i)) / (1.5 + i);
	}
	Gaussian_elimination(A, b, N + 1);
	cout << "�ڶ��⣺" << endl << "�ڵ�Ϊ��";
	for (int i = 0; i <= N; ++i) {
		cout << "x" << i << "=" << zero[i];
		if (i != N) cout << ", ";
	}
	cout << endl;
	cout << "ϵ��Ϊ��";
	for (int i = 0; i <= N; ++i) {
		cout << "A" << i << "=" << b[i];
		if (i != N) cout << ", ";
	}
	cout << endl;
//�����ⲿ�֣�
	double t_x[2];
	double t_A[2];
	t_x[0] = sqrt(10) / 5;
	t_x[1] = -sqrt(10) / 5;
	t_A[0] = 4 / 3.0;
	t_A[1] = 4 / 3.0;
	cout << "�����⣺" << endl << "�ڵ�Ϊ��x0=" << t_x[0] << ", x1=" << t_x[1] << endl;
	cout << "ϵ��Ϊ��A0=" << t_A[0] << ", A1=" << t_A[1] << endl;

//Test����(���ɷ��Ӳ���)
	cout << endl << "Test���֣�" << endl;

	cout << "f(x)=x^5 ���֣�" << endl;
	Test(f_x, f_A, zero, b, t_x, t_A, power_5);
	cout << "f(x)=x^6 ���֣�" << endl;
	Test(f_x, f_A, zero, b, t_x, t_A, power_6);
	cout << "f(x)=Sin(x) ���֣�" << endl;
	Test(f_x, f_A, zero, b, t_x, t_A, sin);
	cout << "f(x)=exp(x) ���֣�" << endl;
	Test(f_x, f_A, zero, b, t_x, t_A, exp);
}

