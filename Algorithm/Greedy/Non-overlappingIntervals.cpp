#include <vector>
#include <iostream>
#include <algorithm>
#include <numeric>

/*
There are n children standing in a line. Each child is assigned a rating value given in the integer array ratings.

You are giving candies to these children subjected to the following requirements:

Each child must have at least one candy.
Children with a higher rating get more candies than their neighbors.
Return the minimum number of candies you need to have to distribute the candies to the children.

 

Example 1:

Input: ratings = [1,0,2]
Output: 5
Explanation: You can allocate to the first, second and third child with 2, 1, 2 candies respectively.
Example 2:

Input: ratings = [1,2,2]
Output: 4
Explanation: You can allocate to the first, second and third child with 1, 2, 1 candies respectively.
The third child gets 1 candy because it satisfies the above two conditions.
 

Constraints:

n == ratings.length
1 <= n <= 2 * 104
0 <= ratings[i] <= 2 * 104
*/
using namespace std;
class Solution {
public:
    int eraseOverlapIntervals(vector<vector<int>>& intervals) {
        if (intervals.empty()) {
            return 0;
        }
        // 按照最大边界，从小到大排序
        std::sort(intervals.begin(), intervals.end(), [](vector<int>& first, vector<int>& second){
            return first[1] < second[1];
        });
        int remove = 0;
        int prev = intervals[0][1];
        for (int index = 1; index < intervals.size(); ++index) {
            // Right point of prev interval greater than point current interval.
            // This is an intersection
            if (intervals[index][0] < prev) {
                ++remove;
                continue;
            }
            prev = intervals[index][1];
        }
        return remove;
    }
};

int main(void)
{
    // https://leetcode.com/problems/candy/
    Solution solution;
    vector<vector<int>> intervals = {{1,2},{2,3},{3,4},{1,3}};
    std::cout << solution.eraseOverlapIntervals(intervals);
}