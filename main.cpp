#include "sources/OrgChart.hpp"
#include <assert.h> /* assert */

using namespace std;
using namespace ariel;

int main()
{
    OrgChart org1; // main chart

    org1.add_root("baruch");
    org1.add_sub("baruch", "eitan");
    org1.add_sub("baruch", "roza");
    org1.add_sub("baruch", "rita");
    org1.add_sub("eitan", "dvir");
    org1.add_sub("eitan", "shira");
    org1.add_sub("dvir", "unknown");

    OrgChart org2(org1);                           // call for deep
    OrgChart org3(static_cast<OrgChart &&>(org2)); // call for shalow
    OrgChart org4(org2);                           // call for shalow --> org2 still lvalue

    for (auto it = org2.begin_preorder(); it != org2.end_preorder(); ++it)
    {
        cout << it->data() << " "; // // arrow overload
    }
    cout<< endl;
    for (auto element : org4) //  work like level order
    {
        cout << element << " ";
    }
    cout<< endl;

    org1.add_root("mira"); // change the root name

    for (auto it = org1.begin_level_order(); it != org1.end_level_order(); ++it)
    {
        cout << (*it) << " "; //same only the root name changed
    } 
    cout<< endl;

}