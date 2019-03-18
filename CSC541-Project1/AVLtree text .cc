#include <vector>
#include <iostream>
#include <stdlib.h>

typedef struct Node{
    struct Node *left;
    struct Node *right;
    int key;        //# of leaves in its left child
    int height;     //height of the AVL tree
    char* _text;
}Node;

typedef struct text_t
{
	Node* node;
}text_t;

char* ans_global = NULL;
text_t * create_text();
int length_text( text_t *txt);
char * get_line( text_t *txt, int index);
Node * insertAtLeaf(Node *node, char * new_line);
void append_line( text_t *txt, char * new_line);
Node* append_avl(Node *node, char * new_line);
char * set_line( text_t *txt, int index, char * new_line);
Node* insert_avl(Node *node, int index, char * new_line);
void insert_line( text_t *txt, int index, char * new_line);
Node* delete_avl(Node *node, int index);
char * delete_line( text_t *txt, int index);
int max(int a, int b);
int height(Node *x);
Node * left_rotation(Node *k1);
Node * right_rotation(Node *k2);
Node * left_right_rotation(Node *k3);
Node * right_left_rotation(Node *k4);

text_t * create_text(){
    text_t *root = NULL;
    root = (text_t*)malloc(sizeof(text_t));
    root->node = (Node*)malloc(sizeof(Node));
    root->node->left = NULL;
    root->node->right = NULL;
    root->node->key = 0;
    root->node->height = 0;
    root->node->_text = "";
    return root;
}

int length_text(text_t *txt){
    int length = 0;
    Node* temp = NULL;
    if(txt == NULL)
    {
        return -1;
    }
    temp = txt->node;
    while(temp != NULL)
    {
        length = length + temp->key;
        temp = temp->right;
    }
    return length;
}

//gets the line of number index, if such a line exists, and returns NULL else.
char * get_line( text_t *txt, int index) {
    Node *tmp = NULL;
    if (txt == NULL) {
        return NULL;
    } else {
        tmp = txt->node;
        while (tmp->left != NULL && tmp->right != NULL) {    //until reach a leaf
            if (index <= tmp->key) {
                tmp = tmp->left;
            } else {
                index -= tmp->key;
                tmp = tmp->right;
            }
        }
        if (tmp->key == 0 && index == 1) { //leaf and index match
            return tmp->_text;
        } else {
            return NULL;
        }
    }
}

void append_line( text_t *txt, char * new_line)
{
	Node* temp = txt->node;
	txt->node = append_avl(temp, new_line);
}

Node* append_avl(Node *node, char * new_line)
{
    Node *tmp = NULL;

    if(node->right != NULL)
    {
        node->right = append_avl(node->right, new_line);
        if (height(node->right) - height(node->left) == 2) 
        {
            tmp = node->right;
            if (height(tmp->left) > height(tmp->right)) 
            {
                node = right_left_rotation(node);
            } 
            else 
            {
                node = left_rotation(node);
            }
        }
        else
        {
        	node->height = max(height(node->left), height(node->right)) + 1;
        }
    }
    else
    {
        node = insertAtLeaf(node, new_line);
    }

    return node;
}

// sets the line of number index to new line. 
// and returns a pointer to the previous line of that number.

char * set_line( text_t *txt, int index, char * new_line) {
    Node *tmp = NULL;
    char *pre_line = NULL;
    if (txt == NULL) {
        return NULL;
    } else {
        tmp = txt->node;
        while (tmp->left != NULL && tmp->right != NULL) {    //until reach a leaf
            if (index <= tmp->key) {
                tmp = tmp->left;
            } else {
                index -= tmp->key;
                tmp = tmp->right;
            }
        }
        if (tmp->key == 0 && index == 1) { //leaf and index match
            pre_line = tmp->_text;
            tmp->_text = new_line;
            return pre_line;
        } else {
            return NULL;
        }
    }
}

Node* insertAtLeaf(Node *node, char *new_line) {
    Node *new_node, *old_node;
    new_node = (Node*)malloc(sizeof(Node));
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->key = 0;
    new_node->height = 0;
    new_node->_text = new_line;

    old_node = (Node*)malloc(sizeof(Node));
    old_node->left = NULL;
    old_node->right = NULL;
    old_node->key = 0;
    old_node->height = 0;
    old_node->_text = node->_text;

    node->left = new_node;
    node->right = old_node;
    node->height = 1;
    node->key = 1;
    node->_text = NULL;
    return node;     
}

