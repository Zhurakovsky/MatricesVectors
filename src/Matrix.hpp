#ifndef MATRIX_TEST_MATRIX_HPP
#define MATRIX_TEST_MATRIX_HPP

#include <iostream>
#include <vector>
#include <initializer_list>
#include <tuple>
#include <type_traits>
#include <numeric>

#include "Vector.hpp"

using namespace std;

namespace matrix
{
namespace
{
enum class MatrixState
{
    MATRIX_UNDEFINED,
    MATRIX_OK
};

} // namespace

template <class T>
class Vector;

template <class T>
class Matrix;

template<typename T> Matrix<T> operator* (Matrix<T>& left, Matrix<T>& right);
template<typename T> Matrix<T> operator* (Matrix<T>& left, Vector<T>& right);
template<typename T> Matrix<T> operator+(Matrix<T>& left, Matrix<T>& right);
template<typename T> Matrix<T> operator+(Matrix<T>& left, Vector<T>& right);
template<typename T> Matrix<T> operator-(Matrix<T>& left, Matrix<T>& right);
template<typename T> Matrix<T> operator-(Matrix<T>& left, Vector<T>& right);

template <class T> class Matrix
{
public:
    Matrix(size_t rows, size_t cols, const std::initializer_list<T>& l)
    : Matrix(rows, cols, std::vector<T>(l)) {}

    Matrix(size_t rows, size_t cols, const std::vector<T>& v)
    : rows_{rows}, cols_{cols},
      matrix_state_{(rows * cols) == v.size() ? MatrixState::MATRIX_OK : MatrixState::MATRIX_UNDEFINED},
      mat_{[&]() {
        if ((rows * cols) != v.size())
        {
            return std::vector<std::vector<T>>();
        }

        std::vector<std::vector<T>> tmp_mat;
        
        for (size_t r = 0; r < rows; r++)
        {
            std::vector<T>tmp_row;
            for (size_t c = 0; c < cols; c++)
            {
                size_t idx = r * cols + c;

                T tmp_elem = v.at(idx);

                tmp_row.emplace_back(std::move(tmp_elem));
            }
            tmp_mat.emplace_back(std::move(tmp_row));
        }
        return tmp_mat;
    }()}{}

    Matrix(const Matrix&) = default;
    Matrix(Matrix&&) = default;
    Matrix& operator=(const Matrix&) = default;
    Matrix& operator=(Matrix&&) = default;
    ~Matrix() = default;

    template <typename TT> friend Matrix<TT> operator* (Matrix<TT>& left, Matrix<TT>& right);
    template <typename TT> friend Matrix<TT> operator* (Matrix<TT>& left, Vector<TT>& right);
    template <typename TT> friend Matrix<TT> operator+(Matrix<TT>& left, Matrix<TT>& right);
    template <typename TT> friend Matrix<TT> operator+(Matrix<TT>& left, Vector<TT>& right);
    template <typename TT> friend Matrix<TT> operator-(Matrix<TT>& left, Matrix<TT>& right);
    template <typename TT> friend Matrix<TT> operator-(Matrix<TT>& left, Vector<TT>& right);

    Matrix& operator+(const Matrix& arg)
    {
        if (!IsStateGood() || !arg.IsStateGood())
        {
            cout << "Bad state. Cannot Add." << endl;
            return (*this);
        }

        const auto [arg_rows, arg_cols] = arg.GetDimentions();

        if ((rows_ != arg_rows) || (cols_ != arg_cols))
        {
            cout << "Bad size. Cannot Add." << endl;
            return (*this);
        }

        const auto& tmp_arg = arg.GetData();

        for (size_t r = 0; r < arg_rows; r++)
        {
            for (size_t c = 0; c < arg_cols; c++ )
            {
                mat_[r][c] += tmp_arg[r][c];
            }
        }

        return (*this);
    }

    Matrix& operator+(const Vector<T>& arg)
    {
        if (!IsStateGood() || !arg.IsStateGood())
        {
            cout << "Bad state. Cannot Add." << endl;
            return (*this);
        }

        const auto [arg_rows, arg_cols] = arg.GetDimentions();

        if ((rows_ != arg_rows) || (cols_ != arg_cols))
        {
            cout << "Bad size. Cannot Add." << endl;
            return (*this);
        }

        const auto& tmp_arg = arg.GetData();

        for (size_t r = 0; r < arg_rows; r++)
        {
            for (size_t c = 0; c < arg_cols; c++ )
            {
                mat_[r][c] += tmp_arg.at(r * arg_cols + c);
            }
        }

        return (*this);
    }

    Matrix& operator-(const Vector<T>& arg)
    {
        if (!IsStateGood() || !arg.IsStateGood())
        {
            cout << "Bad state. Cannot Subtract." << endl;
            return (*this);
        }

        const auto [arg_rows, arg_cols] = arg.GetDimentions();

        if ((rows_ != arg_rows) || (cols_ != arg_cols))
        {
            cout << "Bad size. Cannot Subtract." << endl;
            return (*this);
        }

        const auto& tmp_arg = arg.GetData();

        for (size_t r = 0; r < arg_rows; r++)
        {
            for (size_t c = 0; c < arg_cols; c++ )
            {
                mat_[r][c] -= tmp_arg.at(r * arg_cols + c);
            }
        }

        return (*this);
    }

    Matrix& operator-(const Matrix& arg)
    {
        if (!IsStateGood() || !arg.IsStateGood())
        {
            cout << "Bad state. Cannot Subtract." << endl;
            return (*this);
        }

        const auto [arg_rows, arg_cols] = arg.GetDimentions();

        if ((rows_ != arg_rows) || (cols_ != arg_cols))
        {
            cout << "Bad size. Cannot Subtract." << endl;
            return (*this);
        }

        const auto& tmp_arg = arg.GetData();

        for (size_t r = 0; r < arg_rows; r++)
        {
            for (size_t c = 0; c < arg_cols; c++ )
            {
                mat_[r][c] -= tmp_arg[r][c];
            }
        }

        return (*this);
    }

    template<typename K> Matrix& operator+(const K& arg)
    {
        if (!std::is_arithmetic<K>::value)
        {   
            return (*this);
        }
        for (auto& row : mat_)
        {
            for (auto& col : row)
            {
                col += arg;
            }
        }
        return (*this);
    }

    template<typename K> Matrix& operator-(const K& arg)
    {
        if (!std::is_arithmetic<K>::value)
        {   
            return (*this);
        }
        return (*this) + (-arg);
    }
    
    Matrix& operator*(const Matrix& mult)
    {
        if (!IsStateGood() || !mult.IsStateGood())
        {
            cout << "Bad State. Cannot multiply" << endl;
            return (*this);
        }

        const auto [mult_rows, mult_cols] = mult.GetDimentions();

        if (cols_ != mult_rows)
        {
            cout << "Bad size. Product undefined." << endl;
            return (*this); 
        }
        
        std::vector<T> res;
        for (size_t row = 0; row < rows_; row++)
        {
            std::vector<T>row_data = GetRow(row);
            for (size_t col = 0; col < mult_cols; col++)
            {
                std::vector<T>col_data = mult.GetCol(col);
                T tmp = 0;
                for (size_t i = 0; i < mult_rows; i++)
                {
                    tmp += (row_data.at(i) * col_data.at(i));
                }
                res.emplace_back(tmp);
            }
        }

        Matrix  result(rows_, mult_cols, res);
        Clear();
        (*this) = result;

        return (*this);
    }

    Matrix& operator*(const Vector<T>& mult)
    {
        if (!IsStateGood() || !mult.IsStateGood())
        {
            cout << "Bad State. Cannot multiply" << endl;
            return (*this);
        }

        const auto [mult_rows, mult_cols] = mult.GetDimentions();

        if (cols_ != mult_rows)
        {
            cout << "Bad size. Product undefined." << endl;
            return (*this); 
        }

        std::vector<T> res;
        for (size_t row = 0; row < rows_; row++)
        {
            std::vector<T>row_data = GetRow(row);
            for (size_t col = 0; col < mult_cols; col++)
            {
                std::vector<T>col_data = mult.GetCol(col);
                T tmp = 0;
                for (size_t i = 0; i < mult_rows; i++)
                {
                    tmp += (row_data.at(i) * col_data.at(i));
                }
                res.emplace_back(tmp);
            }
        }

        Matrix  result(rows_, mult_cols, res);
        Clear();
        (*this) = result;

        return (*this);
    }

    template<typename K> Matrix& operator*(const K& mult)
    {
        if (!std::is_arithmetic<K>::value)
        {   
            return (*this);
        }
        for (auto& row : mat_)
        {
            for (auto& col : row)
            {
                col *= mult;
            }
        }
        return (*this);
    }

    template<typename K> Matrix& operator/(const K& mult)
    {
        if (!std::is_arithmetic<K>::value)
        {   
            return (*this);
        }
        for (auto& row : mat_)
        {
            for (auto& col : row)
            {
                col /= mult;
            }
        }
        return (*this);
    }

    Matrix& operator+=(const Matrix& arg)
    {
        return (*this) + arg;
    }

    Matrix& operator-=(const Matrix& arg)
    {
        return (*this) - arg;
    }

    Matrix& operator+=(const Vector<T>& arg)
    {
        return (*this) + arg;
    }

    Matrix& operator-=(const Vector<T>& arg)
    {
        return (*this) - arg;
    }

    Matrix& operator*=(const Matrix& mult)
    {
        return (*this) * mult;
    }

    Matrix& operator*=(const Vector<T>& mult)
    {
        return (*this) * mult;
    }

    template<typename K> Matrix& operator+=(const K& arg)
    {
        if (!std::is_arithmetic<K>::value)
        {   
            return (*this);
        }
        return (*this) + arg;
    }

    template<typename K> Matrix& operator-=(const K& arg)
    {
        if (!std::is_arithmetic<K>::value)
        {   
            return (*this);
        }
        return (*this) - arg;
    }

    template<typename K> Matrix& operator*=(const K& mult)
    {
        if (!std::is_arithmetic<K>::value)
        {   
            return (*this);
        }
        return (*this) * mult;
    }

    template<typename K> Matrix& operator/=(const K& mult)
    {
        if (!std::is_arithmetic<K>::value)
        {   
            return (*this);
        }
        return (*this) / mult;
    }

    MatrixState GetState() const
    {
        return matrix_state_;
    }

    std::vector<std::vector<T>> GetData() const
    {
        return mat_;
    }

    std::vector<T> GetRow(size_t idx) const
    {
        std::vector<T> res;
        if (idx < mat_.size())
        {
            res = mat_.at(idx);
        }
        return res;
    }

    std::vector<T> GetCol(size_t idx) const
    {
        std::vector<T> res;
        if (idx < (mat_.begin()->size()))
        {
            for (const auto& row : mat_)
            {
                res.emplace_back(row.at(idx));
            }
        }
        return res;
    }

    friend ostream & operator << (ostream &out, const Matrix &m)
    {
        if (m.GetState() == MatrixState::MATRIX_UNDEFINED)
        {
            out << "Matrix undefined";
            return out;
        }
        const auto mat = m.GetData(); 
        
        for (auto& row : mat)
        {
            for (const auto& col : row)
            {
                out << col << " ";
            }
            out << endl;
        }
        
        return out;
    }

    std::tuple<size_t, size_t> GetDimentions() const
    {
        return std::tuple{rows_, cols_};
    }

    bool IsStateGood() const
    {
        return matrix_state_ == MatrixState::MATRIX_OK;
    }
    
private:
    size_t rows_;
    size_t cols_;
    MatrixState matrix_state_;
    std::vector<std::vector<T>> mat_;

    void Clear()
    {
        mat_.clear();
        rows_ = 0;
        cols_ = 0;
        matrix_state_ = MatrixState::MATRIX_UNDEFINED;
    }
};

template <typename T> Matrix<T> operator* (Matrix<T>& left, Matrix<T>& right)
{
    if (!left.IsStateGood() || !right.IsStateGood())
    {
        cout << "Bad State. Cannot multiply" << endl;
        return Matrix{0,0,{0}};
    }

    const auto [left_rows, left_cols] = left.GetDimentions();
    const auto [right_rows, right_cols] = right.GetDimentions();

    if (left_cols != right_rows)
    {
        cout << "Bad size. Product undefined." << endl;
        return Matrix{0,0,{0}};
    }
    
    std::vector<T> res;
    for (size_t row = 0; row < left_rows; row++)
    {
        std::vector<T>row_data = left.GetRow(row);
        for (size_t col = 0; col < right_cols; col++)
        {
            std::vector<T>col_data = right.GetCol(col);
            T tmp = 0;
            for (size_t i = 0; i < row_data.size(); i++)
            {
                tmp += (row_data.at(i) * col_data.at(i));
            }
            res.emplace_back(tmp);
        }
    }
    Matrix result(left_rows, right_cols, res);
    return result;
}

template <typename T> Matrix<T> operator* (Matrix<T>& left, Vector<T>& right)
{
    if (!left.IsStateGood() || !right.IsStateGood())
    {
        cout << "Bad State. Cannot multiply" << endl;
        return Matrix{0,0,{0}};
    }

    const auto [left_rows, left_cols] = left.GetDimentions();
    const auto [right_rows, right_cols] = right.GetDimentions();

    if (left_cols != right_rows)
    {
        cout << "Bad size. Product undefined." << endl;
        return Matrix{0,0,{0}};
    }
    
    std::vector<T> res;
    for (size_t row = 0; row < left_rows; row++)
    {
        std::vector<T>row_data = left.GetRow(row);
        for (size_t col = 0; col < right_cols; col++)
        {
            std::vector<T>col_data = right.GetCol(col);
            T tmp = 0;
            for (size_t i = 0; i < right_rows; i++)
            {
                tmp += (row_data.at(i) * col_data.at(i));
            }
            res.emplace_back(tmp);
        }
    }

    return Matrix(left_rows, right_cols, res);
}

template <typename T> Matrix<T> operator+(Matrix<T>& left, Matrix<T>& right)
{
    if (!left.IsStateGood() || !right.IsStateGood())
    {
        cout << "Bad State. Cannot Add" << endl;
        return Matrix{0,0,{0}};
    }

    const auto [left_rows, left_cols] = left.GetDimentions();
    const auto [right_rows, right_cols] = right.GetDimentions();

    if ((left_rows != right_rows) || (left_cols != right_cols))
    {
        cout << "Bad size. Summ undefined." << endl;
        return Matrix{0,0,{0}};
    }

    std::vector<T> res;
    const auto& left_data = left.GetData();
    const auto& right_data = right.GetData();

    const size_t size = left_data.size();

    for (size_t row = 0; row < left_rows; row++)
    {
        for (size_t col = 0; col < right_cols; col++)
        {
            res.emplace_back(left_data[row][col] + right_data[row][col]);
        }
    }

    return Matrix(left_rows, right_cols, res);
}

template <typename T> Matrix<T> operator+(Matrix<T>& left, Vector<T>& right)
{
    if (!left.IsStateGood() || !right.IsStateGood())
    {
        cout << "Bad State. Cannot Add" << endl;
        return Matrix{0,0,{0}};
    }

    const auto [left_rows, left_cols] = left.GetDimentions();
    const auto [right_rows, right_cols] = right.GetDimentions();

    if ((left_rows != right_rows) || (left_cols != right_cols))
    {
        cout << "Bad size. Summ undefined." << endl;
        return Matrix{0,0,{0}};
    }

    std::vector<T> res;
    const auto& left_data = left.GetData();
    const auto& right_data = right.GetData();

    const size_t size = left_data.size();

    for (size_t row = 0; row < left_rows; row++)
    {
        for (size_t col = 0; col < right_cols; col++)
        {
            res.emplace_back(left_data[row][col] + right_data[row * right_cols + col]);
        }
    }

    return Matrix(left_rows, right_cols, res);
}

template <typename T> Matrix<T> operator-(Matrix<T>& left, Matrix<T>& right)
{
    if (!left.IsStateGood() || !right.IsStateGood())
    {
        cout << "Bad State. Cannot Add" << endl;
        return Matrix{0,0,{0}};
    }

    const auto [left_rows, left_cols] = left.GetDimentions();
    const auto [right_rows, right_cols] = right.GetDimentions();

    if ((left_rows != right_rows) || (left_cols != right_cols))
    {
        cout << "Bad size. Summ undefined." << endl;
        return Matrix{0,0,{0}};
    }

    std::vector<T> res;
    const auto& left_data = left.GetData();
    const auto& right_data = right.GetData();

    const size_t size = left_data.size();

    for (size_t row = 0; row < left_rows; row++)
    {
        for (size_t col = 0; col < left_cols; col++)
        {
            res.emplace_back(left_data[row][col] - right_data[row][col]);
        }
    }

    return Matrix(left_rows, right_cols, res);
}

template <typename T> Matrix<T> operator-(Matrix<T>& left, Vector<T>& right)
{
    if (!left.IsStateGood() || !right.IsStateGood())
    {
        cout << "Bad State. Cannot Add" << endl;
        return Matrix{0,0,{0}};
    }

    const auto [left_rows, left_cols] = left.GetDimentions();
    const auto [right_rows, right_cols] = right.GetDimentions();

    if ((left_rows != right_rows) || (left_cols != right_cols))
    {
        cout << "Bad size. Summ undefined." << endl;
        return Matrix{0,0,{0}};
    }

    std::vector<T> res;
    const auto& left_data = left.GetData();
    const auto& right_data = right.GetData();

    const size_t size = left_data.size();

    for (size_t row = 0; row < left_rows; row++)
    {
        for (size_t col = 0; col < right_cols; col++)
        {
            res.emplace_back(left_data[row][col] - right_data[row * right_cols + col]);
        }
    }

    return Matrix(left_rows, right_cols, res);
}

} // namespace matrix

#endif // MATRIX_TEST_MATRIX_HPP