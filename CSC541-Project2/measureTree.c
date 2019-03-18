#include <stdio.h>
#include <stdlib.h>

typedef struct intervalList
{
	int leftend;
	int rightend;
	struct intervalList* next;
}intervalList;

typedef struct MTreeNode  
{
	int key;
	int height;
	int measure;
	int leftmin;
	int rightmax;
	int l;
	int r;
	struct MTreeNode* left;
	struct MTreeNode* right;
	intervalList* head;
}MTreeNode;

typedef struct m_tree_t
{
	MTreeNode* node;
}m_tree_t;

typedef struct st_t {
	MTreeNode* *item;
	struct st_t *next; 
} stack_t;

stack_t *create_stack(void)
{
	stack_t *st;
	st = (stack_t*) malloc(sizeof(stack_t));
	if(st == NULL)
		printf("stack malloc failed!\n");
	st->next = NULL;
	return(st);
}

int stack_empty(stack_t *st)
{
	return( st->next == NULL );
}

void push(MTreeNode* x, stack_t *st)
{
	stack_t *tmp;
	tmp = (stack_t*) malloc(sizeof(stack_t));
	if(tmp == NULL)
		printf("stack malloc failed!\n");
  	tmp->item = x;
 	tmp->next = st->next;
  	st->next = tmp;
}

MTreeNode* pop(stack_t *st)
{
	stack_t *tmp; MTreeNode* tmp_item;
	tmp = st->next;
	st->next = tmp->next;
	tmp_item = tmp->item;
	free(tmp);
	return(tmp_item);
}

MTreeNode* top(stack_t *st)
{
	return(st->next->item);
}

void remove_stack(stack_t *st)
{
	stack_t *tmp;
	do
	    { tmp = st->next;
			free(st);
	      st = tmp;
	    }
	while (tmp != NULL);
}

void addNewInterval(MTreeNode* node, intervalList* newInterval)
{
	intervalList* newnode = NULL;
	newnode = (intervalList*)malloc(sizeof(intervalList));
	newnode->leftend = newInterval->leftend;
	newnode->rightend = newInterval->rightend;
	newnode->next = NULL;
	if(node->head == NULL)
	{
		node->head = newnode;
	}
	else
	{
		newnode->next = node->head->next;
		node->head->next = newnode;
	}
}

void deleteInterval(MTreeNode* node, intervalList* interval)
{
	intervalList* temp = node->head;
	intervalList* prev = NULL;

	if(temp->leftend == interval->leftend && temp->rightend == interval->rightend)
	{
		node->head = temp->next;
		free(temp);
	}
	else
	{
		prev = temp;
		temp = temp->next;
		while(temp != NULL)
		{
			if(temp->leftend == interval->leftend && temp->rightend == interval->rightend)
			{
				prev->next = temp->next;
				free(temp);
				break;
			}
			prev = prev->next;
			temp = temp->next;
		}
	}
}

int getLeftMin(intervalList* head) 
{
	int min = head->leftend;

	head = head->next;
  	while(head != NULL)
  	{
  		if(head->leftend < min)
  		{
  			min = head->leftend;
  		}
    	head = head->next;
  	}

  	return min;
}

int getRightMax(intervalList* head) 
{
	int max = head->rightend;

  	head = head->next;
  	while(head != NULL)
  	{
  		if(head->rightend > max)
  		{
  			max = head->rightend;
  		}
    	head = head->next;
  	}

  	return max;
}

void updateLeafMeasure(MTreeNode* node) 
{
	int left = 0;
	int right = 0;

	if(node->leftmin < node->l)
	{
		left = node->l;
	}
	else
	{
		left = node->leftmin;
	}

	if(node->rightmax > node->r)
	{
		right = node->r;
	}
	else
	{
		right = node->rightmax;
	}

	node->measure = right - left;
}

void updateInternalMeasure(MTreeNode* node)
{
	if(node->right->leftmin < node->l && node->left->rightmax >= node->r)
	{
		node->measure = node->r - node->l;
	}
	else if(node->right->leftmin >= node->l && node->left->rightmax >= node->r)
	{
		node->measure = node->r - node->key + node->left->measure;
	}
	else if(node->right->leftmin < node->l && node->left->rightmax < node->r)
	{
		node->measure = node->right->measure + node->key - node->l;
	}
	else
	{
		node->measure = node->right->measure + node->left->measure;
	}
}

