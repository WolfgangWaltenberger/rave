#ifndef MATRIX_H
#define MATRIX_H

#ifndef NDEBUG
#define NDEBUG
#endif

#include <boost/version.hpp> 	 
 #if BOOST_VERSION>=103300 	 
 #include <boost/numeric/ublas/detail/config.hpp> 	
 #else 	 
 #include <boost/numeric/ublas/config.hpp> 	 
 #endif
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_expression.hpp>
#include <boost/numeric/ublas/symmetric.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/lu.hpp>

#include <cmath>
#include <complex>
#include <limits>


namespace vertex_lcfi
{
namespace util
{
	class SymMatrix2x2 : public boost::numeric::ublas::symmetric_matrix<double,boost::numeric::ublas::lower,boost::numeric::ublas::row_major,boost::numeric::ublas::bounded_array<double,3> >
    {
        typedef boost::numeric::ublas::symmetric_matrix<double,boost::numeric::ublas::lower,boost::numeric::ublas::row_major,boost::numeric::ublas::bounded_array<double,3> > Base_Matrix;

    public:
        SymMatrix2x2()
            : Base_Matrix(2)
        {}
                // Construction and assignment from a ublas matrix expression or copy assignment
        template<typename R> SymMatrix2x2(const boost::numeric::ublas::matrix_expression<R>& r) : Base_Matrix(r) {}
        template<typename R> void operator=(const boost::numeric::ublas::matrix_expression<R>& r)
        {
            Base_Matrix::operator=(r);
        }
        void operator=(const Base_Matrix &r)
        {
            Base_Matrix::operator=(r);
        }

    };
	class Matrix2x2 : public boost::numeric::ublas::bounded_matrix<double,2,2>
	{ 

		typedef boost::numeric::ublas::bounded_matrix<double,2,2> Base_Matrix;

    public:

        // Default constructor
        Matrix2x2()
            : Base_Matrix(2,2)
        {}

        // Construction and assignment from a ublas matrix expression or copy assignment
        template<typename R> Matrix2x2(const boost::numeric::ublas::matrix_expression<R>& r) : Base_Matrix(r) {}
        template<typename R> void operator=(const boost::numeric::ublas::matrix_expression<R>& r)
        {
            Base_Matrix::operator=(r);
        }
        void operator=(const Base_Matrix &r)
        {
            Base_Matrix::operator=(r);
        }

    };
    class Matrix3x3 : public boost::numeric::ublas::bounded_matrix<double,3,3>
	{ 

		typedef boost::numeric::ublas::bounded_matrix<double,3,3> Base_Matrix;

    public:

        // Default constructor
        Matrix3x3()
            : Base_Matrix(3,3)
        {}

        // Construction and assignment from a ublas matrix expression or copy assignment
        template<typename R> Matrix3x3(const boost::numeric::ublas::matrix_expression<R>& r) : Base_Matrix(r) {}
        template<typename R> void operator=(const boost::numeric::ublas::matrix_expression<R>& r)
        {
            Base_Matrix::operator=(r);
        }
        void operator=(const Base_Matrix &r)
        {
            Base_Matrix::operator=(r);
        }

    };
    class Matrix5x5 : public boost::numeric::ublas::bounded_matrix<double,5,5>
	{ 

		typedef boost::numeric::ublas::bounded_matrix<double,5,5> Base_Matrix;

    public:

        // Default constructor
        Matrix5x5()
            : Base_Matrix(5,5)
        {}

        // Construction and assignment from a ublas matrix expression or copy assignment
        template<typename R> Matrix5x5(const boost::numeric::ublas::matrix_expression<R>& r) : Base_Matrix(r) {}
        template<typename R> void operator=(const boost::numeric::ublas::matrix_expression<R>& r)
        {
            Base_Matrix::operator=(r);
        }
        void operator=(const Base_Matrix &r)
        {
            Base_Matrix::operator=(r);
        }

    };
    class SymMatrix6x6 : public boost::numeric::ublas::symmetric_matrix<double,boost::numeric::ublas::lower,boost::numeric::ublas::row_major,boost::numeric::ublas::bounded_array<double,21> >
    {
        typedef boost::numeric::ublas::symmetric_matrix<double,boost::numeric::ublas::lower,boost::numeric::ublas::row_major,boost::numeric::ublas::bounded_array<double,21> > Base_Matrix;

