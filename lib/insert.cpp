#include "bptree.h"

/**
 * @brief Insert a key into the tree
 * 
 * @param key 
 * @return true 
 * @return false 
 */
bool bptree::insert(int key){

    // if root is null, well, create one
    if(root == NULL){
        root = new node( true);
        return root->insert_key(key);
    }

    node* cursor = root;

    std::vector<node*> parent_queue;

    while(!cursor->is_leaf){
        //push the cursor to the parent queue
        parent_queue.push_back(cursor);
        int index = cursor->search_key(key);
        cursor = cursor->children[index];
    }

    //insert the key in the leaf
    cursor->insert_key(key);

    //if there is space in the leaf just stop there
    if(cursor->keys.size() <= leaf_deg){
        return true;
    }

    // Split Split SPLIT!!!
    std::pair<node*,int> split_result = cursor->split();
    node* right_node = split_result.first;
    int middle_key = split_result.second;

    // if parent is null, create one as cursor is root
    if(cursor == root){
        root = new node(false);
        root->keys.push_back(middle_key);
        root->children.push_back(cursor);
        root->children.push_back(right_node);
        return true;
    }

    //Gotta propagate it to the parent

    //cursor, right_node and middle_key

    

    while( parent_queue.size() > 0 ){
        //get parent
        node* parent = parent_queue.back();
        parent_queue.pop_back();

        //insert cursor, right_node and middle_key
        int index = parent->search_key(middle_key);
        parent->keys.insert(parent->keys.begin()+index, middle_key);
        parent->children.insert(parent->children.begin()+index+1, right_node);
        
        //if there is space in the parent, insert it
        if(parent->keys.size() <= internal_deg){
            return true;
        }

        // Else Split Split SPLIT!!!
        split_result = parent->split();
        right_node = split_result.first;
        middle_key = split_result.second;

        // if parent is null, create one as cursor is root
        if(parent == root){
            root = new node(false);
            root->keys.push_back(middle_key);
            root->children.push_back(parent);
            root->children.push_back(right_node);
            return true;
        }
    }

    return false;
}