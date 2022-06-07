#pragma once
#include <string>
#include <iostream>
#include <queue>
#include <vector>
#include <memory> // for smart pointers use

using namespace std;

namespace ariel
{
    class OrgChart
    {
        struct Node
        {
            string _name;
            vector<shared_ptr<Node>> _sons;
            shared_ptr<Node> _father;

            Node(string &name) : _name(name) {} // init the node with name
        };
        shared_ptr<Node> _root; // declare the root here
    public:
        OrgChart(); // default
        ~OrgChart();
        OrgChart(OrgChart &other);                      // deep
        OrgChart &operator=(OrgChart const &other);   // default version of the respective assignment operator
        OrgChart(OrgChart &&other) noexcept;            // shalow for "rvalue reference"
        OrgChart &operator=(OrgChart &&other) noexcept; // overload '=' for shalow "rvalue reference"
        OrgChart &add_root(string root);
        OrgChart &add_sub(string parent, string child);
        friend std::ostream &operator<<(std::ostream &out, const OrgChart &tree);

       class iterator
        {
        private:
            shared_ptr<Node> _ptr; // pointer to curr node in the iterator
            vector<shared_ptr<Node>> _level; // vector that rep 'level ordder'
            vector<shared_ptr<Node>> _reverse; // vector that rep 'reverse ordder'
            vector<shared_ptr<Node>> _pre; // vector that rep 'pre ordder'
            int _order; // (1, level), (2, reverse) , (3, pre)

        public:
            iterator(shared_ptr<Node> ptr, int order);
            string &operator*() const;  // dereference overload
            string *operator->() const; // arrow overload
            iterator &operator++();
            bool operator==(const iterator &it) const;
            bool operator!=(const iterator &it) const;
            void init_level(shared_ptr<Node> &n);
            void init_reverse(shared_ptr<Node> &n);
            void init_pre(shared_ptr<Node> &n);
        };

        iterator begin_level_order();
        iterator end_level_order();
        iterator begin_reverse_order();
        iterator reverse_order();
        iterator begin_preorder();
        iterator end_preorder();
        iterator begin();
        iterator end();

        bool check_root(); 
        bool find_parent(shared_ptr<Node> &curr, string &parent, string &child);
    };
}
