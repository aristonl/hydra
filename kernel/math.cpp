int pow(int x, unsigned int y) {
    int res = x;
    for (int i=0;i<y-1;i++) {
        res*=x;
    }
    return res;
}

double sin(double x) {
    double x2 = x*x;
    double x4 = x2*x2;
    double t1 = x * (1.0 - x2 / (2*3));
    double x5 = x * x4;
    double t2 = x5 * (1.0 - x2 / (6*7)) / (1.0* 2*3*4*5);
    double x9 = x5 * x4;
    double t3 = x9 * (1.0 - x2 / (10*11)) / (1.0* 2*3*4*5*6*7*8*9);
    double x13 = x9 * x4;
    double t4 = x13 * (1.0 - x2 / (14*15)) / (1.0* 2*3*4*5*6*7*8*9*10*11*12*13);
    double result = t4;
    result += t3;
    result += t2;
    result += t1;

    return result;
}

double factorial(double n)// not needed by SineN2, CosineN2 functions
{
	if (n<=1.0)
		return(1.0);
	else
		n=n*factorial(n-1.0); // notice we just keep calling this over until we get to 1!
	return(n);
}

double sin2(const double& x, const int& n )// replaces first
{
	double sum = 0.0;

	if(n > 0)	
		for(int j=0; j<n; j++)
		{	// successive terms are to alternate sign
			if(j%2)// j is odd
				sum -= pow(x,(double)(2*j+1))/factorial((double)(2*j+1));
			else// j is even
				sum += pow(x,(double)(2*j+1))/factorial((double)(2*j+1));
		}

	return sum;
}// end of SineN()

// this method uses the recursion relation for the series
// to generate the next coefficient from the last.
// Advantages: 1) No need to find n! for each term.
// 2) No need to call pow(x,n) each term.
double sin3(const double& x, const int& n, double* p_err )
{
	double a = 1.0*x;// 1st term
	double sum = 0.0;

	if(n > 0)
	{
		for(int j=1; j<=n; j++)// for sine
		{
			sum += a;
			a *= -1.0*x*x/( (2*j+1)*(2*j) );// next term from last
		//	sum += a;
		}
		*p_err = a;// max. error = 1st term not taken for alternating series
	}

	return sum;
}// end of SineN2()

double cos(const double& x, const int& n )// replaces second
{
	double sum = 1.0;

	if(n > 0)	
		for(int j=1; j<n; j++)
		{	
			if(j%2)// j is odd
				sum -= pow(x,(double)(2*j))/factorial((double)(2*j));
			else// j is even
				sum += pow(x,(double)(2*j))/factorial((double)(2*j));
		}

	return sum;
}// end of CosineN()

double cos2(const double& x, const int& n, double* p_err )
{
	double a = 1.0;// 1st term
	double sum = 0.0;

	if(n > 0)
	{
		for(int j=0; j<n; j++)// for cosine
		{
			sum += a;
			a *= -1.0*x*x/( (2*j+1)*(2*j+2) );// next term from last
		//	sum += a;
		}
		*p_err = a;// max. error = 1st term not taken for alternating series
	}

	return sum;
}