int min(int a, int b)
{
	if(a < b)
	{
		return a;
	}
	else
	{
		return b;
	}
}

int max(int a, int b)
{
	if(a > b)
	{
		return a;
	}
	else
	{
		return b;
	}
}

int height(MTreeNode* x)
{
    if(x != NULL)
    {
        return x->height;
    }
    return 0;
}

MTreeNode* left_rotation(MTreeNode *n)
{ 
  MTreeNode *node;
  int tmp_key;

  node = n->left;
  tmp_key = n->key;
  n->left = n->right;
  n->key = n->right->key;
  n->right = n->left->right;
  n->left->right = n->left->left;
  n->left->left = node;
  n->left->key = tmp_key;

  n->left->l = n->l;
  n->left->r = n->key;

  n->left->leftmin = min(n->left->left->leftmin,n->left->right->leftmin); 
  n->left->rightmax = max(n->left->left->rightmax,n->left->right->rightmax);
  updateInternalMeasure(n->left);

  return n;
}

MTreeNode* right_rotation(MTreeNode *n)
{ 
  MTreeNode *node;
  int tmp_key;

  node = n->right;
  tmp_key = n->key;
  n->right = n->left;
  n->key = n->left->key;
  n->left = n->right->left;
  n->right->left = n->right->right;
  n->right->right = node;
  n->right->key = tmp_key;

  n->right->l = n->key;
  n->right->r = n->r;

  n->right->leftmin = min(n->right->left->leftmin,n->right->right->leftmin); 
  n->right->rightmax = max(n->right->left->rightmax,n->right->right->rightmax);
  updateInternalMeasure(n->right);

  return n;
}

MTreeNode* insertAtLeaf(MTreeNode* node,int ikey,intervalList* newInterval) {
	MTreeNode *new_node, *old_node;

    new_node = (MTreeNode* )malloc(sizeof(MTreeNode));
    new_node->head = NULL; 
    addNewInterval(new_node, newInterval);
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->key = ikey;
    new_node->height = 0;
    new_node->leftmin = newInterval->leftend;
    new_node->rightmax = newInterval->rightend;

    old_node = (MTreeNode* )malloc(sizeof(MTreeNode));
    old_node->head = node->head;
    old_node->left = NULL;
    old_node->right = NULL;
    old_node->key = node->key;
    old_node->height = 0;
    old_node->leftmin = node->leftmin;
    old_node->rightmax = node->rightmax;

    if (node->key < ikey) {
    	node->left = old_node;
    	node->right = new_node;
    	node->key = ikey;
    	old_node->l = node->l;
    	old_node->r = node->key;
    	new_node->l = node->key;
    	new_node->r = node->r;
    } else {
    	node->left = new_node;
    	node->right = old_node;
    	new_node->l = node->l;
    	new_node->r = node->key;
    	old_node->l = node->key;
    	old_node->r = node->r;
    }
    updateLeafMeasure(old_node);
    updateLeafMeasure(new_node);
    
    node->height = 1;
    node->head = NULL;
    node->leftmin = min(node->left->leftmin, node->right->leftmin);
    node->rightmax = max(node->left->rightmax, node->right->rightmax); 

    return node;  
}

