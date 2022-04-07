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
    int candy(vector<int>& ratings) {
        int size = ratings.size();
        if (size < 2) {
            return size;
        }
        vector<int> candys(size, 1);
        for (int index = 1; index < size; ++index) {
            if (ratings[index] > ratings[index - 1]) {
                candys[index] = candys[index - 1] + 1;
            }
        }
        for (int index = size - 1; index > 0; --index) {
            if (ratings[index] < ratings[index - 1]) {
                candys[index - 1] = max(candys[index - 1], candys[index] + 1);
            }
        }
        return accumulate(candys.begin(), candys.end(), 0);
    }
};

int main(void)
{
    // https://leetcode.com/problems/candy/
    Solution solution;
    vector<int> ratings = {1, 2, 0, 4, 3, 5};
    std::cout << solution.candy(ratings);
}