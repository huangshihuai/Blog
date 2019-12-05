/**
    Given an array of integers and an integer k, you need to find the total number of continuous subarrays whose sum equals to k.

    Example 1:
    Input:nums = [1,1,1], k = 2
    Output: 2
    Note:
    The length of the array is in range [1, 20,000].
    The range of numbers in the array is [-1000, 1000] and the range of the integer k is [-1e7, 1e7].

    来源：力扣（LeetCode）
    链接：https://leetcode-cn.com/problems/subarray-sum-equals-k
    著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
 */

/**
 * solution: 这是一道闭包题目, 我们需要找出[i - n, i]的和为k(i >= n).
 *              索引[0, n]的数组构建累计和为key的hash map, 如果当前累计和与K值的差值V能够在hash map中查找到, 就说明存在和为K的子数组
 * such as : 
 *      和值:   S1  S2  S3  S4  S5
 *      原值:   N1  N2  N3  N4  N5
 *      索引:   I0  I1  I2  I3  I4
 *  假设 S5 - K = S3, 那么 S5 - S3 = K
 *  S5 = N1 + N2 + N3 + N4 + N5
 *  S3 = N1 + N2 + N3
 *  S5 - S3 = (N1 + N2 + N3 + N4 + N5) - (N1 + N2 + N3) = N4 + N5 = K
 *  也就是说: SI - SJ = N(I + 1) +... NJ
 */
#include<unordered_map>
#include <vector>

using namespace std;

class Solution {
public:
    int subarraySum(vector<int>& nums, int k) {
        int size = nums.size();
        int sum = 0;
        int res = 0;
        std::unordered_map<int,int> umap;
        umap[0]=1;
        for(int i = 0; i < size; i++){
            sum += nums[i];
            res += umap[sum - k];
            umap[sum] += 1;
        }
        return res;
    }
};