MTreeNode* insert(MTreeNode* node, int ikey, intervalList* newInterval)
{
	MTreeNode* tmp_node = NULL;
	stack_t *s = create_stack();
	stack_t *rotate_s = create_stack();
	int end = 0;
	int t_height = 0;
	int o_height = 0;

	if (node == NULL) {
		node = (MTreeNode*)malloc(sizeof(MTreeNode));
		node->key = ikey;
		node->height = 0;
		node->leftmin = newInterval->leftend;
		node->rightmax = newInterval->rightend;
		node->l = -2147483648;
		node->r = 2147483647;
		node->left = NULL;
		node->right = NULL;
		node->head = NULL;
		addNewInterval(node, newInterval);
		updateLeafMeasure(node);
		return node;
	} else {
		tmp_node = node;
		while (tmp_node->right != NULL) {
			push(tmp_node, s);
			push(tmp_node, rotate_s);
			if (ikey < tmp_node->key) {
				tmp_node = tmp_node->left;
			} else {
				tmp_node = tmp_node->right;				
			}
		}
		if (ikey == tmp_node->key && tmp_node->head != NULL) {
	    	addNewInterval(tmp_node, newInterval);
	    	tmp_node->leftmin = min(tmp_node->leftmin, newInterval->leftend);
	    	tmp_node->rightmax = max(tmp_node->rightmax, newInterval->rightend);
	        updateLeafMeasure(tmp_node);			
		} else {
			tmp_node = insertAtLeaf(tmp_node, ikey, newInterval);
	        tmp_node->leftmin = min(tmp_node->left->leftmin, tmp_node->right->leftmin);
	        tmp_node->rightmax = max(tmp_node->left->rightmax, tmp_node->right->rightmax);
	        updateInternalMeasure(tmp_node); 
			}
		while(!stack_empty(s))
		{
		  	tmp_node = top(s);
			pop(s);
			updateInternalMeasure(tmp_node);
		  	tmp_node->leftmin = min(tmp_node->left->leftmin,tmp_node->right->leftmin); 
		  	tmp_node->rightmax = max(tmp_node->left->rightmax,tmp_node->right->rightmax);       
		}

      	while(!stack_empty(rotate_s) && !end)
		{
			tmp_node = top(rotate_s);
		  	pop(rotate_s);
		  	o_height = tmp_node->height;
		  	if(tmp_node->left->height - tmp_node->right->height == 2 )
		    { 
		      	if( tmp_node->left->left->height - tmp_node->right->height == 1 )
				{ 
			  		tmp_node = right_rotation( tmp_node );
			  		tmp_node->right->height = tmp_node->right->left->height + 1;
			  		tmp_node->height = tmp_node->right->height + 1;
				}
		      	else
				{ 
			  		tmp_node->left = left_rotation( tmp_node->left );
			  		tmp_node = right_rotation( tmp_node );
			  		t_height = tmp_node->left->left->height;
			  		tmp_node->left->height = t_height + 1;
			  		tmp_node->right->height = t_height + 1;
			  		tmp_node->height = t_height + 2;
				}
		    }
		  	else if(tmp_node->left->height - tmp_node->right->height == -2 )
		    { 
		      	if( tmp_node->right->right->height - tmp_node->left->height == 1 )
				{ 
			  		tmp_node = left_rotation( tmp_node );
			  		tmp_node->left->height = tmp_node->left->right->height + 1;
			  		tmp_node->height = tmp_node->left->height + 1;
				}
		      	else
				{ 
			  		tmp_node->right = right_rotation( tmp_node->right );
			  		tmp_node = left_rotation( tmp_node );
			  		t_height = tmp_node->right->right->height;
			  		tmp_node->left->height = t_height + 1;
			  		tmp_node->right->height = t_height + 1;
			  		tmp_node->height = t_height + 2;
				}
		    }
		  	else
		    { 
		      	if( tmp_node->left->height > tmp_node->right->height )
			  	{
			    	tmp_node->height = tmp_node->left->height + 1;
			  	}
		     	else
			 	{
			   		tmp_node->height = tmp_node->right->height + 1;
			 	}
		    }
		  	if( tmp_node->height == o_height )
		  	{
		    	end = 1;
		  	}
		}
	}
	remove_stack(s);
	remove_stack(rotate_s);
	return node;
}

