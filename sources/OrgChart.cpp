#include "OrgChart.hpp"

namespace ariel
{
    OrgChart::OrgChart() // default
    {
    }
    OrgChart::~OrgChart() // default
    {
    }
    OrgChart::OrgChart(OrgChart &other) // deep
    {
        this->_root = other._root;
    }

    // OrgChart &OrgChart::operator=(OrgChart const &other)  // overload '=' for deep
    // {
    //     // this->_root->_sons = other._root->_sons;
    //     // this->_root->_father = other._root->_father;
    //     this->_root = other._root;
    //     return *this;
    // }

    OrgChart::OrgChart(OrgChart &&other) noexcept // shalow
    {
        this->_root = other._root;
    }
    OrgChart &OrgChart::operator=(OrgChart &&other) noexcept // overload '=' for shalow
    {
        this->_root = other._root;
        other._root.reset();
        return *this;
    }

     bool OrgChart::iterator::operator!=(const OrgChart::iterator &it) const
    {
        return _ptr != it._ptr;
    }
     bool OrgChart::iterator::operator==(const OrgChart::iterator &it) const
    {
        return _ptr == it._ptr;
    }

    OrgChart &OrgChart::add_root(string root)
    {
        if (this->_root == nullptr)
        {
            shared_ptr<Node> tmp = make_shared<Node>(root);
            this->_root = tmp;
        }
        else
        {
            this->_root->_name = root;
        }
        return *this;
    }

    bool OrgChart::find_child(shared_ptr<Node> &curr, string &parent, string &child)
    {
        shared_ptr<Node> nchild;
        if (curr->_name == parent)
        { // equals
            nchild = make_shared<Node>(child);
            nchild->_name = child;
            curr->_sons.push_back(nchild);
            return true;
        }
        for (size_t i = 0; i < curr->_sons.size(); i++)
        {
            if (find_child(curr->_sons.at(i), parent, child))
            {
                return true;
            }
        }
        return false;
    }

    OrgChart &OrgChart::add_sub(string parent, string child)
    {
        if (_root == nullptr)
        {
            throw invalid_argument("root don't exist");
        }
        if (!find_child(_root, parent, child))
        {
            throw runtime_error("parent don't exist");
        }

        return *this;
    }
    // check if the root exist
    bool OrgChart::check_root()
    {
        if (this->_root == nullptr)
        {
            throw invalid_argument("root don't exist");
        }
        return true;
    }

    void OrgChart::iterator::init_level(shared_ptr<Node> &n)
    {
        queue<shared_ptr<Node>> remain;
        for (shared_ptr<Node> &level : n->_sons)
        {
            this->_level.push_back(level);
            remain.push(level);
        }
        while (!remain.empty())
        {
            for (shared_ptr<Node> &level : remain.front()->_sons)
            {
                this->_level.push_back(level);
                remain.push(level);
            }
            remain.pop();
        }
        this->_ptr = this->_level.front();
    }
    void OrgChart::iterator::init_reverse(shared_ptr<Node> &n)
    {
        queue<shared_ptr<Node>> remain;
        remain.push(n);
        while (!remain.empty())
        {
            shared_ptr<Node> tmp = remain.front();
            remain.pop();
            if (!tmp->_sons.empty())
            {
                for (auto i = tmp->_sons.size() - 1; i > 0; i--)
                {
                    remain.push(tmp->_sons[i]);
                }
                remain.push(tmp->_sons[0]);
            }
            this->_reverse.insert(this->_reverse.begin(), tmp);
        }
        this->_ptr = this->_reverse.front();
    }

    void OrgChart::iterator::init_pre(shared_ptr<Node> &n)
    {
        this->_pre.push_back(n);
        for (size_t ind = 0; ind < n->_sons.size(); ind++)
        {

            init_pre(n->_sons.at(ind));
        }
        this->_ptr = this->_pre.front();
    }

    OrgChart::iterator::iterator(shared_ptr<Node> ptr, int order) : _ptr(ptr), _order(order)
    {
        if (this->_ptr != nullptr)
        {

            if (this->_order == 1) // inorder
            {
                if (this->_ptr->_sons.empty()) // don't have child
                {
                    this->_level.push_back(ptr); // push only the root
                }
                else
                {
                    this->_level.push_back(ptr); // push the root
                    iterator::init_level(ptr);
                }
            }
            else if (this->_order == 2) // reverse order
            {
                if (this->_ptr->_sons.empty()) // don't have child
                {
                    this->_reverse.push_back(ptr); // push only the root
                }
                else
                {
                    this->_reverse.push_back(ptr);
                    iterator::init_reverse(ptr);
                }
            }
            else // preorder
            {
                if (this->_ptr->_sons.empty()) // don't have child
                {
                    this->_pre.push_back(ptr); // push only the root
                }
                else
                {
                    iterator::init_pre(ptr);
                }
            }
        }
    }

    string &OrgChart::iterator::operator*() const
    {
        return _ptr->_name;
    }

    string *OrgChart::iterator::operator->() const
    {
        return &(_ptr->_name);
    }
    // overload the '++' operator
    OrgChart::iterator &OrgChart::iterator::operator++()
    {

        if (_order == 1 && this->_level.back() != this->_ptr)
        {
            this->_level.erase(this->_level.begin());
            this->_ptr = this->_level.front();
        }
        else if (_order == 2 && this->_reverse.back() != this->_ptr)
        {
            this->_reverse.erase(this->_reverse.begin());
            this->_ptr = this->_reverse.front();
        }
        else if (_order == 3 && this->_pre.back() != this->_ptr)
        {
            this->_pre.erase(this->_pre.begin());
            this->_ptr = this->_pre.front();
        }
        else
        {
            _ptr = nullptr;
            return *this;
        }
        return *this;
    }

    OrgChart::iterator OrgChart::begin_level_order()
    {
        check_root();
        return iterator(this->_root, 1);
    }
    OrgChart::iterator OrgChart::end_level_order()
    {
        check_root();
        return iterator(nullptr, 1);
    }

    OrgChart::iterator OrgChart::begin_reverse_order()
    {
        check_root();
        return iterator(this->_root, 2);
    }
    OrgChart::iterator OrgChart::reverse_order()
    {
        check_root();
        return iterator(nullptr, 2);
    }

    OrgChart::iterator OrgChart::begin_preorder()
    {
        check_root();
        return iterator(this->_root, 3);
    }
    OrgChart::iterator OrgChart::end_preorder()
    {
        check_root();
        return iterator(nullptr, 3);
    }

    OrgChart::iterator OrgChart::begin()
    {
        check_root();
        return iterator(this->_root, 1);
    }

    OrgChart::iterator OrgChart::end()
    {
        check_root();
        return iterator(nullptr, 1);
    }
    //print the chart
    ostream &operator<<(ostream &out, const OrgChart &tree)
    {
        if (tree._root == nullptr)
        {
            return out;
        }
        queue<shared_ptr<OrgChart::Node>> remain;
        cout << tree._root->_name << endl;
        for (const shared_ptr<OrgChart::Node> &curr : tree._root->_sons)
        {
            out << curr->_name << "  ";
            remain.push(curr);
        }
        out << endl;
        while (!remain.empty())
        {
            for (const shared_ptr<OrgChart::Node> &curr : remain.front()->_sons)
            {
                out << curr->_name << " ";
                remain.push(curr);
            }
            remain.pop();
        }

        return out;
    }
}