    public:
        SymMatrix6x6()
            : Base_Matrix(6)
        {}
                // Construction and assignment from a ublas matrix expression or copy assignment
        template<typename R> SymMatrix6x6(const boost::numeric::ublas::matrix_expression<R>& r) : Base_Matrix(r) {}
        template<typename R> void operator=(const boost::numeric::ublas::matrix_expression<R>& r)
        {
            Base_Matrix::operator=(r);
        }
        void operator=(const Base_Matrix &r)
        {
            Base_Matrix::operator=(r);
        }

    };

    class Matrix6x6 : public boost::numeric::ublas::matrix<double,boost::numeric::ublas::row_major,boost::numeric::ublas::bounded_array<double,36> >
    {
        typedef boost::numeric::ublas::matrix<double,boost::numeric::ublas::row_major,boost::numeric::ublas::bounded_array<double,36> > Base_Matrix;

    public:
        Matrix6x6()
            : Base_Matrix(6,6)
        {}
                // Construction and assignment from a ublas matrix expression or copy assignment
        template<typename R> Matrix6x6(const boost::numeric::ublas::matrix_expression<R>& r) : Base_Matrix(r) {}
        template<typename R> void operator=(const boost::numeric::ublas::matrix_expression<R>& r)
        {
            Base_Matrix::operator=(r);
        }
        void operator=(const Base_Matrix &r)
        {
            Base_Matrix::operator=(r);
        }

    };
    
    class SymMatrix5x5 : public boost::numeric::ublas::symmetric_matrix<double,boost::numeric::ublas::lower,boost::numeric::ublas::row_major,boost::numeric::ublas::bounded_array<double,15> >
    {
        typedef boost::numeric::ublas::symmetric_matrix<double,boost::numeric::ublas::lower,boost::numeric::ublas::row_major,boost::numeric::ublas::bounded_array<double,15> > Base_Matrix;

    public:
        SymMatrix5x5()
            : Base_Matrix(5)
        {}
                // Construction and assignment from a ublas matrix expression or copy assignment
        template<typename R> SymMatrix5x5(const boost::numeric::ublas::matrix_expression<R>& r) : Base_Matrix(r) {}
        template<typename R> void operator=(const boost::numeric::ublas::matrix_expression<R>& r)
        {
            Base_Matrix::operator=(r);
        }
        void operator=(const Base_Matrix &r)
        {
            Base_Matrix::operator=(r);
        }

    };

    class SymMatrix3x3 : public boost::numeric::ublas::symmetric_matrix<double,boost::numeric::ublas::lower,boost::numeric::ublas::row_major,boost::numeric::ublas::bounded_array<double,6> >
    {
		typedef boost::numeric::ublas::symmetric_matrix<double,boost::numeric::ublas::lower,boost::numeric::ublas::row_major,boost::numeric::ublas::bounded_array<double,6> > Base_Matrix;

    public:
        SymMatrix3x3()
            : Base_Matrix(3)
        {}
                // Construction and assignment from a ublas matrix expression or copy assignment
        template<typename R> SymMatrix3x3(const boost::numeric::ublas::matrix_expression<R>& r) : Base_Matrix(r) {}
        template<typename R> void operator=(const boost::numeric::ublas::matrix_expression<R>& r)
        {
            Base_Matrix::operator=(r);
        }
        void operator=(const Base_Matrix &r)
        {
            Base_Matrix::operator=(r);
        }

    };

//Should be a size invariant inversion somehow                                                                            

double determinant(Matrix3x3 input);                                                                   
SymMatrix5x5 InvertMatrix5x5(SymMatrix5x5 input);
Matrix3x3 InvertMatrix(Matrix3x3 input);//const ublas::matrix<T>& input, ublas::matrix<T>& inverse) 
Matrix2x2 InvertMatrix2(Matrix2x2 input);//const ublas::matrix<T>& input, ublas::matrix<T>& inverse) 

#ifdef DOMATRIX
/*

    Computes eigenvalues and eigenvectors of a real (non-complex)
    matrix. 
<P>
    If A is symmetric, then A = V*D*V' where the eigenvalue matrix D is
    diagonal and the eigenvector matrix V is orthogonal. That is,
	the diagonal values of D are the eigenvalues, and
    V*V' = I, where I is the identity matrix.  The columns of V 
    represent the eigenvectors in the sense that A*V = V*D.
    
<P>
    If A is not symmetric, then the eigenvalue matrix D is block diagonal
    with the real eigenvalues in 1-by-1 blocks and any complex eigenvalues,
    a + i*b, in 2-by-2 blocks, [a, b; -b, a].  That is, if the complex
    eigenvalues look like
<pre>

          u + iv     .        .          .      .    .
            .      u - iv     .          .      .    .
            .        .      a + ib       .      .    .
            .        .        .        a - ib   .    .
            .        .        .          .      x    .
            .        .        .          .      .    y
</pre>
        then D looks like
<pre>

            u        v        .          .      .    .
           -v        u        .          .      .    . 
            .        .        a          b      .    .
            .        .       -b          a      .    .
            .        .        .          .      x    .
            .        .        .          .      .    y
</pre>
    This keeps V a real matrix in both symmetric and non-symmetric
    cases, and A*V = V*D.
    
    
    
    <p>
    The matrix V may be badly
    conditioned, or even singular, so the validity of the equation
    A = V*D*inverse(V) depends upon the condition number of V.

   <p>
	(Adapted from JAMA, a Java Matrix Library, developed by jointly 
	by the Mathworks and NIST; see  http://math.nist.gov/javanumerics/jama).
**/