MTreeNode* delete(MTreeNode* node, int dkey, intervalList* interval)
{
 	MTreeNode *tmp_node, *upper_node, *other_node;
 	int end = 0;
	int t_height = 0;
	int o_height = 0;
  	stack_t *s = create_stack();
  	stack_t *rotate_s = create_stack();
  	if( node == NULL )
  	{
  		;
  	}
  	else if( node->right == NULL && node->left == NULL )
   	{  
    	if(  node->key == dkey )
		{ 
	  		free(node);
	  		node = NULL;	
		} 
    }
  	else
    {  
    	tmp_node = node;
      	while( tmp_node->right != NULL && tmp_node->left != NULL)
		{   
			upper_node = tmp_node;
	  		push(tmp_node, s);
	  		push(tmp_node, rotate_s);
      		if( dkey < tmp_node->key )
	    	{  
	    		tmp_node   = upper_node->left; 
	      		other_node = upper_node->right;
	    	} 
          	else
	    	{  
	    		tmp_node   = upper_node->right; 
	      		other_node = upper_node->left;
	    	} 
		}
      	if( tmp_node->key != dkey )
			return node;

      	deleteInterval(tmp_node, interval);

      	if(tmp_node->head == NULL)
		{  
			upper_node->key   = other_node->key;
	  		upper_node->left  = other_node->left;
	  		upper_node->right = other_node->right;
	  		upper_node->leftmin = other_node->leftmin;
	  		upper_node->rightmax = other_node->rightmax;
	  		upper_node->measure = other_node->measure;
	  		upper_node->height = other_node->height;
	  		upper_node->head = other_node->head;
	 
	  		if(upper_node->right != NULL)
	    	{
	      		upper_node->right->r = upper_node->r;
	      		upper_node->left->l = upper_node->l;
	   		}	     
	   		else
	   		{
	  			updateLeafMeasure(upper_node);
	  		}
	  		free(tmp_node);
	  		free( other_node );
	  		tmp_node = NULL;
	  		other_node = NULL;
	  		pop(s);
	  		pop(rotate_s);
		}
      	else
		{
	  		tmp_node->leftmin = getLeftMin(tmp_node->head);
	  		tmp_node->rightmax = getRightMax(tmp_node->head);
	  		updateLeafMeasure(tmp_node);
		}

      	while(!stack_empty(s))
		{
	  		tmp_node = top(s);
	  		pop(s);
	  		updateInternalMeasure(tmp_node);
	  		tmp_node->leftmin = min(tmp_node->left->leftmin,tmp_node->right->leftmin); 
	  		tmp_node->rightmax = max(tmp_node->left->rightmax,tmp_node->right->rightmax);         
		}

      	while(!stack_empty(rotate_s) && !end)
		{
		  	tmp_node = top(rotate_s);
		  	pop(rotate_s);
		  	o_height= tmp_node->height;
		  	if( tmp_node->left != NULL && tmp_node->right != NULL && tmp_node->left->left != NULL && tmp_node->left->height - tmp_node->right->height == 2 )
		    { 
		      	if( tmp_node->left->left->height - tmp_node->right->height == 1 )
				{ 
					tmp_node = right_rotation( tmp_node );
					tmp_node->right->height = tmp_node->right->left->height + 1;
					tmp_node->height = tmp_node->right->height + 1;
				}
		      	else
				{ 
			  		tmp_node->left = left_rotation( tmp_node->left );
			  		tmp_node = right_rotation( tmp_node );
			  		t_height = tmp_node->left->left->height;
			  		tmp_node->left->height = t_height + 1;
			  		tmp_node->right->height = t_height + 1;
			  		tmp_node->height = t_height + 2;
				}
		    }
		  	else if( tmp_node->left != NULL && tmp_node->right != NULL && tmp_node->right->right != NULL && tmp_node->left->height - tmp_node->right->height == -2 )
		    { 
		      	if( tmp_node->right->right->height - tmp_node->left->height == 1 )
				{ 
			  		tmp_node = left_rotation( tmp_node );
			  		tmp_node->left->height = tmp_node->left->right->height + 1;
			  		tmp_node->height = tmp_node->left->height + 1;
				}
		      	else
				{ 
			  		tmp_node->right = right_rotation( tmp_node->right );
			  		tmp_node = left_rotation( tmp_node );
			  		t_height = tmp_node->right->right->height;
			  		tmp_node->left->height = t_height + 1;
			  		tmp_node->right->height = t_height + 1;
			  		tmp_node->height = t_height + 2;
				}
		    }
		  	else if( tmp_node->left != NULL && tmp_node->right != NULL)
		    { 
		      	if( tmp_node->left->height > tmp_node->right->height )
			  	{
			    	tmp_node->height = tmp_node->left->height + 1;
			  	}
		     	else
			 	{
			   		tmp_node->height = tmp_node->right->height + 1;
			 	}
		    }
		  	if( tmp_node->height == o_height )
		    	end = 1;

		}
	}
  	remove_stack(s);
  	remove_stack(rotate_s);
  	return node;
}

