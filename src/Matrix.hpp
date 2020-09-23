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

template<typename T> std::variant<Vector<T>, Matrix<T>> operator* (Matrix<T>& left, Matrix<T>& right);
template<typename T> std::variant<Vector<T>, Matrix<T>> operator* (Matrix<T>& left, Vector<T>& right);
template<typename T> Matrix<T> operator+(Matrix<T>& left, Matrix<T>& right);
template<typename T> Matrix<T> operator+(Matrix<T>& left, Vector<T>& right);

template <class T> class Matrix
{
public:
    Matrix(size_t rows, size_t cols, const std::initializer_list<T>& l)
    : Matrix(rows, cols, std::vector<T>(l))
    {}

    Matrix(size_t rows, size_t cols, const std::vector<T>& v)
    : rows_{rows},
      cols_{cols},
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
    }()}
    {
        if ((rows * cols) != v.size())
        {
            cout << "Wrong archive dimentions" << endl;
        }
        else
        {
            cout << "Constructed matrix " << rows << " x " << cols << " with " << v.size() << " elements." << endl;
        }
    }

    ~Matrix() = default;

    template <typename TT> friend std::variant<Vector<TT>, Matrix<TT>> operator* (Matrix<TT>& left, Matrix<TT>& right);
    template <typename TT> friend std::variant<Vector<TT>, Matrix<TT>> operator* (Matrix<TT>& left, Vector<TT>& right);
    template <typename TT> friend Matrix<TT> operator+(Matrix<TT>& left, Matrix<TT>& right);
    template <typename TT> friend Matrix<TT> operator+(Matrix<TT>& left, Vector<TT>& right);

    Matrix& operator+(const Matrix& arg)
    {
        // TODO
    }

    Matrix& operator+(const Vector<T>& arg)
    {
        // TODO
    }
    
    Matrix& operator*(const Matrix& mult)
    {
        // TODO
    }

    Matrix& operator*(const Vector<T>& mult)
    {
        // TODO
    }

    template<typename K> Matrix& operator*(const K& mult)
    {
        // TODO
    }

     Matrix& operator+=(const Matrix& arg)
    {
        return (*this) + arg;
    }

    Matrix& operator+=(const Vector<T>& arg)
    {
        return (*this) + arg;
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

    template<typename K> Matrix& operator*=(const K& mult)
    {
        if (!std::is_arithmetic<K>::value)
        {   
            return (*this);
        }
        return (*this) * mult;
    }

    MatrixState GetMatrixState() const
    {
        return matrix_state_;
    }

    std::vector<std::vector<T>> GetData() const
    {
        return mat_;
    }

    std::vector<T> GetRow(size_t idx) const
    {
        std::vector<T> res();
        if (idx < mat_.size())
        {
            res.emplace_back(mat_.at(idx).begin(), mat_.at(idx).end());
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
        if (m.GetMatrixState() == MatrixState::MATRIX_UNDEFINED)
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

};

template <typename T> std::variant<Vector<T>, Matrix<T>> operator* (Matrix<T>& left, Matrix<T>& right)
{
    // TODO
}

template <typename T> std::variant<Vector<T>, Matrix<T>> operator* (Matrix<T>& left, Vector<T>& right)
{
    // TODO
}

template <typename T> Matrix<T> operator+(Matrix<T>& left, Matrix<T>& right)
{
    // TODO
}

template <typename T> Matrix<T> operator+(Matrix<T>& left, Vector<T>& right)
{
    // TODO
}

} // namespace matrix

#endif // MATRIX_TEST_MATRIX_HPP