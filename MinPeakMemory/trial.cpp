// CS 218 Programming Assignment 2
// Author: Saksham Rathi

#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;
int n;
vector<vector<int> > memory;
int* dp;
int* auxiliary_values;
pair<int, int> update_current_subset(int prev_mask, int new_element) {
	int current_cost = auxiliary_values[prev_mask];
	for (int i = 0 ; i < n ; i ++) {
		if ((prev_mask & (1<<i))) {
			if (memory[new_element][i]) {
				return make_pair(INT_MAX, INT_MAX);
			}
		}
	}
	for (int i = 0 ; i < n ; i ++) {
		if (!(prev_mask & (1<<i))) {
			current_cost += memory[new_element][i];
		}
	}
	
	int auxiliary_value = current_cost;
	for (int i = 0 ; i < n ; i ++ ) {
		if (prev_mask & (1<<i)) {
			auxiliary_value -= memory[i][new_element];
		}
	}
	auxiliary_value -= memory[new_element][new_element];
	return make_pair(max(dp[prev_mask], current_cost), auxiliary_value);
}

int main() {
	cin >> n;
	for (int i = 0 ; i < n ; i ++ ) {
		memory.push_back(vector<int>(n));
		for (int j = 0 ; j < n ; j ++) {
			cin >> memory[i][j];
		}
	}	
	dp = (int*)malloc(1<<(n+2));
	auxiliary_values = (int*)malloc(1<<(n+2));
	memset(dp, 127, 1<<(n+2));
	memset(auxiliary_values, 0, 1<<(n+2));
	dp[0] = 0;
	// dp.push_back(0);
	// auxiliary_values.push_back(0);
	for (int mask = 1 ; mask < (1 << n) ; mask++) {
		// dp.push_back(INT_MAX);
		// auxiliary_values.push_back(0);
		for (int i = 0 ; i < n ; i ++) {
			if (mask & ( 1 << i)) {
				int prev_mask = (mask^ (1<<i));
				pair<int, int> to_be_considered = update_current_subset(prev_mask, i);
				if (to_be_considered.first < dp[mask]) {
					auxiliary_values[mask] = to_be_considered.second;
					dp[mask] = to_be_considered.first;
				}
			}
		}
	}
	cout << dp[(1 << n)-1];
}

// Thank you for reading the code.