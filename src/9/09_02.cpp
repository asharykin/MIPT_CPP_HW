#include <iostream>
#include <memory>
#include <queue>
#include <stack>

class Tree
{
public:

    struct Node
    {
        int value = 0;
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;
        std::weak_ptr<Node> parent;

        explicit Node(int val) : value(val) { }

        ~Node()
        {
            std::cout << "Node::~Node(" << value << ")" << std::endl;
        }
    };

    std::shared_ptr<Node> root;

    Tree() = default;

    ~Tree() {
        std::cout << "Tree::~Tree()" << std::endl;
    }

    void traverse_v1() const
    {
        if (!root) return;

        std::cout << "BFS Traversal: ";
        std::queue<std::shared_ptr<Node>> q;
        q.push(root);

        while (!q.empty())
        {
            std::shared_ptr<Node> current = q.front();
            q.pop();

            std::cout << current->value << " ";

            if (current->left)
            {
                q.push(current->left);
            }
            if (current->right)
            {
                q.push(current->right);
            }
        }
        std::cout << std::endl;
    }

    void traverse_v2() const
    {
        if (!root) return;

        std::cout << "DFS Traversal: ";
        std::stack<std::shared_ptr<Node>> s;
        s.push(root);

        while (!s.empty())
        {
            std::shared_ptr<Node> current = s.top();
            s.pop();

            std::cout << current->value << " ";

            if (current->right)
            {
                s.push(current->right);
            }
            if (current->left)
            {
                s.push(current->left);
            }
        }
        std::cout << std::endl;
    }

    static std::shared_ptr<Node> create_node(int val, std::shared_ptr<Node> parent)
    {
        std::shared_ptr<Node> node = std::make_shared<Node>(val);
        node->parent = parent;
        return node;
    }
};

int main()
{
    {
        Tree tree;

        tree.root = Tree::create_node(1, nullptr);

        tree.root->left = Tree::create_node(2, tree.root);
        tree.root->right = Tree::create_node(3, tree.root);

        tree.root->left->left = Tree::create_node(4, tree.root->left);
        tree.root->left->right = Tree::create_node(5, tree.root->left);

        tree.root->right->left = Tree::create_node(6, tree.root->right);
        tree.root->right->right = Tree::create_node(7, tree.root->right);

        tree.traverse_v1();
        tree.traverse_v2();
    }
    // At this point, tree is destroyed.
    // Since we used weak_ptr for parents, there are no reference cycles.
    // All nodes will be automatically deleted by shared_ptr's reference counting.
}