    template<typename T>
    class Eigenvalue
    {
    private:
        int n; // Row and column dimension (square matrix).
        bool issymmetric;
        boost::numeric::ublas::vector<T> d; // Array for internal storage of eigenvalues.
        boost::numeric::ublas::vector<T> e; // Array for internal storage of eigenvalues.
        boost::numeric::ublas::matrix<T> V; // Array for internal storage of eigenvectors.
        boost::numeric::ublas::matrix<T> H; // Array for internal storage of nonsymmetric Hessenberg form.
        boost::numeric::ublas::vector<T> ort; // Working storage for nonsymmetric algorithm.


        T hypot(const T& a,const T& b)
        {
            if (a == 0.0)
                return std::abs(b);
            else
            {
                T c = b/a;
                return std::abs(a)*std::sqrt(1.0+c*c);
            }
        }

        // Symmetric Householder reduction to tridiagonal form.
        //  This is derived from the Algol procedures tred2 by
        //  Bowdler, Martin, Reinsch, and Wilkinson, Handbook for
        //  Auto. Comp., Vol.ii-Linear Algebra, and the corresponding
        //  Fortran subroutine in EISPACK.
        inline void tred2()
        {
            for (int j=0;j<n;++j)
                d[j] = V(n-1,j);

            for (int i=n-1;i>0;--i)
            {
                T scale = 0.0;
                T h = 0.0;
                for (int k=0;k<i;++k)
                    scale += std::abs(d[k]);

                if (scale == 0.0)
                {
                    e[i] = d[i-1];
                    for (int j=0;j<i;++j)
                    {
                        d[j] = V(i-1,j);
                        V(i,j) = 0.0;
                        V(j,i) = 0.0;
                    }
                }
                else
                {
                    for (int k=0;k<i;++k)
                    {
                        d[k] /= scale;
                        h+= d[k]*d[k];
                    }
                    T f = d[i-1];
                    T g = ((f > 0.0) ? -std::sqrt(h) : std::sqrt(h));
                    e[i] = scale*g;
                    h -= f*g;
                    d[i-1] = f-g;
                    for (int j=0;j<i;++j)
                        e[j] = 0.0;

                    for (int j=0;j<i;++j)
                    {
                        f = d[j];
                        V(j,i) = f;
                        g = e[j]+V(j,j)*f;
                        for (int k=j+1;k<=i-1;++k)
                        {
                            g += V(k,j)*d[k];
                            e[k] += V(k,j)*f;
                        }
                        e[j]=g;
                    }
                    f = 0.0;
                    for (int j=0;j<i;++j)
                    {
                        e[j] /= h;
                        f+=e[j]*d[j];
                    }
                    T hh = f/(h+h);
                    for (int j=0;j<i;++j)
                        e[j] -= hh*d[j];

                    for (int j=0;j<i;++j)
                    {
                        f = d[j];
                        g = e[j];
                        for (int k=j;k <= i-1;++k)
                            V(k,j) -= (f*e[k]+g*d[k]);

                        d[j] = V(i-1,j);
                        V(i,j) = 0.0;
                    }
                }                
                d[i] = h;
            }

            for (int i=0;i<n-1;++i)
            {
                V(n-1,i) = V(i,i);
                V(i,i) = 1.0;
                T h = d[i+1];
                if (h != 0.0)
                {
                    for (int k=0;k<=i;++k)
                        d[k] = V(k,i+1)/h;

                    for (int j=0;j<=i;++j)
                    {
                        T g = 0.0;
                        for (int k=0;k<=i;++k)
                            g += V(k,i+1)*V(k,j);

                        for (int k=0;k<=i;++k)
                            V(k,j) -= g*d[k];
                    }
                }
                for (int k=0;k<=i;++k)
                    V(k,i+1) = 0.0;
            }
            for (int j=0;j<n;++j)
            {
                d[j] = V(n-1,j);
                V(n-1,j) = 0.0;
            }
            V(n-1,n-1) = 1.0;
            e[0] = 0.0;
        }

