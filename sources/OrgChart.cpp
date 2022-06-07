#include "OrgChart.hpp"

namespace ariel
{
    OrgChart::OrgChart() = default; // explicitly defaulted, calls OrgChart::OrgChart()

    OrgChart::~OrgChart() = default; // same as above

    OrgChart::OrgChart(OrgChart &other) // deep
    {
        this->_root = other._root;
        // std::cout << "deep" << endl;
    }
    OrgChart &OrgChart::operator=(OrgChart const &other) = default; // overload '=' for deep

    /*
    reason is 'noexcept'
    :compile-time check that returns true if an expression is declared to not throw any exceptions.
    reason to use here:
    https://stackoverflow.com/questions/9249781/are-move-constructors-required-to-be-noexcept
    */
    OrgChart::OrgChart(OrgChart &&other) noexcept // shalow
    {
        this->_root = other._root;
        // std::cout << "shalow" << endl;
    }
    OrgChart &OrgChart::operator=(OrgChart &&other) noexcept // overload '=' for shalow
    {
        this->_root = other._root;
        //    other._root.reset(); // other becomes empty
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
    /*
    recursive helper function that find the parent using his name in order to add him new child
    */
    bool OrgChart::find_parent(shared_ptr<Node> &curr, string &parent, string &child)
    {
        shared_ptr<Node> nchild; // init shared_ptr of Node
        if (curr->_name == parent) // finfd the parent
        { // equals
            nchild = make_shared<Node>(child); // assign
            nchild->_name = child;
            curr->_sons.push_back(nchild);
            return true; // found
        }
        for (size_t i = 0; i < curr->_sons.size(); i++)
        {
            if (find_parent(curr->_sons.at(i), parent, child)) // recursive call on each one of the curr sons
            {
                return true;
            }
        }
        return false; // didn't found  - parent don't exist
    }

    OrgChart &OrgChart::add_sub(string parent, string child)
    {
        if (_root == nullptr)
        {
            throw invalid_argument("root don't exist");
        }
        if (!find_parent(_root, parent, child))
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
        queue<shared_ptr<Node>> remain; // q that contain all the remain nodes that needed to check there childs
        for (shared_ptr<Node> &level : n->_sons)
        {
            this->_level.push_back(level);
            remain.push(level);
        }
        while (!remain.empty())
        {
            for (shared_ptr<Node> &level : remain.front()->_sons)
            {
                this->_level.push_back(level); // push the curr node son to _level
                remain.push(level);            // push the curr node son to remain - need to check his childs
            }
            remain.pop(); // when finish check all curr sons  - pop him from remain
        }
        this->_ptr = this->_level.front(); // _ptr now point to first eement in _level vec
    }

    void OrgChart::iterator::init_reverse(shared_ptr<Node> &n)
    {
        queue<shared_ptr<Node>> remain;
        shared_ptr<Node> tmp;
        remain.push(n);
        while (!remain.empty())
        {
            tmp.reset(); // reset the smart pointer
            tmp = remain.front();
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
        for (size_t ind = 0; ind < n->_sons.size(); ++ind)
        {

            init_pre(n->_sons.at(ind)); // left
        }
        this->_ptr = this->_pre.front(); // eventually
    }

    OrgChart::iterator::iterator(shared_ptr<Node> ptr, int order) : _ptr(ptr), _order(order)
    {
        if (this->_ptr != nullptr)
        {

            if (this->_order == 1) // inorder
            {
                if (this->_ptr->_sons.empty()) // don't have child - cause seg fault if we init with no childs
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
    // return reference to string
    string &OrgChart::iterator::operator*() const
    {
        return _ptr->_name;
    }
    // return address to string (-> overload)
    string *OrgChart::iterator::operator->() const
    {
        return &(_ptr->_name);
    }
    // overload the '++' operator
    OrgChart::iterator &OrgChart::iterator::operator++()
    {
        if (_order == 1 && this->_level.back() != this->_ptr)
        { // not get to end
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
            _ptr = nullptr; // finish to iterate: can't ++ anymore
        }

        return *this;
    }
    /*
    return iterator for level order' iterate
    */
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
    // print the chart like leverl order
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
            remain.push(curr); // add to remain -> print this node childs (if exist..)
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
