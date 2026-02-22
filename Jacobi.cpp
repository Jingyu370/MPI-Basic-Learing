#include <iostream>
#include <vector>
using namespace std;

int main(){
    int step = 100;
    int N = 1000;

    vector<vector<float>> A(N+1,vector<float>(N + 1, 0.0f));
    vector<vector<float>> B(N+1,vector<float>(N + 1, 0.0f));

    for(int k = 1; k < step; ++k){

        for(int j = 1; j < N; ++j){
            for(int i = 1; i < N; ++i){
                B[i][j] = 0.25f * (A[i - 1][j] + A[i + 1][j] + A[i][j + 1] + A[i][j - 1]);
            }
        }

        for(int j = 1;j < N; ++j){
            for(int i = 1;i < n; ++i){
                A[i][j] = B[i][j];
            }
        }

    }

    return 0;
}