        // Symmetric tridiagonal QL algorithm.
        //  This is derived from the Algol procedures tql2, by
        //  Bowdler, Martin, Reinsch, and Wilkinson, Handbook for
        //  Auto. Comp., Vol.ii-Linear Algebra, and the corresponding
        //  Fortran subroutine in EISPACK.
        inline void tql2()
        {
            for (int i=1;i<n;++i)
                e[i-1] = e[i];

            e[n-1] = 0.0;

            T f = 0.0;
            T tst1 = 0.0;
            T eps = std::numeric_limits<T>::epsilon();
            for (int l=0;l<n;++l)
            {
                tst1 = std::max(tst1,std::abs(d[l])+std::abs(e[l]));
                int m = l;
                while (m < n)
                {
                    if (std::abs(e[m]) <= eps*tst1)
                        break;

                    ++m;
                }

                if (m > l)
                {
                    int iter = 0;
                    do
                    {
                        iter += 1;
                        T g = d[l];
                        T p = (d[l+1]-g)/(2.0*e[l]);
                        T r = hypot(p,1.0);
                        if (p<0.0) r = -r;
                        d[l] = e[l]/(p+r);
                        d[l+1] = e[l]*(p+r);
                        T dl1 = d[l+1];
                        T h = g-d[l];
                        for (int i=l+2;i<n;++i)
                            d[i] -= h;

                        f += h;

                        p = d[m];
                        T c = 1.0;
                        T c2 = c;
                        T c3 = c;
                        T el1 = e[l+1];
                        T s = 0.0;
                        T s2 = 0.0;
                        for (int i=m-1;i>=l;--i)
                        {
                            c3 = c2;
                            c2 = c;
                            s2 = s;
                            g = c*e[i];
                            h = c*p;
                            r = hypot(p,e[i]);
                            e[i+1] = s*r;
                            s = e[i]/r;
                            c = p/r;
                            p = c*d[i]-s*g;
                            d[i+1] = h+s*(c*g+s*d[i]);

                            for (int k=0;k<n;++k)
                            {
                                h = V(k,i+1);
                                V(k,i+1) = s*V(k,i)+c*h;
                                V(k,i) = c*V(k,i)-s*h;
                            }
                        }
                        p = -s*s2*c3*el1*e[l]/dl1;
                        e[l] = s*p;
                        d[l] = c*p;
                    } while (std::abs(e[l]) > eps*tst1);
                }
                d[l] += f;
                e[l] = 0.0;
            }
            for (int i=0;i<n-1;++i)
            {
                int k=i;
                T p = d[i];
                for (int j=i+1;j<n;++j)
                {
                    if (d[j] < p)
                    {
                        k = j;
                        p = d[j];
                    }
                }
                if (k != i)
                {
                    d[k] = d[i];
                    d[i] = p;
                    for (int j=0;j<n;++j)
                    {
                        p = V(j,i);
                        V(j,i) = V(j,k);
                        V(j,k) = p;
                    }
                }
            }
        }

