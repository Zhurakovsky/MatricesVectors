#include <iostream>
#include <variant>
#include <typeinfo>
#include "Vector.hpp"
#include "Matrix.hpp"

using namespace std;
using namespace matrix;

int main()
{
    cout << "******* Vectors *******" << endl;
    Vector v1(1, 5, {1,2,3,4,5});
    Vector v2(5, 1, {1,2,3,4,5});
    Vector v3(1, 5, {1,2,3,4,5});

    cout << v1 << endl;
    cout << v2 << endl;

    
    cout << "******* Vector += Vector *******" << endl;
    v1 += v3;
    cout << v1 << endl;

    cout << "******* Vector -= Vector *******" << endl;
    v1 -= v3;
    cout << v1 << endl;

    cout << "******* Vector -= Vector *******" << endl;
    v1 *= v2;
    cout << v1 << endl;

    cout << "******* Vector = Vector + Vector *******" << endl;
    auto v4 = v3 * v2;

    using T = typename std::decay<decltype(*(v1.GetData().begin()))>::type;

    if (auto pval = std::get_if<Vector<T>>(&v4))
    {
        cout << *pval << endl;
    }
    else if(auto pval = std::get_if<Matrix<T>>(&v4))
    {
        cout <<  *pval << endl;
    }
    else
    {
        cout << "Unable to multiply" << endl;
    }
    
    cout << "******* Vector.ScalarMultiply(Vector) *******" << endl;

    Vector v111(1, 5, {1,2,3,4,5});
    Vector v222(5, 1, {1,2,3,4,5});
    Vector v333(1, 5, {1,2,3,4,5});

    auto res = v111.ScalarMultiply(v222);
    if (res)
    {
        cout <<  "Scalar multiply = " << *res << endl;
    }
    else
    {
        cout << "Bad Scalar multiply" << endl;
    }

    res = v111.ScalarMultiply(v333);
    if (res)
    {
        cout <<  "Scalar multiply = " << *res << endl;
    }
    else
    {
        cout << "Bad Scalar multiply" << endl;
    }
    
    res = v222.ScalarMultiply(v111);
    if (res)
    {
        cout <<  "Scalar multiply = " << *res << endl;
    }
    else
    {
        cout << "Bad Scalar multiply" << endl;
    }

    cout << "******* <Vector || Matrix> = Vector * Matrix *******" << endl;

    Vector v7(1, 3, {1,2,3});
    Matrix m7(3, 3, {1,2,3,4,5,6,7,8,9});

    auto vm7 = v7 * m7;

    using TT = typename std::decay<decltype(*(v7.GetData().begin()))>::type;

    if (auto pval = std::get_if<Vector<TT>>(&vm7))
    {
        cout << "Vector output:" << endl;
        cout << *pval << endl;
    }
    else if(auto pval = std::get_if<Matrix<TT>>(&vm7))
    {
        cout << "Matrix output:" << endl; 
        cout <<  *pval << endl;
    }
    else
    {
        cout << "Unable to multiply" << endl;
    }

    cout << "******* Vector *= Matrix *******" << endl;

    v7 *= m7;
    cout << v7 << endl;

    cout << "******* <Vector || Matrix> = Vector * Matrix *******" << endl;

    Vector v8(3, 1, {1,2,3});
    Matrix m8(1, 3, {1,2,3});

    auto vm8 = v8 * m8;

    using TT = typename std::decay<decltype(*(v8.GetData().begin()))>::type;

    if (auto pval = std::get_if<Vector<TT>>(&vm8))
    {
        cout << "Vector output:" << endl;
        cout << *pval << endl;
    }
    else if(auto pval = std::get_if<Matrix<TT>>(&vm8))
    {
        cout << "Matrix output:" << endl;
        cout <<  *pval << endl;
    }
    else
    {
        cout << "Unable to multiply" << endl;
    }

    cout << "******* Matrix +- Matrix *******" << endl;

    Matrix m3(3, 3, {1,1,1,1,1,1,1,1,1});
    Matrix m4(3, 3, {1,1,1,1,1,1,1,1,1});

    m3 += m4;
    cout << m3 << endl;

    m3 -= m4;
    cout << m3 << endl;

    cout << "******* Matrix +-*/ Scalar *******" << endl;

    m3 += 2;
    cout << m3 << endl;

    m3 -= 1;
    cout << m3 << endl;

    m3 *= 2;
    cout << m3 << endl;

    m3 /= 2;
    cout << m3 << endl;

    cout << "******* Matrix +- Vector *******" << endl;

    Matrix m9(3, 1, {1,2,3});
    Vector v9(3, 1, {1,2,3});

    cout << m9 << endl; 

    m9 += v9;
    cout << m9 << endl;

    m9 -= v9;
    cout << m9 << endl; 

    Matrix m10(1, 3, {1,2,3});
    Vector v10(1, 3, {1,2,3});

    cout << m10 << endl; 

    m10 += v10;
    cout << m10 << endl;

    m10 -= v10;
    cout << m10 << endl;

    cout << "******* Matrix * Matrix *******" << endl;

    Matrix m11(3, 1, {1,2,3});
    Matrix m12(1, 3, {1,2,3});

    m11 *= m12;
    cout << m11 << endl;

    cout << "******* Matrix * Vector *******" << endl;
    
    Matrix m14(3, 1, {1,2,3});
    Vector v14(1, 5, {1,2,3,4,5});
    
    m14 *= v14;
    cout << m14 << endl;

    cout << "******* Matrix = Matrix + Matrix *******" << endl;
    Matrix m15(3, 3, {1,2,3,4,5,6,7,8,9});
    Matrix m16(3, 3, {1,2,3,4,5,6,7,8,9});
    Matrix m17 = (m15 + m16);

    cout << m17 << endl; 

    cout << "******* Matrix = Matrix - Matrix *******" << endl;
    Matrix m18(3, 3, {1,2,3,4,5,6,7,8,9});
    Matrix m19(3, 3, {1,2,3,4,5,6,7,8,9});
    Matrix m20 = (m18 - m19);

    cout << m20 << endl;

    cout << "******* Matrix = Matrix * Matrix *******" << endl;
    Matrix m21(3, 3, {1,2,3,4,5,6,7,8,9});
    Matrix m22(3, 3, {1,2,3,4,5,6,7,8,9});
    Matrix m23 = m21 * m22;

    cout << m23 << endl;

    cout << "******* Matrix = Matrix + Matrix *******" << endl;
    Matrix m24(3, 1, {1,2,3});
    Vector v24(3, 1, {1,2,3});
    Matrix mv24 = (m24 + v24);

    cout << mv24 << endl; 

    cout << "******* Matrix = Matrix - Matrix *******" << endl;
    Matrix m25(3, 1, {1,2,3});
    Vector v25(3, 1, {1,2,3});
    Matrix mv25 = (m25 - v25);

    cout << mv25 << endl; 

    return 0;
}

/*
    Write two classes in C++. One class shall be a matrix class and one shall be a vector class. 
    Matrices and vectors shall be able to be multiplied.
    Addition and scalar multiplication shall be supported as well.
    No linear algebra functions are sought for (e.g. inverse, linear solving, svd etc.).
*/