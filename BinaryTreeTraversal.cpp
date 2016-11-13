// @Created time :2016年11月13日 星期日 14时05分11秒
#include <bits/stdc++.h>
using namespace std;

struct TreeNode
{
	int val;
	TreeNode* left;
	TreeNode* right;
	TreeNode(int x): val(x), left(NULL), right(NULL){}
};

class BinaryTreeTeaversal
{
	public :
		static void Preorder(TreeNode* root)
		{
			vector<int> res;
			stack<TreeNode*> st;
			st.push(root);
			while (!st.empty())
			{
				TreeNode* cur = st.top();
				st.pop();
				while (cur)
				{
					res.push_back(cur->val);
					st.push(cur->right);
					cur = cur->left;
				}
			}
		}
		
		static void Inorder(TreeNode* root)
		{
			vector<int> res;
			stack<TreeNode* > st;
			TreeNode* cur = root;
			while (!st.empty() || cur)
			{
				while (cur)
				{
					st.push(cur);
					cur = cur->left;
				}
				res.push_back(st.top()->val);
				cur = st.top()->right;
				st.pop();
			}
		}

		/*按根节点,右子树,左子树的先后顺序遍历二叉树,将遍历结果逆序即为后续遍历的结果*/
		static void Postorder(TreeNode* root)
		{
			vector<int> res; 
			stack<TreeNode* > st;
			st.push(root);
			while (!st.empty())
			{
				TreeNode* cur = st.top();
				st.pop();
				res.push_back(cur->val);
				if (cur->left)
					st.push(cur->left);
				if (cur->right)
					st.push(cur->right);
			}
			for (int i = 0, j = res.size() - 1; i < j; i++, j--)
				swap(res[i], res[j]);
		}
};

int main()
{
	BinaryTreeTeaversal::Preorder(NULL);
    return 0;
}