        // Nonsymmetric reduction to Hessenberg form.
        //  This is derived from the Algol procedures orthes and ortran,
        //  by Martin and Wilkinson, Handbook for Auto. Comp.,
        //  Vol.ii-Linear Algebra, and the corresponding
        //  Fortran subroutines in EISPACK.
        inline void orthes()
        {
            int low = 0;
            int high = n-1;

            for (int m=low+1;m<=high-1;++m)
            {
                T scale = 0.0;
                for (int i=m;i<=high;++i)
                    scale += std::abs(H(i,m-1));

                if (scale != 0.0)
                {
                    T h = 0.0;
                    for (int i=high;i>=m;--i)
                    {
                        ort[i] = H(i,m-1)/scale;
                        h+=ort[i]*ort[i];
                    }
                    T g = ((ort[m] > 0.0) ? -std::sqrt(h) : std::sqrt(h));
                    h -= ort[m]*g;
                    ort[m] -= g;

                    for (int j=m;j<n;++j)
                    {
                        T f = 0.0;
                        for (int i=high;i>=m;--i)
                            f += ort[i]*H(i,j);
                        f /= h;
                        for (int i=m;i<=high;++i)
                            H(i,j) -= f*ort[i];
                    }
                    for (int i=0;i<=high;++i)
                    {
                        T f = 0.0;
                        for (int j=high;j>=m;--j)
                            f += ort[j]*H(i,j);
                        f /= h;
                        for (int j=m;j<=high;++j)
                            H(i,j) -= f*ort[j];
                    }
                    ort[m] *= scale;
                    H(m,m-1) = scale*g;
                }
            }
            for (int m=high-1;m>=low+1;--m)
            {
                if (H(m,m-1) != 0.0)
                {
                    for (int i=m+1;i<=high;++i)
                        ort[i] = H(i,m-1);
                    for (int j=m;j<=high;++j)
                    {
                        T g = 0.0;
                        for (int i=m;i<=high;++i)
                            g += ort[i]*V(i,j);
                        g /= ort[m];
                        g /= H(m,m-1);
                        for (int i=m;i<=high;++i)
                            V(i,j) += g*ort[i];
                    }
                }
            }
        }

