//
//  list.cpp
//  Homework 4
//
//  Created by Duncan Hackmann on 5/30/23.
//


void listAll(const MenuItem* m, string path) {
    // Append the current menu item's name to the path
    string newPath = path + m->name();
    
    // Print the path if it's not empty
    if (!newPath.empty()) {
        cout << newPath << endl;
    }
    
    // If the menu item is a compound item, recursively call listAll on its subitems
    const vector<MenuItem*>* subItems = m->menuItems();
    if (subItems) {
        for (auto it = subItems->begin(); it != subItems->end(); ++it) {
            listAll(*it, newPath + "/");
        }
    }
}
