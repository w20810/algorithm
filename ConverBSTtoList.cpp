// @Created time :2016年11月13日 星期日 19时13分08秒
#include <bits/stdc++.h>
using namespace std;

typedef long long LL;
const int INF = 1E9;

struct node
{
	int val;
	node* left;
	node* right;
	node(int x): val(x), left(NULL), right(NULL){}
};

class Solution
{
	public :
		/*
		 * 利用二叉树的中序遍历非递归版,直接把节点一个一个拼接起来就行了
		 * */
		static void ConvertBSTtoList(node** root)
		{
			stack<node*> st;
			node* pre = NULL;
			node* cur = *root;
			while (cur || !st.empty())
			{
				while (cur)
				{
					st.push(cur);
					cur = cur->left;
				}
				cur = st.top();
				st.pop();
				if (NULL == pre)
				{
					pre = cur;
					*root = cur;
				}
				else
				{
					pre->right = cur;
					cur->left = pre;
					pre = cur;
				}
				cur = cur->right;
			}
		}

		static node* GetTree(const string& s, int& index)
		{
			if ('#' == s[index])
			{
				index++;
				return NULL;
			}
			node* root = new node(s[index++]);
			root->left = GetTree(s, index);
			root->right = GetTree(s, index);
			return root;
		}

		static void TraversalTree(const node* root)
		{
			if (root)
			{
				printf("%c ", root->val);
				TraversalTree(root->left);
				TraversalTree(root->right);
			}
		}

		static void TraversalList(const node* head)
		{
			const node* tail = NULL;
			printf("\n --> ");
			while (head)
			{
				if (NULL == head->right)
					tail = head;
				printf("%c ", head->val);
				head = head->right;
			}
			printf("\n<-- ");
			while (tail)
			{
				printf("%c ", tail->val);
				tail = tail->left;
			}
		}
};

int main()
{
	int index = 0;
	node* root = Solution::GetTree("abc#d###e##", index);
	Solution::TraversalTree(root);
	Solution::ConvertBSTtoList(&root);
	Solution::TraversalList(root);	
    return 0;
}
