#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>

#define PB push_back
using namespace std;

typedef std::vector<int> vi;
typedef std::vector<std::vector<int>> vvi;
typedef std::vector<bool> vb;

vvi Mem;
int N;
int minMax = INT32_MAX;
vvi DP1;
vi DP2;

void printOrder(vi Order){
    for (int i = 0 ; i < Order.size() ; i++){
        cout << Order[i] << " ";
    }
    cout << endl;
}

bool isSchedulable(vi subset,vi cset){
    int n = subset.size();
    int nc = cset.size();
    if (n == 0 || n == N) return true; 
    for (int j = 0 ; j < nc ; j++){
        for (int i = 0 ; i < n ; i++){
            if (Mem[cset[j]][subset[i]] != 0){
                return false;
            }
        }
    }
    return true;
}

// int mem(vi order, int k){
//     int m = 0;
//     int n = order.size();
//     m += Mem[order[k]][order[k]];
//     for (int p = 0 ; p < k ; p++){
//         m += Mem[order[p]][order[k]];
//     }
//     for (int q = k + 1 ; q < n ; q++){
//         m += Mem[order[k]][order[q]];
//     }
//     for (int p = 0 ; p < k ; p++){
//         for (int q = k + 1 ; q < n ; q++){
//             m += Mem[order[p]][order[q]];
//         }
//     }
//     return m;   
// }


int peakMem(vi order){
	int max = 0;
	int n = order.size();
	for (int k = 0 ; k < n ; k++){
		int m = 0;
		m += Mem[order[k]][order[k]];
		for (int p = 0 ; p < k ; p++){
			m += Mem[order[p]][order[k]];
		}
		for (int q = k + 1 ; q < n ; q++){
			m += Mem[order[k]][order[q]];
		}
		for (int p = 0 ; p < k ; p++){
			for (int q = k + 1 ; q < n ; q++){
				m += Mem[order[p]][order[q]];
			}
		}
		if (m > max) max = m;

	}
	return max;
}
void Swap(int& a,int &b){
    int temp = a;
    a = b;
    b = a;
}
void findSubsets(vi set, int b){
    int n = set.size();
    if (n == 0) {
        DP2[0] = INT32_MAX;
        DP1[0] = vi(0);
        cout << "Base case empty subset " << DP1[0].size() << endl;
        return;
    }
    if (n == 1) {
        DP1[b] = vi(set);
        DP2[b] = Mem[set[0]][set[0]];
        cout <<"Base case singleton set " << DP1[b].size() << endl; 
        return;
    }
    cout << "The value of b" << b << endl;
    int Min = INT32_MAX;
    int Max = 0;
    vi subset_min(n);
    vi subset(n);

    for (int i = 0 ; i < n ; i++){
        int b0 = b;
        b0 -= (1 << set[i]);
        cout << "The value of b0" << b0 << endl;
        cout << DP1[b0].size() << endl;
        for (int v = 0 ; v < DP1[b0].size();v++) subset[v] = DP1[b0][v];
        subset.PB(set[i]);
        int m = peakMem(subset);
        if (Min > m){
            subset_min = vi(subset);
            Min = m;
        }
    }
    DP1[b] = vi(subset_min);
    // cout << "Inside" << endl;
    // printOrder(subset_min);
    DP1[b].resize(n);
    DP2[b] = Min; 
}

void createSubsets(){
    for (int i = 0 ; i < (1 << N) ; i++){
        vi subset,cset;
        for (int j = 0 ; j < N ; j++){
            if ((i & (1 << j)) != 0){
              subset.PB(j);  
            }
            else cset.PB(j);
        }
        cout << "Subset for i " << i << endl;
        printOrder(subset);
        cout << "Subset for i again " << i << endl;
        if (isSchedulable(subset,cset)){
            cout << "Size of schedulable subset inside create subset of size " << subset.size() << " for i value "<< i << endl;
            // cout << "Could schedule\n";
            // printOrder(subset);
            findSubsets(subset,i);
            // cout << "Found subsets" << endl;
        } 
        // cout << i << "\n";
    }
}
int main(){
	   /* Enter your code here. Read input from STDIN. Print output to STDOUT */   
	int n; // number of processes
	
 
	cin >> n;
	
	Mem = vvi(n);  
	N = n;
	int a;
    DP1 = vvi(1 << N);
    DP2 = vi(1 << N,INT32_MAX);
    for (int i = 0 ; i < (1<< N) ; i++) DP1[i] = vi(N);
    // for (int i = 0 ; i < (1<< N) ; i++) cout << DP1[i].size() << "\n";
	for(int i = 0 ; i < n ; i++){
		for (int j = 0 ; j < n ; j++){
			cin >> a;
			Mem[i].PB(a);
		}
	}
    createSubsets();
    for (int i = 0 ; i < DP2.size() ; i++)  cout << DP2[i] << "\n";
    return 0;
}