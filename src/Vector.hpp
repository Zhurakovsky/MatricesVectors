#ifndef MATRIX_TEST_VECTOR_HPP
#define MATRIX_TEST_VECTOR_HPP

#include <iostream>
#include <vector>
#include <initializer_list>
#include <algorithm>
#include <tuple>
#include <type_traits>
#include <typeinfo>
#include <variant>
#include <optional>

#include "Matrix.hpp"

using namespace std;

namespace matrix
{
namespace
{
    enum class VectorType
    {
        VECTOR_UNDEFINED,
        VECTOR_ROW,
        VECTOR_COL
    };

} // namespace

template <class T>
class Matrix;

template <class T>
class Vector;

template<typename T> std::variant<Vector<T>, Matrix<T>> operator* (Vector<T>& lhs, Vector<T>& rhs);

template <class T>
class Vector
{
public:
    Vector(size_t rows, size_t cols, std::initializer_list<T> l) 
    : rows_{rows},
    cols_{cols},
    vector_type_{(rows == 1) 
        ? VectorType::VECTOR_ROW : ((cols == 1) 
        ? VectorType::VECTOR_COL : VectorType::VECTOR_UNDEFINED)},
    vec_{l} 
    {
        //cout << "Constructed vector of size " << l.size() << endl;
    }

    ~Vector()
    {
        //cout << "Destructed vector of size " << vec.size() << endl;
    }
    template <typename TT>
    friend std::variant<Vector<TT>, Matrix<TT>> operator* (Vector<TT>& left, Vector<TT>& right);
    //friend Container<T> operator+ <> (Container<T>& lhs, Container<T>& rhs);
    
    Vector& operator*(const Vector& mult)
    //std::variant<Vector&>&& operator*(Vector& mult)
    {
        if (GetType() == mult.GetType())
        {
            cout << "Unable to multiply collinear vectors" << endl;
            return (*this);
        }
        if (GetType() == VectorType::VECTOR_ROW && mult.GetType() == VectorType::VECTOR_COL)
        {
            if(GetSize() != mult.GetSize())
            {
                cout << "Product not defined" << endl;
                return (*this);
            }
            cout << "Unary operator Vector * Vector called" << endl;
            T value = 0;
            const size_t size = GetSize();
            for (size_t i = 0; i < size; i++)
            {
                value += (GetData().at(i) * mult.GetData().at(i));
            }
            vec_.clear();
            vec_.emplace_back(value);
            rows_ = 1;
            cols_ = 1;
            vector_type_ = VectorType::VECTOR_ROW;
            
            return (*this);
        }
        cout << "Catched Right Mult Vector Operator!!!" << endl;
        return (*this);
    }

    template<typename K>
    Vector& operator*(const K& mult)
    {
        if (!std::is_arithmetic<K>::value)
        {   
            return (*this);
        }

        for (auto & v : vec_)
        {
            v *= mult;
        }
    
        return (*this);
    }

    Vector& operator*=(const Vector& mult)
    {
        return (*this) * mult;
    }

    template<typename K>
    Vector& operator*=(const K& mult)
    {
        if (!std::is_arithmetic<K>::value)
        {   
            return (*this);
        }
        return (*this) * mult;
    }



    friend ostream & operator << (ostream &out, const Vector &v)
    {
        if (v.isRow())
        {
            for (const auto& val : v.GetData())
            {
                out << val << " ";
            }
        }
        else if (v.isCol())
        {
            for (const auto& val : v.GetData())
            {
                out << val << '\n';
            }
        }
        else
        {
            out << "Vector is undefined";
        }
        
        return out;
    }

    size_t GetSize() const
    {
        return vec_.size();
    }

    VectorType GetType() const
    {
        return vector_type_;
    }

    std::vector<T> GetData() const
    {
        return vec_;
    }

    template <typename K>
    std::optional<T> ScalarMultiply(const K& mult)
    {
        std::optional<T> res = nullopt;
        if (typeid(K)==typeid(Vector))
        {
            //cout << "Scalar Mult Vector with Vector!!!" << endl;
            if (mult.GetSize() == this->GetSize())
            {
                const size_t vector_size = this->GetSize(); 
                //cout << "Sizes are good" << endl;
                T res_val = 0;
                const auto tmp_local_vector = GetData();
                const auto tmp_other_vector = mult.GetData();
                for (size_t i = 0; i < vector_size; i ++)
                {
                    res_val += (tmp_local_vector.at(i) * tmp_other_vector.at(i));
                }
                res = res_val;
            }
        }
        return res;
    }

private:
    size_t rows_;
    size_t cols_;
    VectorType vector_type_{VectorType::VECTOR_UNDEFINED};
    std::vector<T> vec_;

    bool isRow() const
    {
        return vector_type_ == VectorType::VECTOR_ROW;
    }

    bool isCol() const
    {
        return vector_type_ == VectorType::VECTOR_COL;
    }

    inline tuple<bool, bool> GetOrientation()
    {
        return {isRow(), isCol()};
    }
};

template <typename T>
std::variant<Vector<T>, Matrix<T>> operator* (Vector<T>& left, Vector<T>& right)
{
    // std::variant<Vector<T>, Matrix<T>>(Vector{1,1,{1}});

    cout << "Binary operator Vector * Vector called" << endl;
    if (left.GetType() == right.GetType())
    {
        cout << "Unable to multiply collinear vectors" << endl;
        return std::variant<Vector<T>, Matrix<T>>(Vector{0,0,{0}});
    }
    if (left.GetType() == VectorType::VECTOR_ROW && right.GetType() == VectorType::VECTOR_COL)
    {
        if(left.GetSize() != right.GetSize())
        {
            cout << "Product not defined" << endl;
            return std::variant<Vector<T>, Matrix<T>>(Vector{0,0,{0}});
        }
        
        T value = 0;
        const size_t size = left.GetSize();
        for (size_t i = 0; i < size; i++)
        {
            value += (left.GetData().at(i) * right.GetData().at(i));
        }
        
        return std::variant<Vector<T>, Matrix<T>> {Vector{1,1,{value}}};
    }
    else if (left.GetType() ==VectorType::VECTOR_COL && right.GetType() == VectorType::VECTOR_ROW)
    {
        std::vector<T> data;
        const size_t rows = left.GetSize();
        const size_t cols = right.GetSize();
        for (size_t row = 0; row < rows; row++)
        {
            for (size_t col = 0; col < cols; col++)
            {
                data.emplace_back(left.GetData().at(row) * right.GetData().at(col));
            }
        }
        
        return std::variant<Vector<T>, Matrix<T>>(Matrix(rows,cols,data));
    }

    return std::variant<Vector<T>, Matrix<T>>(Vector{0,0,{0}});
}

} // namespace matrix

#endif // MATRIX_TEST_VECTOR_HPP
