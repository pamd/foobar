/* Assume the US dollar is available in denominations of $100, $50, $20, $10, $5, $1, 
 * $0.25, $0.10, $0.05 and $0.01, write a function to return the number of different ways 
 * to exactly represent an arbitrary value less than $1,000,000 using any number or 
 * combination of these denominations.
 * Refer to:
 * http://www.mitbbs.com/article_t/JobHunting/32154707.html
 * 
 * Optimizations by grass: 
 * (1) Create array size : int arraySize = v[v.length - 1] + 1;
 * arraySize = 10,001 ($100 = 10,000 cents)
 *
 * (dhu: THIS IS WRONG! The size of array should be v[v.length - 1].)
 *
 * (2) Write a simple MyBigInt to speed up.

 * Java BigInteger's performance is poor for this question. If I use BigInteger, 
 * it needs more than 10 mins to get the result; use the customized MyBigInt, 
 * it only needs less than 1 min.
 *
 * Answer:
 * x =	5555.55, result = 2,578,775,129,774,516,166,630,260 (25 digits)
 * Total time = 265 ms
 *
 * x =   50000.00, result = 875,711,632,112,903,190,555,557,650,261,001 (33 digits)
 * Total time = 2,281 ms
 *
 * x =  100000.00, result = 444628603176596242843543077208522001
 * Total time = 4,484 ms
 *
 * x = 1000000.00, result = 441,287,168,799,272,062,712,629,114,612,633,953,025,220,001 
 * (45 digits)
 */

#include<iostream>
#include <limits>

using namespace std;

/* dhu C++ code: translated from peking2 python code.
 * For large x, use double dp[][] to avoid overflow.
 * coins[] is input array of coin face values;
 * n is the number of coins in the array;
 * x is the target sum that we are trying to generate. */
unsigned long long count(int* coins, int n, int x) {
  const int row = coins[n - 1];

  // DP table: DP[i][j] is the number of unique combinations 
  // that have sum i and are generated by only using coins[0:j]. 
  unsigned long long dp[row][n];
  for (int i = 0; i < row; i++) {
    for (int j = 0; j < n; j++)
      dp[i][j] = 0;
  }

  for (int j = 0; j < n; j++) 
    dp[0][j] = 1;

  // Set elements in DP table
  for (int i = 1; i <= x; i++) {
    int r = i % row;
    // One way to generate r by only using coin[0], whose face value is 1.
    dp[r][0] = 1;  
    for (int j = 1; j < n; j++) {
      if (i < coins[j])
	dp[r][j] = dp[r][j - 1];
      else
	dp[r][j] = dp[r][j - 1] + dp[(i - coins[j]) % row][j];

      // Alternatively, we can use the following dirty trick to 
      // replace the above if/else section, because when i < coins[j], 
      // all elements after dp[i][j] are still 0. 
      // dp[idx][j] = dp[idx][j - 1] + dp[(i - coins[j] + row) % row][j];
    }       
  }

  return dp[x % row][n - 1];
}

/* Optimized version of count(). 
 * count() requires that 1 be available in input array of coin face values. 
 * Without this requirement, we can not set DP[r][0] = 1. 
 * This requirement can be removed by checking whether r can be divided 
 * by coin[0] or not.
 */
unsigned long long count_improved1(int* coins, int n, int x) {
  const int row = coins[n - 1];

  unsigned long long dp[row][n];
  for (int i = 0; i < row; i++) {
    for (int j = 0; j < n; j++) 
      dp[i][j] = 0;
  }

  for (int j = 0; j < n; j++)
    dp[0][j] = 1;

  // Set elements in DP table
  for (int i = 1; i <= x; i++) {
    int r = i % row;

    // For optimization only: Always reset dp[r][0] to 0. 
    // This is required because it may have been set to 1 in previous iteration. 
    dp[r][0] = 0; 

    // For optimization only: If i can be divided by coin[0], 
    // then there is one way to generate sum i by using coin[0] only. */ 
    if (i % coins[0] == 0)
      dp[r][0] = 1;  

    for (int j = 1; j < n; j++) {  
      if (i < coins[j])
	dp[r][j] = dp[r][j - 1]; 
      else
	dp[r][j] = dp[r][j - 1] + dp[(i - coins[j]) % row][j];

      /* Alternatively, we can use the following dirty trick to 
       * replace the above if/else section, because when i < coins[j], 
       * all elements after dp[i][j] are still 0. */
      // dp[r][j] = dp[r][j - 1] + dp[(i - coins[j] + row) % row][j];
    }       
  }

  return dp[x % row][n - 1];
}

