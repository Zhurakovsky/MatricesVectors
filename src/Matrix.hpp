#ifndef MATRIX_TEST_MATRIX_HPP
#define MATRIX_TEST_MATRIX_HPP

#include <iostream>
#include <vector>
#include <initializer_list>
#include <tuple>
#include <type_traits>

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
class Matrix
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

    MatrixState GetMatrixState() const
    {
        return matrix_state_;
    }

    std::tuple<size_t, size_t, std::vector<std::vector<T>>> GetData() const
    {
        return std::tuple{rows_, cols_, mat_};
    }

    friend ostream & operator << (ostream &out, const Matrix &m)
    {
        if (m.GetMatrixState() == MatrixState::MATRIX_UNDEFINED)
        {
            out << "Matrix undefined";
            return out;
        }
        const auto [rows, cols, mat] = m.GetData();
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
    
private:
    size_t rows_;
    size_t cols_;
    MatrixState matrix_state_;
    std::vector<std::vector<T>> mat_;
};

} // namespace matrix

#endif // MATRIX_TEST_MATRIX_HPP