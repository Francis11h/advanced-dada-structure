

struct text_t{
    struct text_t *left;
    struct text_t *right;
    int key;		//# of leaves in its left child
    int height;		//height of the AVL tree
    char* _text;
};

//gets the line of number index, if such a line exists, and returns NULL else.
char * get_line( text_t *txt, int index) {
	text_t *tmp;
	if (txt == NULL) {
		return NULL;
	} else {
		tmp = txt;
		while (tmp->right != NULL) {	//until reach a leaf
			if (index <= tmp->key) {
				tmp = tmp->left;
			} else {
				tmp = tmp->right;
				index -= tmp->key;
			}
		}
		if (tmp->key == 0 && index == 1) { //leaf and index match
			return tmp->_text;
		} else {
			return NULL;
		}
	}
}


// sets the line of number index to new line. 
// and returns a pointer to the previous line of that number.

char * set_line( text_t *txt, int index, char * new_line) {
	char* t;
	t = get_line(txt, index);
	if (t == NULL) {
		return NULL;
	} else {
		t = new_line;
	}
	return t;
}

//inserts the line before the line of number index to new line,
//renumbering all lines after that line
//If no such line exists, it appends new line as new last line.
void insert_line( text_t *txt, int index, char * new_line) {
	//insert
	char* t;
	text_t *tmp;

	t = get_line(txt, index);
	if (t == NULL) {
		append_line(txt, new_line);
	} else {
		create_stack();
		tmp = txt;

		while (tmp->right != NULL) {
			push(tmp);					//每往下走一个点，走过的点压push入栈中。
			if (index <= tmp->key) {
				tmp = tmp->left;
			} else {
				tmp = tmp->right;
				index -= tmp->key;
			}
		}
		//node = find_line(txt, index); //find the node of index， return text_t *

		text_t *old, *new; //new two nodes
		old->_text = tmp->_text;
		new->_text = new_line;

		tmp->left = new;
		tmp->right = old;
		tmp->key = 1;
		tmp->height = 1;
		tmp->_text = NULL;
	}

	//rebalance
	int finished = 0;
	while (!stack_empty() && !finished) {
		int tmp_height, old_height;
		tmp = pop();			
		old_height = tmp->height;	//弹出点的原来高度

		if (tmp->left->height - tmp->right->height == 2) {
			if (tmp->left->left->height - tmp->right->height == 1) {
				right_rotation(tmp);
				tmp->right->height = tmp->right->left->height + 1;
				tmp->height = tmp->right->height + 1;
			} else {
				left_rotation(tmp->left);
				right_rotation(tmp);
				tmp_height = 						
					tmp->left->left->height;	//把之前要变的点的左左height存下
				tmp->left->height =		
					tmp_height + 1;					//老得点的左边高度 = 老的左左+1，因为平衡
				tmp->right->height =
					tmp_height + 1;					//老得点的右边高度 = 老的左左+1，因为平衡
				tmp->height = tmp_height + 2;	//老得点的位置的高度 = 老的左左+2，因为平衡
			
			}
		} else if (tmp->left->height - tmp->right->height == -2){
			if (tmp->right->right->height - tmp->left->height == 1) {
				left_rotation(tmp);


			} else {
				right_rotation(tmp->right);
				left_rotation(tmp);


			}
		} else {
			if (tmp->left->height > tmp->right->height) {
				tmp->height = tmp->left->height + 1;
			} else {
				tmp->height = tmp->right->height + 1;				
			}
		}

		if (tmp->height == old_height) { //高度没有变化的话 本来就平衡
			finished = 1;
		}
	}
}
