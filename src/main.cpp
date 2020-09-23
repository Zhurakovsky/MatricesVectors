#include <iostream>
#include <variant>
#include <typeinfo>
#include "Vector.hpp"
#include "Matrix.hpp"

using namespace std;
using namespace matrix;

int main()
{
    Vector v1(1, 5, {1,2,3,4,5});
    Vector v2(5, 1, {1,2,3,4,5});
    
    //Vector v3(1, 5, {1,2,3,4,5});

    //cout << v1 << endl;

    //cout << v2 << endl;

    //v1 = v1 * v2;
    //v1 *= 2;
    cout << v1 << endl;

    //v2 *= 0.5;
    cout << v2 << endl;

    auto v5 = v1 + v2;
    cout << v5 << endl;
    //v1 *= v2;

    //v3 *= v2;

    //cout << "v3: " << v3 << endl;

    //auto v4 = v1 * v2;
    // auto v4 = v2 * v1;

    // using T = typename std::decay<decltype(*(v1.GetData().begin()))>::type;

    // if (auto pval = std::get_if<Vector<T>>(&v4))
    // {
    //     cout << "V4 " << *pval << endl;
    // }
    // else if(auto pval = std::get_if<Matrix<T>>(&v4))
    // {
    //     cout <<  *pval << endl;
    // }
    // else
    // {
    //     cout << "Unable to multiply" << endl;
    // }
    


    // auto res = v1.ScalarMultiply(v2);
    // if (res)
    // {
    //     cout <<  "Scalar multiply = " << *res << endl;
    // }
    // else
    // {
    //     cout << "Bad Scalar multiply" << endl;
    // }

    // res = v1.ScalarMultiply(v3);
    // if (res)
    // {
    //     cout <<  "Scalar multiply = " << *res << endl;
    // }
    // else
    // {
    //     cout << "Bad Scalar multiply" << endl;
    // }
    
    // res = v2.ScalarMultiply(v1);
    // if (res)
    // {
    //     cout <<  "Scalar multiply = " << *res << endl;
    // }
    // else
    // {
    //     cout << "Bad Scalar multiply" << endl;
    // }
// **********************
    Vector v7(1, 3, {1,2,3});
    Matrix m7(3, 3, {1,2,3,4,5,6,7,8,9});

    auto vm7 = v7 * m7;

    using TT = typename std::decay<decltype(*(v7.GetData().begin()))>::type;

    if (auto pval = std::get_if<Vector<TT>>(&vm7))
    {
        cout << "V7 " << *pval << endl;
    }
    else if(auto pval = std::get_if<Matrix<TT>>(&vm7))
    {
        cout << "M7 " <<  *pval << endl;
    }
    else
    {
        cout << "MV7 Unable to multiply" << endl;
    }
// **********************

v7 *= m7;

cout << "V7 x m7: " << v7 << endl;

// **********************
    Vector v8(3, 1, {1,2,3});
    Matrix m8(1, 3, {1,2,3});


    auto vm8 = v8 * m8;

    using TT = typename std::decay<decltype(*(v8.GetData().begin()))>::type;

    if (auto pval = std::get_if<Vector<TT>>(&vm8))
    {
        cout << "V8 " << *pval << endl;
    }
    else if(auto pval = std::get_if<Matrix<TT>>(&vm8))
    {
        cout << "M8 " << endl;
        cout <<  *pval << endl;
    }
    else
    {
        cout << "MV8 Unable to multiply" << endl;
    }
// **********************
    // Matrix m1(3, 2, {1,2,3,4,5,6});
    // cout << m1 << endl;

    // Matrix m2(2, 3, {1,2,3,4,5,6});
    // cout << m2 << endl;

    // Matrix m3(3, 3, {1,2,3,4,5,6,7,8,9});
    // cout << m3 << endl;

    return 0;
}

/*
    Matrices and vectors shall be able to be multiplied.
    Addition and scalar multiplication shall be supported as well.
    No linear algebra functions are sought for (e.g. inverse, linear solving, svd etc.).
*/