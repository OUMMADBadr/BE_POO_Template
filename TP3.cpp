#include <iostream>
#include <vector>
using namespace std;

#include <iostream>
#include <list>
using namespace std;
int main(int argc, char **argv) {
    float i;
    list<float> l1, l2;
    list<float>::iterator it;

    // for(i=0; i<10; i++){
    //     l1.push_front(i);
    //     l2.push_back(i);
    // }

    for(i=0; i<1.0; i=i+0.1){
        l1.push_front(i);
    }

    for(it=l1.begin(); it!=l1.end(); it++) 
        cout << *it << " ";
        cout << endl;

    l2=l1;

    for(it=l2.begin(); it!=l2.end(); it++) 
        cout << *it << " ";
        cout << endl;

    // if(l2 < l1){
    //     it = l1.begin();
    // for(i=0; i<4; i++) 
    //     it++;
    //     l1.insert(it, l2.begin(), l2.end());
    // }

    for(it=l2.begin(); it!=l2.end(); it++) 
        l2.erase(it);

    
}

// int main(int argc, char **argv) {
// vector<float> v1,v2;
// float i;

// for(i=0; i<1.0; i=i+0.1){
//     v1.push_back(i);
// }

// cout << v1.size() << endl;

// v2=v1;

// for(i=0; i<v2.size(); i++){
//     cout << v2[i] << " ";
//     cout << endl;
// }

// for(i=0; i<=v2.size()/2+1; i++){
//     v2.pop_back();
// }

// for(i=0; i<v2.size(); i++){
//     cout << v2[i] << " ";
//     cout << endl;
// }
// }