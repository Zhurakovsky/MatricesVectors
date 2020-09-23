#ifndef MATRIX_TEST_VECTOR_HPP
#define MATRIX_TEST_VECTOR_HPP

#include <iostream>
#include <vector>
#include <initializer_list>
#include <algorithm>
#include <tuple>
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

template<typename T> std::variant<Vector<T>, Matrix<T>> operator* (Vector<T>& left, Vector<T>& right);
template<typename T> std::variant<Vector<T>, Matrix<T>> operator* (Vector<T>& left, Matrix<T>& right);
template<typename T> Vector<T> operator+(Vector<T>& left, Vector<T>& right);
template<typename T> Vector<T> operator+(Vector<T>& left, Matrix<T>& right);

template <class T> class Vector
{
public:
    Vector(size_t rows, size_t cols, std::initializer_list<T> l)
    : Vector(rows, cols, std::vector<T>(l))
    {}

    Vector(size_t rows, size_t cols, std::vector<T> v) 
    : rows_{rows},
    cols_{cols},
    vector_type_{(rows == 1) ? VectorType::VECTOR_ROW : ((cols == 1) ? VectorType::VECTOR_COL : VectorType::VECTOR_UNDEFINED)},
    vec_{v} 
    {}

    ~Vector() = default;

    template <typename TT> friend std::variant<Vector<TT>, Matrix<TT>> operator* (Vector<TT>& left, Vector<TT>& right);
    template <typename TT> friend std::variant<Vector<TT>, Matrix<TT>> operator* (Vector<TT>& left, Matrix<TT>& right);
    template <typename TT> friend Vector<TT> operator+(Vector<TT>& left, Vector<TT>& right);
    template <typename TT> friend Vector<TT> operator+(Vector<TT>& left, Matrix<TT>& right);

    Vector& operator+(const Vector& arg)
    {
        // TODO
    }

    Vector& operator+(const Matrix<T>& arg)
    {
        // TODO
    }
    
    Vector& operator*(const Vector& mult)
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

    Vector& operator*(const Matrix<T>& mult)
    {
        if (!IsStateGood() || !mult.IsStateGood())
        {
            cout << "Bad state. Cannot multiply." << endl;
            return (*this);
        }

        const auto [mult_rows, mult_cols] = mult.GetDimentions();

        if ((cols_ != mult_rows) || (GetType() != VectorType::VECTOR_ROW))
        {
            cout << "Bad multiply conditions. Cannot multiply." << endl;
            return (*this);
        }

        std::vector<T> data;
        for (size_t i = 0; i < mult_cols; i++)
        {
            auto col_data = mult.GetCol(i);
            T tmp = 0;
            for (size_t j = 0; j < mult_rows; j++)
            {
                tmp += (vec_.at(j) * col_data.at(j));
            }
            data.emplace_back(tmp);
        }

        vec_.clear();
        cols_ = data.size();
        std::copy(data.begin(), data.end(), std::back_inserter(vec_));

        return (*this);
    }

    template<typename K> Vector& operator*(const K& mult)
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

    Vector& operator+=(const Vector& arg)
    {
        return (*this) + arg;
    }

    Vector& operator+=(const Matrix<T>& arg)
    {
        return (*this) + arg;
    }

    Vector& operator*=(const Vector& mult)
    {
        return (*this) * mult;
    }

    Vector& operator*=(const Matrix<T>& mult)
    {
        return (*this) * mult;
    }

    template<typename K> Vector& operator+=(const K& arg)
    {
        if (!std::is_arithmetic<K>::value)
        {   
            return (*this);
        }
        return (*this) + arg;
    }

    template<typename K> Vector& operator*=(const K& mult)
    {
        if (!std::is_arithmetic<K>::value)
        {   
            return (*this);
        }
        return (*this) * mult;
    }

    friend ostream & operator << (ostream &out, const Vector &v)
    {
        if (v.IsRow())
        {
            for (const auto& val : v.GetData())
            {
                out << val << " ";
            }
        }
        else if (v.IsCol())
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

    std::tuple<size_t, size_t> GetDimentions() const
    {
        return std::tuple{rows_, cols_};
    }

    bool IsStateGood() const
    {
        return ((vector_type_ == VectorType::VECTOR_ROW) || (vector_type_ == VectorType::VECTOR_COL));
    } 

    std::vector<T> GetRow(size_t idx) const
    {
        std::vector<T> res;
        if(vector_type_ == VectorType::VECTOR_ROW)
        {
            if(idx < rows_)
            {
                res = vec_;
            }
        }
        else if(vector_type_ == VectorType::VECTOR_COL)
        {
            if(idx < rows_)
            {
                res.emplace_back(vec_.at(idx));
            }
        }
        return res;
    }

    std::vector<T> GetCol(size_t idx) const
    {
        std::vector<T> res();
        if(vector_type_ == VectorType::VECTOR_COL)
        {
            if(idx < cols_)
            {
                res.emplace_back(vec_.begin(), vec_.end());
            }
        }
        else if(vector_type_ == VectorType::VECTOR_ROW)
        {
            if(idx < cols_)
            {
                res.emplace_back(vec_.at(idx));
            }
        }

        return res;
    }
    
    template <typename K>
    std::optional<T> ScalarMultiply(const K& mult)
    {
        std::optional<T> res{nullopt};
        if (typeid(K)==typeid(Vector))
        {
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

    bool IsRow() const
    {
        return vector_type_ == VectorType::VECTOR_ROW;
    }

    bool IsCol() const
    {
        return vector_type_ == VectorType::VECTOR_COL;
    }
};

template <typename T> std::variant<Vector<T>, Matrix<T>> operator* (Vector<T>& left, Vector<T>& right)
{
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

template <typename T> std::variant<Vector<T>, Matrix<T>> operator* (Vector<T>& left, Matrix<T>& right)
{
    cout << "Binary operator Vector * Matrix called" << endl;

    if (!left.IsStateGood() || !right.IsStateGood())
    {
        cout << "Bad state. Cannot multiply." << endl;
        return std::variant<Vector<T>, Matrix<T>>(Vector{0,0,{0}});
    }

    const auto [left_rows, left_cols] = left.GetDimentions();
    const auto [right_rows, right_cols] = right.GetDimentions();

    if (left_cols != right_rows)
    {
        cout << "Left cols != Right rows. Product undefined." << endl;
        return std::variant<Vector<T>, Matrix<T>>(Vector{0,0,{0}});
    }

    std::vector<T> res_data;
    for (size_t row = 0; row < left_rows; row++)
    {
        auto r = left.GetRow(row);
        for (size_t col = 0; col < right_cols; col++)
        {
            auto c = right.GetCol(col);
            
            if (r.size() == c.size())
            {
                const size_t size = r.size();
                T tmp = 0;
                for (size_t i = 0; i < size; i++)
                {
                    tmp += (r.at(i) * c.at(i));
                }
                res_data.emplace_back(tmp);
            }
        }
    }

    if ((left_rows == 1) || (right_cols == 1))
    {
        return std::variant<Vector<T>, Matrix<T>>(Vector{left_rows,right_cols, res_data});
    }
    
    return std::variant<Vector<T>, Matrix<T>>(Matrix{left_rows,right_cols, res_data});
}

template <typename T> Vector<T> operator+(Vector<T>& left, Vector<T>& right)
{
    const size_t left_size = left.GetSize();
    const size_t right_size = right.GetSize();
    if (left_size != right_size)
    {
        return Vector(0,0,{0});
    }

    auto [rows, cols] = left.GetDimentions();
    std::vector<T> res;
    for (size_t i = 0; i < left_size; i++)
    {
        res.emplace_back(left.GetData().at(i) + right.GetData().at(i));
    }

    return Vector(rows, cols, res);
}

template <typename T> Vector<T> operator+(Vector<T>& left, Matrix<T>& right)
{
    // TODO
}

} // namespace matrix

#endif // MATRIX_TEST_VECTOR_HPP