// Same improvement, implemented in another way.
unsigned long long count_improved2(int* coins, int n, int x) {
  const int row = coins[n - 1];

  unsigned long long dp[row][n];
  for (int i = 0; i < row; i++) {
    for (int j = 0; j < n; j++) 
      dp[i][j] = 0;
  }

  for (int j = 0; j < n; j++)
    dp[0][j] = 1;

  // Set elements in DP table
  for (int i = 1; i <= x; i++) {
    int r = i % row;
    for (int j = 0; j < n; j++) {  
      if (i < coins[j])
	dp[r][j] = (j > 0 ? dp[r][j - 1] : 0); 
      else
	dp[r][j] = (j > 0 ? dp[r][j - 1] : 0) + dp[(i - coins[j]) % row][j];
      
      // NEVER EVER write the above section like this:
      //      dp[r][j] = (j > 0 ? dp[r][j - 1] : 0);     // line 1
      //      if (i >= coins[j])                         // line 2
      //	dp[r][j] += dp[(i - coins[j]) % row][j]; // line 3
      // Because when r == (i - coins[j]) % row, line 1 already 
      // overwrites dp[(i - coins[j]) % row][j] in line 3, then
      // the assignement on line 3 is totally wrong!
   }       
  }

  return dp[x % row][n - 1];
}

/* Use a full DP table to record previous results. 
 * code is more straightforward, but takes much more space. */
unsigned long long count_full_table(int* coins, int n, int x) {
  int row = x + 1;
  unsigned long long dp[row][n];
  for (int i = 0; i < row; i++) 
    for (int j = 0; j < n; j++)
      dp[i][j] = 0;

  for (int j = 0; j < n; j++) 
    dp[0][j] = 1;

  for (int i = 1; i <= x; i++) {
    // One way to generate r by only using coin[0] (face value of 1)
    dp[i][0] = 1;
    for (int j = 1; j < n; j++) {
      dp[i][j] = dp[i][j - 1];
      if (i >= coins[j])
	dp[i][j] += dp[i - coins[j]][j];
    }    
  }

  return dp[x][n - 1];
}
 
/* Solution copied from EPI 12.15 that uses a 1-D DP table.
 * Tried to change the length of DP[] from x + 1 into coins[n - 1] + 1, not work. 
 * This space optimization is probably not possible. 
 *
 * This solution does not require 1 be available in input array either.
 */
unsigned long long count_epi(int* coins, int n, int x) {
  unsigned long long DP[x + 1];
  DP[0] = 1;
  for (int i = 1; i <= x; i++)
    DP[i] = 0;
  
  for (int i = 0; i < n; i++) {
    for (int j = coins[i]; j <= x; j++) {
      DP[j] += DP[j - coins[i]];
    }
  }

  return DP[x];
}

// Improved version from EPI approach: incorrect when n > max_val
unsigned long long count_epi_improved(int* coins, int n, int x) {
  int max_val = coins[n - 1];
  unsigned long long DP[max_val + 1];
  DP[0] = 1;
  for (int i = 1; i <= max_val; i++)
    DP[i] = 0;
  
  for (int i = 0; i < n; i++) {
    for (int j = coins[i]; j <= x; j++) {
      if (j <= max_val) 
	DP[j] += DP[j - coins[i]];
      else if ((j - coins[i]) % max_val == 0)
	DP[j % max_val] += DP[max_val];
      else
	DP[j % max_val] += DP[(j - coins[i]) % max_val];
    }
  }

  //cout << "x: " << x << endl;
  //cout << "max_val: " << max_val << endl;
  if (x <= max_val)
    return DP[x];
  else if (x % max_val)
    return DP[x % max_val];
  else
    return DP[max_val];
}

// Test harness
int main(int argc, char** argv) {
  if (argc != 2) {
    cout << "Input value not found" << endl;
    return 0;
  }

  int x = atoi(argv[1]);
  if (x <= 0)
    return 0;

  int coins[] = { 1, 5, 10, 25, 100, 500, 1000, 2000, 5000, 10000 };

  // For the following input, only count1_optm() and count_epi() will give correct answer. 
  //int coins[] = { 2, 3, 5 };

  int n = sizeof(coins) / sizeof(int);

  //unsigned long long ull_max = numeric_limits<unsigned long long>::max();
  //cout << ull_max << " (unsigned long long max)" << endl;

  cout << count_full_table(coins, n, x) << "\t(full table)" << endl;
  //cout << count_epi(coins, n, x) << "\t(epi)" << endl;
  cout << count_improved2(coins, n, x) << "\t(test" << endl;

  return 0;
}