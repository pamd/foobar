//============================================================================
// Search Insert Position
//
// Given a sorted array and a target value, return the index if the target is
// found. If not, return the index where it would be if it were inserted in order.
//
// You may assume no duplicates in the array.
//
// Here are few examples.
// [1,3,5,6], 5 → 2
// [1,3,5,6], 2 → 1
// [1,3,5,6], 7 → 4
// [1,3,5,6], 0 → 0
//============================================================================

#include <iostream>
using namespace std;

class Solution {
public:
  int searchInsert(int A[], int n, int target) {
    int left = 0;
    int right = n - 1;
    while (left <= right) {
      int mid = left + (right - left) / 2;
      if (A[mid] > target) 
	right = mid - 1;
      else if (A[mid] < target) 
	left = mid + 1;
      else 
	return mid;
    }
    return left;
  }
};

int main() {
  return 0;
}