        // Nonsymmetric reduction from Hessenberg to real Schur form.
        //  This is derived from the Algol procedure hqr2,
        //  by Martin and Wilkinson, Handbook for Auto. Comp.,
        //  Vol.ii-Linear Algebra, and the corresponding
        //  Fortran subroutine in EISPACK.
        inline void hqr2()
        {
            int nn = n;
            int ln = nn-1;
            int low = 0;
            int high = nn-1;
            T eps = std::numeric_limits<T>::epsilon();
            T exshift = 0.0;
            T p=0.0,q=0.0,r=0.0,s=0.0,z=0.0,t,w,x,y;
            T norm = 0.0;
            for (int i=0;i<nn;++i)
            {
                if ((i<low)||(i>high))
                {
                    d[i] = H(i,i);
                    e[i] = 0.0;
                }
                for (int j=std::max(i-1,0);j<nn;++j)
                    norm += std::abs(H(i,j));
            }
            int iter = 0;
            while (ln >= low)
            {
                int l = ln;
                while (l > low)
                {
                    s = std::abs(H(l-1,l-1))+std::abs(H(l,l));
                    if (s==0.0) s=norm;
                    if (std::abs(H(l,l-1)) < eps*s) break;
                    --l;
                }
                if (l==ln)
                {
                    H(ln,ln) += exshift;
                    d[ln] = H(ln,ln);
                    e[ln] = 0.0;
                    --ln;
                    iter = 0;
                }
                else if (l == ln-1)
                {
                    w = H(ln,ln-1)*H(ln-1,ln);
                    p = (H(ln-1,ln-1)-H(ln,ln))/2.0;
                    q = p*p+w;
                    z = std::sqrt(std::abs(q));
                    H(ln,ln) += exshift;
                    H(ln-1,ln-1) += exshift;
                    x = H(ln,ln);

                    if (q >= 0.0)
                    {
                        if (p>=0.0) z += p;
                        else z = p-z;
                        d[ln-1] = x+z;
                        d[ln] = ((z != 0.0) ? x-w/z : x+z); // d[ln] = d[ln-1] saving operator access
                        e[ln-1] = 0.0;
                        e[ln] = 0.0;
                        x = H(ln,ln-1);
                        s = std::abs(x)+std::abs(z);
                        p = x/s;
                        q = z/s;
                        r = std::sqrt(p*p+q*q);
                        p /= r;
                        q /= r;

                        for (int j=ln-1;j<nn;++j)
                        {
                            z = H(ln-1,j);
                            H(ln-1,j) = q*z+p*H(ln,j);
                            H(ln,j) *= q;
                            H(ln,j) -= p*z;
                        }
                        for (int i=0;i<=ln;++i)
                        {
                            z = H(i,ln-1);
                            H(i,ln-1) = q*z+p*H(i,ln);
                            H(i,ln) *= q;
                            H(i,ln) -= p*z;
                        }
                        for (int i=low;i<=high;++i)
                        {
                            z = V(i,ln-1);
                            V(i,ln-1) = q*z+p*V(i,ln);
                            V(i,ln) *= q;
                            V(i,ln) -= p*z;
                        }
                    }
                    else
                    {
                        d[ln-1] = x+p;
                        d[ln] = x+p;
                        e[ln-1] = z;
                        e[ln] = -z;
                    }
                    ln -= 2;
                    iter = 0;
                }
                else
                {
                    x = H(ln,ln);
                    y = 0.0;
                    w = 0.0;
                    if (l<ln)
                    {
                        y = H(ln-1,ln-1);
                        w = H(ln,ln-1)*H(ln-1,ln);
                    }
                    if (iter == 10)
                    {
                        exshift += x;
                        for (int i=low;i<=ln;++i)
                            H(i,i) -= x;
                        s = std::abs(H(ln,ln-1))+std::abs(H(ln-1,ln-2));
                        x = y = 0.75*s;
                        w = -0.4375*s*s;
                    }
                    if (iter == 30)
                    {
                        s = (y-x)/2.0;
                        s *= s;
                        s += w;
                        if (s > 0.0)
                            s = ((y<x) ? -std::sqrt(s) : std::sqrt(s));
                        s = x-w/((y-x)/2.0+s);
                        for (int i=low;i<=ln;++i)
                            H(i,i) -= s;
                        exshift += s;
                        x = y = w = 0.964;
                    }
                }
                ++iter;

                int m = ln-2;
                while (m >= l)
                {
                    z = H(m,m);
                    r = x-z;
                    s = y-z;
                    p = (r*s-w)/H(m+1,m)+H(m,m+1);
                    q = H(m+1,m+1)-z-r-s;
                    r = H(m+2,m+1);
                    s = std::abs(p)+std::abs(q)+std::abs(r);
                    p /= s;
                    q /= s;
                    r /= s;
                    if (m == 1) break;
                    if (std::abs(H(m,m-1))*(std::abs(q)+std::abs(r)) <
                        eps*(std::abs(p)*(std::abs(H(m-1,m-1))+
                        std::abs(z)+std::abs(H(m+1,m+1)))))
                            break;
                    --m;
                }
                for (int i=m+2;i<=ln;++i)
                {
                    H(i,i-2) = 0.0;
                    if (i>m+2)
                        H(i,i-3) = 0.0;
                }
                for (int k=m;k<ln-1;++k)
                {
                    bool notlast = (k != ln-1);
                    if (k != m)
                    {
                        p = H(k,k-1);
                        q = H(k+1,k-1);
                        r = (notlast ? H(k+2,k-1) : 0.0);
                        x = std::abs(p)+std::abs(q)+std::abs(r);
                        if (x != 0.0)
                        {
                            p /= x;
                            q /= x;
                            r /= x;
                        }
                    }
                    if (x == 0.0) break;
                    s = ((p<0.0) ? -std::sqrt(p*p+q*q+r*r) : std::sqrt(p*p+q*q+r*r));
                    if ( s != 0.0)
                    {
                        if ( k!= m)
                            H(k,k-1) = -s*x;
                        else if (l != m)
                            H(k,k-1) = -H(k,k-1);
                        p += s;
                        x = p/s;
                        y = q/s;
                        z = r/s;
                        q /= p;
                        r /= p;

                        for (int j=k;j<nn;++j)
                        {
                            p = H(k,j) + q*H(k+1,j);
                            if (notlast)
                            {
                                p += r*H(k+2,j);
                                H(k+2,j) -= p*z;
                            }
                            H(k,j) -= p*x;
                            H(k+1,j) -= p*y;
                        }
                        for (int i=0;i<=std::min(ln,k+3);++i)
                        {
                            p = x*H(i,k)+y*H(i,k+1);
                            if (notlast)
                            {
                                p += z*H(i,k+2);
                                H(i,k+2) -= p*r;
                            }
                            H(i,k) -= p;
                            H(i,k+1) -= p*q;
                        }
                        for (int i=low;i<=high;++i)
                        {
                            p = x*V(i,k)+y*V(i,k+1);
                            if (notlast)
                            {
                                p += z*V(i,k+2);
                                V(i,k+2) -= p*r;
                            }
                            V(i,k) -= p;
                            V(i,k+1) -= p*q;
                        }
                    }
                }
            }
            if (norm == 0.0) return;
            for (int ni = nn-1;ni>=0;--ni)
            {
                p = d[ni];
                q = e[ni];
                if (q == 0.0)
                {
                    int l = ni;
                    H(ni,ni) = 1.0;
                    for (int i=ni-1;i>=0;--i)
                    {
                        w = H(i,i)-p;
                        r = 0.0;
                        for (int j=l;j<=ni;++j)
                            r += H(i,j)*H(j,ni);
                        if (e[i] < 0.0)
                        {
                            z = w;
                            s = r;
                        }
                        else
                        {
                            l = i;
                            if (e[i] == 0.0)
                                if (w != 0.0)
                                    H(i,ni) = -r/w;
                                else
                                    H(i,ni) = -r/(eps*norm);
                            else
                            {
                                x = H(i,i+1);
                                y = H(i+1,i);
                                q = (d[i]-p)*(d[i]-p)+e[i]*e[i];
                                t = (x*s-z*r)/q;
                                H(i,ni) = t;
                                if (std::abs(x) > std::abs(z))
                                    H(i+1,ni) = (-r-w*t)/x;
                                else
                                    H(i+1,ni) = (-s-y*t)/z;
                            }
                            t = std::abs(H(i,ni));
                            if ((eps*t)*t > 1.0)
                                for (int j=i;j<=ni;++j)
                                    H(j,ni) /= t;
                        }
                    }
                }
                else if (q < 0.0)
                {
                    int l=ni-1;
                    if (std::abs(H(ni,ni-1)) > std::abs(H(ni-1,ni)))
                    {
                        H(ni-1,ni-1) = q/H(ni,ni-1);
                        H(ni-1,ni) = -(H(ni,ni)-p)/H(ni,ni-1);
                    }
                    else
                    {
                        std::complex<T> ctmp(0.0,-H(ni-1,ni));
                        std::complex<T> ctmp2(H(ni-1,ni-1)-p,q);
                        ctmp /= ctmp2;
                        H(ni-1,ni-1) = ctmp.real();
                        H(ni-1,ni) = ctmp.imag();
                    }
                    H(ni,ni-1) = 0.0;
                    H(ni,ni) = 1.0;
                    for (int i=ni-2;i>=0;--i)
                    {
                        T ra,sa,vr,vi;
                        ra = 0.0;
                        sa = 0.0;
                        for (int j=l;j<=ni;++j)
                        {
                            ra += H(i,j)*H(j,ni-1);
                            sa += H(i,j)*H(j,ni);
                        }
                        w = H(i,i)-p;
                        if (e[i] < 0.0)
                        {
                            z = w;
                            r = ra;
                            s = sa;
                        }
                        else
                        {
                            l = i;
                            if (e[i] == 0.0)
                            {
                                std::complex<T> ctmp(-ra,-sa);
                                std::complex<T> ctmp2(w,q);
                                ctmp /= ctmp2;
                                H(i,ni-1) = ctmp.real();
                                H(i,ni) = ctmp.imag();
                            }
                            else
                            {
                                x = H(i,i+1);
                                y = H(i+1,i);
                                vr = (d[i]-p)*(d[i]-p)+e[i]*e[i]-q*q;
                                vi = (d[i]-p)*2.0*q;
                                if ((vr == 0.0) && (vi == 0.0))
                                    vr = eps*norm*
                                        (std::abs(w)+std::abs(q)+
                                        std::abs(x)+std::abs(y)+
                                        std::abs(z));
                                std::complex<T> ctmp(x*r-z*ra+q*sa,x*s-z*sa-q*ra);
                                std::complex<T> ctmp2(vr,vi);
                                ctmp /= ctmp2;
                                H(i,ni-1) = ctmp.real();
                                H(i,ni) = ctmp.imag();
                                if (std::abs(x) > (std::abs(z)+std::abs(q)))
                                {
                                    H(i+1,ni-1) = (-ra - w * H(i,ni-1) + q * H(i,ni)) / x;
                                    H(i+1,ni) = (-sa - w * H(i,ni) - q * H(i,ni-1)) / x;
                                }
                                else
                                {
                                    std::complex<T> ctmp(-r-y*H(i,ni-1),-s-y*H(i,ni));
                                    std::complex<T> ctmp2(z,q);
                                    ctmp /= ctmp2;
                                    H(i+1,ni-1) = ctmp.real();
                                    H(i+1,ni) = ctmp.imag();
                                }
                            }
                            t = std::max(std::abs(H(i,ni-1)),std::abs(H(i,ni)));
                            if ((eps*t)*t > 1.0)
                                for (int j=i;j<=ni;++j)
                                {
                                    H(j,ni-1) /= t;
                                    H(j,ni) /= t;
                                }
                        }
                    }
                }
            }
            {
		int i;
		for (i=0;i<nn;++i)
                if ((i<low)||(i>high))
                    for (int j=i;j<nn;++j)
                        V(i,j) = H(i,j);
	    }
            for (int j=nn-1;j>=low;--j)
            {
                {
		int i;
		for (i=low;i<=high;++i);
                {
                    z = 0.0;
                    for (int k=low;k<=std::min(j,high);++k)
                        z += V(i,k)*H(k,j);
                    V(i,j) = z;
                }
		}
            }
        }
    public:

