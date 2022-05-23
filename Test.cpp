#include <iostream>
#include "sources/OrgChart.hpp"
#include <string>
#include <vector>
#include "doctest.h"

using namespace std;
using namespace ariel;

TEST_CASE("add to chart")
{
    OrgChart organization;
    CHECK_NOTHROW(organization.add_root("CEO"));
    CHECK_NOTHROW(organization.add_sub("CEO", "CTO"));   // Now the CTO is subordinate to the CEO
    CHECK_NOTHROW(organization.add_sub("CEO", "CFO"));   // Now the CFO is subordinate to the CEO
    CHECK_NOTHROW(organization.add_sub("CEO", "COO"));   // Now the COO is subordinate to the CEO
    CHECK_NOTHROW(organization.add_sub("CTO", "VP_SW")); // Now the VP Software is subordinate to the CTO
    CHECK_NOTHROW(organization.add_sub("COO", "VP_BI")); // Now the VP_BI is subordinate to the COO
}

TEST_CASE("init checks")
{
    OrgChart organization;
    CHECK_THROWS(organization.add_sub("CEO", "CTO")); // throws- root don't exist
    CHECK_NOTHROW(organization.add_root("CEO"));
    CHECK_NOTHROW(organization.add_sub("CEO", "CTO"));
    CHECK_THROWS(organization.add_sub("CE", "CFO")); // not exist
}

TEST_CASE("level_order")
{
    OrgChart organization;
    CHECK_NOTHROW(organization.add_root("A"));
    CHECK_NOTHROW(organization.add_sub("A", "B"));
    CHECK_NOTHROW(organization.add_sub("A", "C"));
    CHECK_NOTHROW(organization.add_sub("B", "D"));
    CHECK_NOTHROW(organization.add_sub("D", "E"));
    CHECK_NOTHROW(organization.add_sub("D", "F"));
    string check;
    string ans;
    SUBCASE("level_order check")
    {
        for (auto it = organization.begin_level_order(); it != organization.end_level_order(); ++it)
        {
            check += (*it);
            check += " ";
        } // prints: A B C D E F
        ans = "A B C D E F ";
        CHECK(ans == check);
        organization.add_sub("D", "G");
        check.clear();

        for (auto it = organization.begin_level_order(); it != organization.end_level_order(); ++it)
        {
            check += (*it);
            check += " ";
        } // prints: A B C D E F G
        CHECK(ans != check);
        ans += "G ";
        CHECK(ans == check);
    }
    SUBCASE("forech eqals to level order")
    {
        ans  = "A B C D E F ";
        string check2;
        for (auto element : organization)
        {
            check2 += element;
            check2 += " ";
        }
        CHECK(ans == check2);
    }
}

TEST_CASE("reverse_order")
{
    OrgChart organization;
    CHECK_NOTHROW(organization.add_root("A"));
    CHECK_NOTHROW(organization.add_sub("A", "B"));
    CHECK_NOTHROW(organization.add_sub("A", "C"));
    CHECK_NOTHROW(organization.add_sub("B", "D"));
    CHECK_NOTHROW(organization.add_sub("D", "E"));
    CHECK_NOTHROW(organization.add_sub("D", "F"));
    string check;
    for (auto it = organization.begin_reverse_order(); it != organization.reverse_order(); ++it)
    {
        check += (*it);
        check += " ";
    } // prints:
    string ans = "E F D B C A ";
    CHECK(ans == check);
}
TEST_CASE("pre_order")
{
    OrgChart organization;
    CHECK_NOTHROW(organization.add_root("A"));
    CHECK_NOTHROW(organization.add_sub("A", "B"));
    CHECK_NOTHROW(organization.add_sub("A", "C"));
    CHECK_NOTHROW(organization.add_sub("B", "D"));
    CHECK_NOTHROW(organization.add_sub("D", "E"));
    CHECK_NOTHROW(organization.add_sub("D", "F"));
    string check;
    for (auto it = organization.begin_preorder(); it != organization.end_preorder(); ++it)
    {
        check += (*it);
        check += " ";
    } // prints:
    string ans = "A B D E F C ";
    CHECK(ans == check);
}
