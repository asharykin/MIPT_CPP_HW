#include <gtest/gtest.h>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/operation.hpp>

using boost::numeric::ublas::bounded_matrix;

using boost::numeric::ublas::identity_matrix;

using boost::numeric::ublas::scalar_matrix;

using boost::numeric::ublas::prod;

bounded_matrix<uint64_t, 2, 2> matrix_pow(bounded_matrix<uint64_t, 2, 2> A, unsigned char n)
{
    // [ [1, 0], [0, 1] ]
    bounded_matrix<uint64_t, 2, 2> R = identity_matrix<uint64_t>(2);

    while (n > 0) {
        if (n & 1) { // like (n % 2 == 1) but faster
            R = prod(R, A);
        }
        A = prod(A, A);
        n >>= 1; // like (n /= 2) but faster
    }
    return R;
}

uint64_t fibonacci(unsigned char n)
{
    if (n == 0) return 0;

    bounded_matrix<uint64_t, 2, 2> T = scalar_matrix<uint64_t>(2, 2, 1); // [ [1, 1], [1, 1] ]
    T(1, 1) = 0; // [ [1, 1], [1, 0] ]

    // no need for n calculations, n-1 is enough. (0, 0) is already F(n)
    bounded_matrix<uint64_t, 2, 2> result = matrix_pow(T, n - 1);
    return result(0, 0);
}

TEST(MatrixPowTest, IdentityMatrix)
{
    identity_matrix<uint64_t> I = identity_matrix<uint64_t>(2);
    bounded_matrix<uint64_t, 2, 2> R = matrix_pow(I, 100);

    EXPECT_EQ(R(0, 0), 1u);
    EXPECT_EQ(R(0, 1), 0u);
    EXPECT_EQ(R(1, 0), 0u);
    EXPECT_EQ(R(1, 1), 1u);
}

TEST(MatrixPowTest, DiagonalMatrix)
{
    // A = [ [2, 0], [0, 3] ]
    bounded_matrix<uint64_t, 2, 2> A;
    A(0, 0) = 2; A(0, 1) = 0;
    A(1, 0) = 0; A(1, 1) = 3;

    // A^3 = [ [2^3, 0], [0, 3^3] ] = [ [8, 0], [0, 27] ]
    bounded_matrix<uint64_t, 2, 2> R = matrix_pow(A, 3);
    EXPECT_EQ(R(0, 0), 8u);
    EXPECT_EQ(R(0, 1), 0u);
    EXPECT_EQ(R(1, 0), 0u);
    EXPECT_EQ(R(1, 1), 27u);
}

TEST(MatrixPowTest, NonSymmetricMatrix)
{
    // A = [ [1, 2], [3, 4] ]
    bounded_matrix<uint64_t, 2, 2> M;
    M(0, 0) = 1; M(0, 1) = 2;
    M(1, 0) = 3; M(1, 1) = 4;

    // A^2 = [ [1*1 + 2*3, 1*2 + 2*4], [3*1 + 4*3, 3*2 + 4*4] ] = [ [7, 10], [15, 22] ]
    bounded_matrix<uint64_t, 2, 2> R = matrix_pow(M, 2);
    EXPECT_EQ(R(0, 0), 7u);
    EXPECT_EQ(R(0, 1), 10u);
    EXPECT_EQ(R(1, 0), 15u);
    EXPECT_EQ(R(1, 1), 22u);
}

TEST(FibonacciTest, HandlesBaseCase)
{
    EXPECT_EQ(fibonacci(0), 0u);
}

TEST(FibonacciTest, HandlesSmallValues)
{
    EXPECT_EQ(fibonacci(1), 1u);
    EXPECT_EQ(fibonacci(3), 2u);
    EXPECT_EQ(fibonacci(5), 5u);
    EXPECT_EQ(fibonacci(7), 13u);
    EXPECT_EQ(fibonacci(9), 34u);
}

TEST(FibonacciTest, HandlesMaxUint64Case)
{
    EXPECT_EQ(fibonacci(93), 12200160415121876738u);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

/**
 * Time Complexity: O(log n)
 * This algorithm implements the "Exponentiation by Squaring" technique.
 * In each iteration of the while loop, the exponent n is divided by 2 (n >>= 1).
 * This results in exactly floor(log2(n)) matrix multiplications.
 * Since we use fixed-size 2x2 matrices, each prod(A, B) call produces
 * a constant number of operations (8 multiplications, 4 additions).
 * Thus, the total complexity is O(C * log n), which simplifies to O(log n).
 */

/**
 * Other algorithms:
 * Naive Recursive - O(2^n)
 * Iterative (Simple Loop) - O(n)
 * Dynamic Programming - O(n)
 * Binet's Formula - O(1)
 */