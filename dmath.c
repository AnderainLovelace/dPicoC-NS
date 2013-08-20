const double PI = 3.1415926535897932384626433832795;
const double TWOPI  = 2.0*3.1415926535897932384626433832795;
const int N=100;

double fabs(double x) {
  if (x > 0) return x;
  if (x < 0) return -x;
  return x;
}

double cos(double x) {
  x = x - TWOPI * ((int) (x / TWOPI));
  double x2 = x * x;
  return 1 + x2 * (-.5 + x2 * (.0417 - x2 * 0.00138)); 
}

double sin(double x) {
  x = x - TWOPI * ((int) (x / TWOPI));
  double x2 = x * x;
  return x * (1 + x2 * (-0.1667 + .00833 * (x2 - .0002 * x2 )));
}

double tan(double x)
{
	return sin(x)/cos(x);
}
double coef(int n)
{
	if(n==0) return 0;
	double t=1.0/n;
	if(n%2==0) t=-t;
	return t;
}//x^n的系数
double horner(double x)
{
	double u=coef(N);
	int i;
	for(i=N-1;i>=0;i--)
		u=u*x+coef(i);
	return u;
}
double sqrt(double b)
{
	double x=1;int step=0;
	while((x*x-b<-0.000000000000001||x*x-b>0.000000000000001)&&step<50)
	{x=(b/x+x)/2.0;step++;}
	return x;
}//开平方

double ln(double x)//ln(1+x)=x-x^2/2+x^3/3-x^4/4…… 
{
	if(x>1.5)
	{
		int i;
		for(i=0;x>1.25;i++)
			x=sqrt(x);
		return (1<<i)*horner(x-1);
	}
	else if(x<0.7&&x>0)
	{
		int i;
		for(i=0;x<0.7;i++)
		x=sqrt(x);
		return (1<<i)*horner(x-1);
	}
	else if(x>0)
		return horner(x-1);
}

double log(double m,double base)
{
	return ln(m)/ln(base);//换底公式
}

double exp(double x)
{
	double sum=1;
	int i;
	for(i=N;i>0;i--)
	{ 
		sum/=i;
		sum*=x;
		sum+=1;
	}
	return sum;
}//e^x≈1+x+x^2/2!+x^3/3!+……+x^n/n!

double pow(double m,double n)
{
	return exp(n*ln(m));
}