//inserts the line before the line of number index to new line,
//renumbering all lines after that line
//If no such line exists, it appends new line as new last line.

void insert_line( text_t *txt, int index, char * new_line)
{
	Node* temp = NULL;
	if (index < 1 || index > length_text(txt) + 1) {
        index = length_text(txt) + 1;
    }
    temp = txt->node;
    txt->node = insert_avl(temp, index, new_line);
}

Node* insert_avl(Node *node, int index, char * new_line) {
    //insert
    char *t = NULL;
    Node *tmp = NULL;
    
    if (index <= node->key) {
        node->key += 1;                
        node->left = insert_avl(node->left, index, new_line);
        if (height(node->left) - height(node->right) == 2) {
            tmp = node->left;
            if (height(tmp->right) > height(tmp->left)) {
                node = left_right_rotation(node);
            } else {
                node = right_rotation(node);
            }
        }
        else
        {
        	node->height = max(height(node->left), height(node->right)) + 1;
        }
    }
    else if (index > node->key && node->right != NULL) {
        node->right = insert_avl(node->right, index - node->key, new_line);
        if (height(node->right) - height(node->left) == 2) {
            tmp = node->right;
            if (height(tmp->left) > height(tmp->right)) {
                node = right_left_rotation(node);
            } else {
                node = left_rotation(node);
            }
        }
        else
        {
        	node->height = max(height(node->left), height(node->right)) + 1;
        }
    }
    else {
        node = insertAtLeaf(node, new_line);
    }

    return node;       
}

char * delete_line( text_t *txt, int index)
{
	Node* temp = txt->node;
    if(txt == NULL || index < 1 || index > length_text(txt))
    {
        return NULL;
    }
    txt->node = delete_avl(temp, index);
    return ans_global;
}

Node* delete_avl(Node *node, int index)
{
    Node *temp = NULL;
    Node *mid = NULL;

    if(index <= node->key)
    {
        node->key = node->key - 1;
        node->left = delete_avl(node->left, index);
        if(node->left == NULL)
        {
            mid = node->right;
            node->left = mid->left;
            node->right = mid->right;
            node->key = mid->key;
            node->_text = mid->_text;
            node->height = mid->height;
            free(mid);
            mid = NULL;
        }
        if(height(node->right) - height(node->left) == 2)
        {
            temp = node->right;
            if(height(temp->left) > height(temp->right))
            {
                node = right_left_rotation(node);
            }
            else
            {
                node = left_rotation(node);
            }
        }
        else
        {
        	node->height = max(height(node->left), height(node->right)) + 1;
        }
    }
    else if(index > node->key && node->right != NULL)
    {
        node->right = delete_avl(node->right, index - node->key);
        if(node->right == NULL)
        {
            mid = node->left;
            node->left = mid->left;
            node->right = mid->right;
            node->key = mid->key;
            node->_text = mid->_text;
            node->height = mid->height;
            free(mid);
            mid = NULL;
        }
        if(height(node->left) - height(node->right) == 2)
        {
            temp = node->left;
            if(height(temp->right) > height(temp->left))
            {
                node = left_right_rotation(node);
            }
            else
            {
                node = right_rotation(node);
            }
        }
        else
        {
        	node->height = max(height(node->left), height(node->right)) + 1;
        }
    }
    else
    {
        ans_global = node->_text;
        free(node);
        node = NULL;
    }

    return node;
}

int max(int a, int b)
{
    if(a > b)
    {
        return a;
    }
    return b;
}

int height(Node *x)
{
    if(x != NULL)
    {
        return x->height;
    }
    return 0;
}

Node * left_rotation(Node *k1)
{
    Node *k2 = NULL;

    k2 = k1->right;
    k1->right = k2->left;
    k2->left = k1;
    k1->height = max( height(k1->left), height(k1->right)) + 1;
    k2->height = max( height(k2->right), k1->height) + 1;
    k2->key = k2->key + k1->key;

    return k2;
}

Node * right_rotation(Node *k2)
{
    Node *k1 = NULL;

    k1 = k2->left;
    k2->key = k2->key - k1->key;
    k2->left = k1->right;
    k1->right = k2;
    k2->height = max( height(k2->left), height(k2->right)) + 1;
    k1->height = max( height(k1->left), k2->height) + 1;

    return k1;
}

Node * left_right_rotation(Node *k3)
{
    k3->left = left_rotation(k3->left);
    return right_rotation(k3);
}

Node * right_left_rotation(Node *k4)
{
    k4->right = right_rotation(k4->right);
    return left_rotation(k4);
}