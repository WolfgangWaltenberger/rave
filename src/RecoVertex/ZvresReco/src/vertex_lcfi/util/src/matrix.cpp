#include "../inc/matrix.h"


//fg: boost headers should be included here in user code
#include <boost/numeric/ublas/operation.hpp>
#include <boost/numeric/ublas/vector_proxy.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/triangular.hpp>


namespace vertex_lcfi
{
namespace util
{
/* Matrix inversion routine.- only 3x3 for now - make sure input type is not symetric
    Uses lu_factorize and lu_substitute in uBLAS to invert a matrix */
//template<class T>
SymMatrix5x5 InvertMatrix5x5(SymMatrix5x5 input)
{
	using namespace boost::numeric::ublas;
 	// create a working copy of the input
	matrix<double>  A(input);
	//std::cout << "A1: " << A << std::endl;
	// perform LU-factorization
	lu_factorize(A);
	//std::cout << "A2: " << A << std::endl;
	
 	// create identity matrix of "inverse"
	matrix<double>  B(5,5);
	B.clear();
	for (unsigned int i = 0; i < A.size1(); i++)
		B(i,i) = 1;
	//std::cout << "B1: " << B << std::endl;
	// backsubstitute to get the inverse
	lu_substitute<const matrix<double>,matrix<double> >(A,B);//const matrix<T>, matrix<T> >(A, inverse);
	//std::cout << "B2: " << B << std::endl;
	return B;
}

double determinant(Matrix3x3 a)
{
  double det = -a(0,2)*a(1,1)*a(2,0) + a(0,1)*a(1,2)*a(2,0) + a(0,2)*a(1,0)*a(2,1) - a(0,0)*a(1,2)*a(2,1) - a(0,1)*a(1,0)*a(2,2) + a(0,0)*a(1,1)*a(2,2);
  
  return det;
}

Matrix3x3 InvertMatrix(Matrix3x3 a)//const ublas::matrix<T>& input, ublas::matrix<T>& inverse) 
{
	using namespace boost::numeric::ublas;
 	//std::cout.precision(100);
	//std::cout << a << std::endl;
	//a = a * (10000.0*10000.0); //Convert to Microns to avoid computational inaccuracy
	//std::cout << a << std::endl;
	//double det = a(0,0)*((a(1,1)*a(2,2))-(a(2,1)*a(1,2))) + a(0,1)*((a(1,2)*a(2,0))-(a(2,2)*a(1,0))) + a(0,2)*((a(1,0)*a(2,1))-(a(2,0)*a(1,1)));

	double det = determinant( a );
	
	//std::cout << -a(0,2)*a(1,1)*a(2,0) << " " << a(0,1)*a(1,2)*a(2,0)  << " " <<  a(0,2)*a(1,0)*a(2,1)  << " " <<  a(0,0)*a(1,2)*a(2,1)  << " " <<  a(0,1)*a(1,0)*a(2,2)  << " " <<  a(0,0)*a(1,1)*a(2,2) << std::endl;
	//std::cout << det << std::endl;
	Matrix3x3 inverse;
	
	for (short j=0;j<3;j++) 
	{
		for (short i=0;i<3;i++) 
		{

			//Form the adjoint a_ij */
         		matrix<double> c(2,2);
			short i1 = 0;
			for (short ii=0;ii<3;ii++) 
				{
					if (ii == i)
						continue;
					short j1 = 0;
					for (short jj=0;jj<3;jj++) 
					{
						if (jj == j)
							continue;
						c(i1,j1) = a(ii,jj);
						j1++;
					}
					i1++;
				}

				/* Calculate the determinate */
				double tempdet = (c(0,0)*c(1,1)) - (c(1,0)*c(0,1));
				/* Fill in the elements of the cofactor */
				inverse(j,i) = (pow(-1.0,i+j+2.0) * tempdet)/det; //Note inline transposition
		}
	}
	//std::cout << inverse <<std::endl<<std::endl;
	//double f;
	//std::cin >> f;
	//inverse = inverse * (10000*10000);//Convert back to cm
	//std::cout << inverse <<std::endl<<std::endl;
	return inverse;
	
	/*// create a working copy of the input
	matrix<double>  A(input);
	//std::cout << "A1: " << A << std::endl;
	// perform LU-factorization
	lu_factorize(A);
	//std::cout << "A2: " << A << std::endl;
	
 	// create identity matrix of "inverse"
	matrix<double>  B(3,3);
	B.clear();
	for (unsigned int i = 0; i < A.size1(); i++)
		B(i,i) = 1;
	//std::cout << "B1: " << B << std::endl;
	// backsubstitute to get the inverse
	lu_substitute<const matrix<double>,matrix<double> >(A,B);//const matrix<T>, matrix<T> >(A, inverse);
	//std::cout << "B2: " << B << std::endl;
	return B;
	*/
}

Matrix2x2 InvertMatrix2(Matrix2x2 a)//const ublas::matrix<T>& input, ublas::matrix<T>& inverse) 
{
	Matrix2x2 inverse;
	double det = (a(0,0)*a(1,1))-(a(0,1)*a(1,0));
	inverse(0,0) = a(1,1)/det;
	inverse(0,1) = -a(0,1)/det;
	inverse(1,0) = -a(1,0)/det;
	inverse(1,1) = a(0,0)/det;
	return inverse;
	
}
}
}