        /* Check for symmetry, then construct the eigenvalue decomposition
        @param A    Square real (non-complex) matrix
        */
        Eigenvalue(const boost::numeric::ublas::matrix<T> &A)
        {
            n = A.size2();
            V.resize(n,n,false);
            d.resize(n,false);
            e.resize(n,false);
            issymmetric = boost::numeric::ublas::is_symmetric(A);

            if (issymmetric)
            {
                V = A;
                tred2();
                tql2();
            }
            else
            {
                H.resize(n,n,false);
                ort.resize(n,false);
                H = A;
                orthes();
                hqr2();
            }
        }

        /** Return the eigenvector matrix
        @return     V
        */
        inline void getV(boost::numeric::ublas::matrix<T> &M)
        {
            M = V;
        }

        /** Return the real parts of the eigenvalues
        @return     real(diag(D))
        */
        inline void getRealEigenvalues(boost::numeric::ublas::vector<T> &V)
        {
            V = d;
        }

        /** Return the imaginary parts of the eigenvalues
        in parameter V.

        @pararm V: new matrix with imaginary parts of the eigenvalues.
        */
        inline void getImagEigenvalues(boost::numeric::ublas::vector<T> &V)
        {
            V = e;
        }

/** 
	Computes the block diagonal eigenvalue matrix.
    If the original matrix A is not symmetric, then the eigenvalue 
	matrix D is block diagonal with the real eigenvalues in 1-by-1 
	blocks and any complex eigenvalues,
    a + i*b, in 2-by-2 blocks, [a, b; -b, a].  That is, if the complex
    eigenvalues look like
<pre>

          u + iv     .        .          .      .    .
            .      u - iv     .          .      .    .
            .        .      a + ib       .      .    .
            .        .        .        a - ib   .    .
            .        .        .          .      x    .
            .        .        .          .      .    y
</pre>
        then D looks like
<pre>

            u        v        .          .      .    .
           -v        u        .          .      .    . 
            .        .        a          b      .    .
            .        .       -b          a      .    .
            .        .        .          .      x    .
            .        .        .          .      .    y
</pre>
    This keeps V a real matrix in both symmetric and non-symmetric
    cases, and A*V = V*D.

	@param D: upon return, the matrix is filled with the block diagonal 
	eigenvalue matrix.
	
*/
        inline void getD(boost::numeric::ublas::matrix<T> &D)
        {
            D.resize(n,n,false);
            D.clear();
            for (int i=0;i<n;++i)
            {
                D(i,i) = d[i];
                if (e[i]>0.0)
                    D(i,i+1) = e[i];
                else if (e[i]<0.0)
                    D(i,i-1) = e[i];
            }
        }
    }; 
#endif //DOMATRIX
}
}
#endif //MATRIX_H

