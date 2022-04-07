#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/*
455. Assign Cookies (Easy)
https://leetcode.com/problems/assign-cookies/
题目描述
    有一群孩子和一堆饼干，每个孩子有一个饥饿度，每个饼干都有一个大小。每个孩子只能吃
    一个饼干，且只有饼干的大小不小于孩子的饥饿度时，这个孩子才能吃饱。求解最多有多少孩子
    可以吃饱。
输入输出样例
    输入两个数组，分别代表孩子的饥饿度和饼干的大小。输出最多有多少孩子可以吃饱的数量。
    Input: [1,2], [1,2,3]
    Output: 2
    在这个样例中，我们可以给两个孩子喂[1,2]、[1,3]、[2,3] 这三种组合的任意一种。

题解
    因为饥饿度最小的孩子最容易吃饱，所以我们先考虑这个孩子。为了尽量使得剩下的饼干可
    以满足饥饿度更大的孩子，所以我们应该把大于等于这个孩子饥饿度的、且大小最小的饼干给这
    个孩子。满足了这个孩子之后，我们采取同样的策略，考虑剩下孩子里饥饿度最小的孩子，直到
    没有满足条件的饼干存在。

Assume you are an awesome parent and want to give your children some cookies. But, you should give each child at most one cookie.

Each child i has a greed factor g[i], which is the minimum size of a cookie that the child will be content with; and each cookie j has a size s[j]. If s[j] >= g[i], we can assign the cookie j to the child i, and the child i will be content. Your goal is to maximize the number of your content children and output the maximum number.

 

Example 1:

Input: g = [1,2,3], s = [1,1]
Output: 1
Explanation: You have 3 children and 2 cookies. The greed factors of 3 children are 1, 2, 3. 
And even though you have 2 cookies, since their size is both 1, you could only make the child whose greed factor is 1 content.
You need to output 1.
Example 2:

Input: g = [1,2], s = [1,2,3]
Output: 2
Explanation: You have 2 children and 3 cookies. The greed factors of 2 children are 1, 2. 
You have 3 cookies and their sizes are big enough to gratify all of the children, 
You need to output 2.
 

Constraints:

1 <= g.length <= 3 * 104
0 <= s.length <= 3 * 104
1 <= g[i], s[j] <= 231 - 1
*/

int findChilderns(vector<int>& childern, vector<int>& cookies);

int main(void)
{
    vector<int> childern = {1, 2};
    vector<int> cookies = {1, 2, 3};
    std::cout << findChilderns(childern, cookies);
    return 0;
}

int findChilderns(vector<int>& childern, vector<int>& cookies)
{
    sort(childern.begin(), childern.end());
    sort(cookies.begin(), cookies.end());
    int childIndex = 0;
    int cookieIndex = 0;
    while (childIndex < childern.size() && cookieIndex < cookies.size()) {
        if (childern[childIndex] <= cookies[cookieIndex]) {
            childIndex++;
        }
        cookieIndex++;
    }
    return childIndex;
}