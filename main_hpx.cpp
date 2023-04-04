// main.cpp
#include <hpx/hpx_main.hpp>

#include <hpx/algorithm.hpp>
#include <hpx/chrono.hpp>
#include <hpx/local/init.hpp>
#include <hpx/modules/assertion.hpp>

#include <iostream>
#include <random>
#include <chrono>



using namespace std;

#define N 1000
vector<vector<int>> generateMatrix(int rows, int cols) {
    // Create a random number generator
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(1, 100);

    // Create the matrix and fill it with random values
    vector<vector<int>> matrix(rows, vector<int>(cols));
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            matrix[i][j] = dist(gen);
        }
    }

    return matrix;
}
//sequential version of matrix multipication 
vector<vector<int>> matmul_seq( const vector<vector<int>>& a,
                            const vector<vector<int>>& b, 
                            bool flag = 1) {
    
    auto start = std::chrono::high_resolution_clock::now();
    int ra = a.size();
    int ca = a[0].size();
    int rb = b.size();
    int cb = b[0].size();

    if (ca != rb){
        cerr << "Dimensions incorrect (it's A(m,n)*B(n,k))"<< endl;
        return {};
    }

    vector<vector<int>> c(ra, vector<int>(cb,0));

    for (int i = 0; i < ra; i++) {
        for (int j = 0; j < cb; j++) {
            for (int k = 0; k < ca; k++) {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }
    //printing the time taken by the sequential version
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    if (flag == 1){
        std::cout   << "Time taken by sequential version: " 
                    << duration.count() 
                    << "ms" << endl; 
    }
    return c;
}

vector<vector<int>> matmul_par_unseq( const vector<vector<int>>& a,
                            const vector<vector<int>>& b, 
                            bool flag = 1) {
    
    auto start = std::chrono::high_resolution_clock::now();
    int ra = a.size();
    int ca = a[0].size();
    int rb = b.size();
    int cb = b[0].size();

    if (ca != rb){
        cerr << "Dimensions incorrect (it's A(m,n)*B(n,k))"<< endl;
        return {};
    }

    vector<vector<int>> c(ra, vector<int>(cb,0));
    //todo : to make it  vectorization-safe, is to replace the mutex by an atomic variable
    hpx::for_each(hpx::execution::par_unseq, a.begin(), a.end(), [&](const std::vector<int>& row_a) {
        int i = &row_a - &a[0];
        for (int j = 0; j < cb; j++) {
            for (int k = 0; k < ca; k++) {
                c[i][j] += row_a[k] * b[k][j];
            }
        }
    });
    

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    if (flag == 1){
        std::cout   << "Time taken by  parallel(par_unseq) version: " 
                    << duration.count() 
                    << " ms" << endl; 
    }
    return c;
}
vector<vector<int>> matmul_par( const vector<vector<int>>& a,
                            const vector<vector<int>>& b, 
                            bool flag = 1) {
    

    auto start = std::chrono::high_resolution_clock::now();
    int ra = a.size();
    int ca = a[0].size();
    int rb = b.size();
    int cb = b[0].size();

    if (ca != rb){
        cerr << "Dimensions incorrect (it's A(m,n)*B(n,k))"<< endl;
        return {};
    }

    vector<vector<int>> c(ra, vector<int>(cb,0));
    //
    hpx::for_each(hpx::execution::par, a.begin(), a.end(), [&](const std::vector<int>& row_a) {
        int i = &row_a - &a[0];
        for (int j = 0; j < cb; j++) {
            for (int k = 0; k < ca; k++) {
                c[i][j] += row_a[k] * b[k][j];
            }
        }
    });
    
    //printing the time taken 
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    if (flag == 1){
        std::cout   << "Time taken by  parallel(par) version: " 
                    << duration.count() 
                    << "ms" << endl; 
    }
    return c;
}
int main() {
    auto a = generateMatrix(N, N);
    auto b = generateMatrix(N, N);

    auto e = matmul_seq(a, b);
    auto c = matmul_par_unseq(a, b);
    auto d = matmul_par(a, b);
   
}