m_tree_t * create_m_tree()
{
	m_tree_t* root;

	root = (m_tree_t*)malloc(sizeof(m_tree_t));
	root->node = NULL;

	return root;
}

void insert_interval(m_tree_t * tree, int a, int b)
{
	intervalList* interval = (intervalList*)malloc(sizeof(intervalList));

	interval->leftend = a;
	interval->rightend = b;
	interval->next = NULL;
	tree->node = insert(tree->node, a, interval);
	tree->node = insert(tree->node, b, interval);
	free(interval);
}

void delete_interval(m_tree_t * tree, int a, int b)
{
	intervalList* interval = (intervalList*)malloc(sizeof(intervalList));

	interval->leftend = a;
	interval->rightend = b;
	interval->next = NULL;
	tree->node = delete(tree->node, a, interval);
	tree->node = delete(tree->node, b, interval);
	free(interval);
}

int query_length(m_tree_t * tree)
{
	if(tree->node != NULL)
	{
		return tree->node->measure;
	}
	else
	{
		return 0;
	}
}



int main()
{  int i; m_tree_t *t; ;
   printf("starting \n");
   t = create_m_tree();
   for(i=0; i< 50; i++ )
      insert_interval( t, 2*i, 2*i +1 );
   printf("inserted first 50 intervals, total length is %d, should be 50.\n", query_length(t));
      insert_interval( t, 0, 100 );
   printf("inserted another interval, total length is %d, should be 100.\n", query_length(t));
   for(i=1; i< 50; i++ )
     insert_interval( t, 199 - (3*i), 200 ); /*[52,200] is longest*/
   printf("inserted further 49 intervals, total length is %d, should be 200.\n", query_length(t));
   for(i=2; i< 50; i++ )
     delete_interval(t, 2*i, 2*i +1 );
   delete_interval(t,0,100);
   printf("deleted some intervals, total length is %d, should be 150.\n", query_length(t));
   insert_interval( t, 1,2 ); 
   for(i=49; i>0; i-- )
     delete_interval( t, 199 - (3*i), 200 ); 
   insert_interval( t, 0,2 );
   insert_interval( t, 1,5 );  
   printf("deleted some intervals, total length is %d, should be 5.\n", query_length(t));
   insert_interval( t, 0, 100 );
   printf("inserted another interval, total length is %d, should be 100.\n", query_length(t));
   for(i=0; i<=3000; i++ )
      insert_interval( t, 2000+i, 3000+i ); 
   printf("inserted 3000 intervals, total length is %d, should be 4100.\n", query_length(t));
   for(i=0; i<=3000; i++ )
     delete_interval( t, 2000+i, 3000+i ); 
   printf("deleted 3000 intervals, total length is %d, should be 100.\n", query_length(t));
   for(i=0; i<=100; i++ )
      insert_interval( t, 10*i, 10*i+100 ); 
   printf("inserted another 100 intervals, total length is %d, should be 1100.\n", query_length(t));
   delete_interval( t, 1,2 ); 
   delete_interval( t, 0,2 ); 
   delete_interval( t, 2,3 ); 
   delete_interval( t, 0,1 ); 
   delete_interval( t, 1,5 );
   printf("deleted some intervals, total length is %d, should be still 1100.\n", query_length(t)); 
   for(i=0; i<= 100; i++ )
     delete_interval(t, 10*i, 10*i+100);
   delete_interval(t,0,100);
   printf("deleted last interval, total length is %d, should be 0.\n", query_length(t));
   for( i=0; i<100000; i++)
   {
   		insert_interval(t, i, 1000000);
   }
   printf("inserted again 100000 intervals, total length is %d, should be 1000000.\n", query_length(t));
   printf("End Test\n");
}



