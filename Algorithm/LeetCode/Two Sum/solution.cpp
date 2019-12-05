/**
 * Given an array of integers, return indices of the two numbers such that they add up to a specific target.
 * You may assume that each input would have exactly one solution, and you may not use the same element twice.
 * Example:
 *      Given nums = [2, 7, 11, 15], target = 9,
 *      Because nums[0] + nums[1] = 2 + 7 = 9,
 *      return [0, 1].
 * 来源：力扣（LeetCode）
 * 链接：https://leetcode-cn.com/problems/two-sum
 * 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
 */

/**
 * solution: V1 + V2 = K, 那么 V2 = K - V1;
 *              假设存在 Num == V2, 那么 Num 和 V1 之和变为K.
 *              将遍历数组将差值V2作为索引存储到hash map中.
 *              将数组中每一项的值都去查询hash map, 如果存在那么便存在两数之和为K.
 */
#include<unordered_map>
#include <vector>

using namespace std;

class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        std::unordered_map<int, int> hash;
        std::vector<int> result;
        for (auto i = 0; i < nums.size(); ++i) {
            auto item = nums.at(i);
            auto it = hash.find(item);
            if(it != hash.end() ){
                result.push_back(it->second);
                result.push_back(i);
                return result;
            }
            auto res = target - item;
            hash[res] = i;
        }
        return result;
    }
};