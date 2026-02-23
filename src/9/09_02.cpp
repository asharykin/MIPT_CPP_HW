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

    ~Tree()
    {
        std::cout << "Tree::~Tree()" << std::endl;
    }

    void traverse_v1() const { traverse_impl<BFS_Policy>(); }

    void traverse_v2() const { traverse_impl<DFS_Policy>(); }

    static std::shared_ptr<Node> create_node(int val, std::shared_ptr<Node> parent)
    {
        std::shared_ptr<Node> node = std::make_shared<Node>(val);
        node->parent = parent;
        return node;
    }

private:

    struct BFS_Policy
    {
        using Container = std::queue<std::shared_ptr<Node>>;

        static auto& get_next(Container& c) { return c.front(); }

        static void add_children(Container& c, const std::shared_ptr<Node>& n)
        {
            if (n->left) c.push(n->left);
            if (n->right) c.push(n->right);
        }
    };

    struct DFS_Policy
    {
        using Container = std::stack<std::shared_ptr<Node>>;

        static auto& get_next(Container& c) { return c.top(); }

        static void add_children(Container& c, const std::shared_ptr<Node>& n)
        {
            if (n->right) c.push(n->right);
            if (n->left) c.push(n->left);
        }
    };

    template <typename Policy, typename C = typename Policy::Container>
    void traverse_impl() const
    {
        if (!root) return;

        C container;
        container.push(root);

        while (!container.empty())
        {
            std::shared_ptr<Node> current = Policy::get_next(container);
            container.pop();
            std::cout << current->value << " ";
            Policy::add_children(container, current);
        }

        std::cout << std::endl;
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

        std::cout << "BSF Traversal: ";
        tree.traverse_v1();

        std::cout << "DSF Traversal: ";
        tree.traverse_v2();
    }
    // At this point, tree is destroyed.
    // Since we used weak_ptr for parents, there are no reference cycles.
    // All nodes will be automatically deleted by shared_ptr's reference counting.
}