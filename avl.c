#include "avl.h"
#include <stdio.h>
#include <stdlib.h>

/****
  Student1 name: Reem Younis
  Student2 name: Majd Farah
  
  Student1 ID: 208593970
  Student2 ID: 316139849
****/

// Utilities
// this function prints both submitters names and ids in this format:
// NAME1
// ID1
// NAME2
// ID2

// additional functions that i've used for implementing the algorithm

AVLNodePtr updateTree(AVLNodePtr root, int balance);                /* updating tree after rotating */
AVLNodePtr LCA(AVLNodePtr root, int firstNumber, int secondNumber); /* find LCA of 2 numbers in tree */
AVLNodePtr find_pre_suc(AVLNodePtr root, int num);                  /* find predecessor or successor of a number or the number itself */
int LCA_to_x1(AVLNodePtr LcaNode, int x1_pre_suc, int x1, int x2);  /* find max y in path from LCA of x1 and x2, to x1 */
int LCA_to_x2(AVLNodePtr LcaNode, int x2_pre_suc, int x2, int x1);  /* find max y in path from LCA of x1 and x2, to x2 */
int Height(AVLNodePtr root);                                        /* return height of node */
int balanceFactor(AVLNodePtr root);                                 /* find balance factor of node */
int return_max_y(AVLNodePtr root);                                  /* update max_y of node */

void submitters()
{
    printf("name student1: Reem Younis\nid student1: 208593970\n");
    printf("name student1: Majd Farah\nid student1: 316139849\n");
}

// Do not change this. You can use but do not touch.
int _max(int x, int y)
{
    return (x < y) ? y : x;
}

// Do not change this. You can use but do not touch.
int _abs(int x)
{
    return (x < 0) ? -x : x;
}

// Operations implementation

AVLNodePtr avl_search(AVLNodePtr root, int x, int y)
{
    if (root == NULL)
    { /* root is empty */
        return NULL;
    }

    else if (x > root->key)
    { /* checking if x is greater than root, if so go check right child */
        return avl_search(root->child[1], x, y);
    }
    else if (x < root->key)
    { /* checking if x is less than root, if so go check left child */
        return avl_search(root->child[0], x, y);
    }
    else if (x == root->key && y == root->y)
    { /* x found */
        return root;
    }
    else
    {
        return NULL;
    }
}

AVLNodePtr avl_insert(AVLNodePtr root, int x, int y)
{
    if (root == NULL) /* reached the right place where x should be added */
    {
        root = new_avl_node(x, y); /* creating new node for x */
        root->max_y = y;
    }

    if (x < root->key) /* root is greater than x so go left */
    {
        if (y > root->max_y)
        {
            root->max_y = y;
        }

        root->child[0] = avl_insert(root->child[0], x, y); /* recursive function */
    }

    else if (x > root->key) /* root is less than x so go right */
    {
        if (y > root->max_y)
        {
            root->max_y = y;
        }

        root->child[1] = avl_insert(root->child[1], x, y); /* recursive function */
    }

    else
    { /* x = root->key */
        return root;
    }

    root->height = 1 + _max(Height(root->child[0]), Height(root->child[1])); /* updating the height of root */
    int balance = balanceFactor(root);                                       /* check root's balance */
    if (_abs(balance) > 1)
    {                                     /* check if tree is imbalanced */
        root = updateTree(root, balance); /* if tree is imbalanced update tree */
    }
    return root; /* return root after inserting x */
}

AVLNodePtr avl_delete(AVLNodePtr root, int x, int y)
{
    if (root == NULL)
    { /* tree is empty or reached a null leaf */
        return NULL;
    }

    if (x < root->key)
    {                                                      /* x is less than root's key, so go left */
        root->child[0] = avl_delete(root->child[0], x, y); /* recursive function */
    }

    else if (x > root->key)
    {                                                      /* x is greater than root's key, so go right */
        root->child[1] = avl_delete(root->child[1], x, y); /* recursive function */
    }

    else if (x == root->key && y == root->y)
    { /* x is equal to root's key*/
        if (root->child[0] == NULL && root->child[1] != NULL)
        { /* tree has one child */
            root = root->child[1];
        }
        else if (root->child[0] != NULL && root->child[0] == NULL)
        { /* tree has one child */
            root = root->child[0];
        }
        else if (root->child[0] == NULL && root->child[1] == NULL)
        { /* tree has no children */
            root = NULL;
        }
        else
        { /* find the first node that is greater than x */
            AVLNodePtr currentNode = root->child[0];

            while (currentNode->child[1] != NULL)
            {
                currentNode = currentNode->child[1];
            }

            root->max_y = return_max_y(root);
            root->key = currentNode->key; /* update root's key */
            root->y = currentNode->y;
            root->child[0] = avl_delete(root->child[0], currentNode->key, currentNode->y); /* recursive function */
        }
    }

    else if (x == root->key && y != root->y)
    {
        return root;
    }

    if (root != NULL)
    {
        root->height = 1 + _max(Height(root->child[0]), Height(root->child[1])); /* update the height of root */
        root->max_y = return_max_y(root);                                        /* update root's max_y */

        int balance = balanceFactor(root); /* check root's balance */

        if (_abs(balance) > 1)
        {                                     /* check if tree is imbalanced */
            root = updateTree(root, balance); /* if tree is imbalanced update tree */
        }
    }

    return root; /* return the updated tree */
}

AVLNodePtr new_avl_node(int x, int y)
{
    AVLNodePtr node = (AVLNodePtr)malloc(sizeof(struct AVLNode));
    if (node == NULL)
    { /* allocating memory failed */
        exit(1);
    }
    node->key = x;
    node->y = y;
    node->child[0] = NULL;
    node->child[1] = NULL;
    node->height = 0;
    return node;
}

void delete_avl_tree(AVLNodePtr root)
{
    if (root == NULL)
    {
        return;
    }
    delete_avl_tree(root->child[0]); /* recursive function */
    delete_avl_tree(root->child[1]); /* recursive function */

    free(root);
}

int avl_range_query(AVLNodePtr root, int x1, int x2)
{
    if (root == NULL || (x2 < x1))
    {
        return -1;
    }

    AVLNodePtr x1_pre_suc = find_pre_suc(root, x1);                   // find x1 or Predecessor or Successor
    AVLNodePtr x2_pre_suc = find_pre_suc(root, x2);                   // find x2 or Predecessor or Successor
    AVLNodePtr LcaNode = LCA(root, x1_pre_suc->key, x2_pre_suc->key); // find LCA of x1 and x2

    if (x1_pre_suc->key != x1 && x2_pre_suc->key != x2) // in case both numbers don't exist
    {
        return -1;
    }

    int left = LCA_to_x1(LcaNode, x1_pre_suc->key, x1, x2);  // find max y of all relevant nodes from LCA to x1
    int right = LCA_to_x2(LcaNode, x2_pre_suc->key, x2, x1); // find max y of all relevant nodes from LCA to x2

    return _max(_max(left, right), LcaNode->y);
}

int balanceFactor(AVLNodePtr root)
{
    if (root == NULL || (root->child[0] == NULL && root->child[1] == NULL)) /* if root is null or both children are null */
    {
        return 0;
    }

    else if (root->child[0] == NULL && root->child[1] != NULL) /* one child is null */
    {
        return -(root->height);
    }
    else if (root->child[1] == NULL && root->child[0] != NULL) /* one child is null */
    {
        return (root->height);
    }
    else /* both children aren't null */
    {
        return (root->child[0]->height - root->child[1]->height);
    }
}

int Height(AVLNodePtr root) /* return Height of root */
{
    if (root == NULL)
        return -1;
    else
    {
        return root->height;
    }
}

AVLNodePtr LL(AVLNodePtr root) /* rotate right */
{

    AVLNodePtr tmp = root->child[0];
    AVLNodePtr child = root->child[0]->child[1];
    tmp->child[1] = root;
    root->child[0] = child;
    root->height = 1 + _max(Height(root->child[0]), Height(root->child[1])); /* updating root's height */
    root->max_y = return_max_y(root);                                        /* updating root's max_y */
    tmp->height = 1 + _max(Height(tmp->child[0]), Height(tmp->child[1]));    /* updating tmp's height */
    tmp->max_y = return_max_y(tmp);                                          /* updating root's max_y */

    return tmp; /* return updated tree */
}

AVLNodePtr RR(AVLNodePtr root) /* rotate left */
{

    AVLNodePtr tmp = root->child[1];
    AVLNodePtr child = root->child[1]->child[0];
    tmp->child[0] = root;
    root->child[1] = child;
    root->height = 1 + _max(Height(root->child[0]), Height(root->child[1])); /* updating root's height */
    root->max_y = return_max_y(root);                                        /* updating root's max_y */
    tmp->height = 1 + _max(Height(tmp->child[0]), Height(tmp->child[1]));    /* updating tmp's height */
    tmp->max_y = return_max_y(tmp);                                          /* updating root's max_y */

    return tmp; /* return updated tree */
}

AVLNodePtr LR(AVLNodePtr root) /* rotate left then right */
{
    root->child[0] = RR(root->child[0]); /* rotate left */
    return LL(root);                     /* rotate right */
}

AVLNodePtr RL(AVLNodePtr root) /* rotate right then left */
{
    root->child[1] = LL(root->child[1]); /* rotate right */
    return RR(root);                     /* rotate left */
}

AVLNodePtr updateTree(AVLNodePtr root, int balance)
{ /* check if tree is balance, if not update it */
    if (balance < -1)
    {
        if ((Height(root->child[1]->child[0]) - Height(root->child[1]->child[1])) <= 0)
        {
            root = RR(root); /* rotate left */
        }
        else
        {
            root = RL(root); /* rotate right then left */
        }
    }

    else if (balance > 1)
    {
        if ((Height(root->child[0]->child[0]) - Height(root->child[0]->child[1])) >= 0)
        {
            root = LL(root); /* rotate right */
        }
        else
        {
            root = LR(root); /* rotate left then right */
        }
    }
    return root; /* return updated root */
}

AVLNodePtr LCA(AVLNodePtr root, int firstNumber, int secondNumber) /* finding LCA of two numbers */
{
    if (root == NULL)
    {
        return NULL;
    }

    if (root->key < firstNumber && root->key < secondNumber) /* both numbers are greater than current node's key */
    {
        return LCA(root->child[1], firstNumber, secondNumber);
    }
    else if (root->key > firstNumber && root->key > secondNumber) /* both numbers are less than current node's key */
    {
        return LCA(root->child[0], firstNumber, secondNumber);
    }
    else /* first number < current node's key < second number */
    {
        return root;
    }
}

AVLNodePtr find_pre_suc(AVLNodePtr root, int num) /* recursive function for finding predecessor or successor of num */
{
    if (root != NULL)
    {
        if ((root->child[0] == NULL && root->child[1] == NULL) || root->key == num || (root->key > num && root->child[0] == NULL) || (root->key < num && root->child[1] == NULL)) /* num is found or predecessor or successor */
        {
            return root;
        }

        if (root->key > num) /* current node's key is greater than num  */
        {
            return find_pre_suc(root->child[0], num);
        }

        if (root->key < num) /* current node's key is less than num  */
        {
            return find_pre_suc(root->child[1], num);
        }
    }

    return NULL;
}

int LCA_to_x1(AVLNodePtr LcaNode, int x1_pre_suc, int x1, int x2) /* recursive function for finding max y of all relevant nodes from LCA to x1 */
{
    int currentMax = 0;
    if (LcaNode == NULL)
    {
        return 0;
    }
    else if (LcaNode->key == x1_pre_suc) /* current node's key is equal to x1 pre or suc  */
    {
        if (x1_pre_suc == x1)
        {
            currentMax = LcaNode->y;
        }
        else if (LcaNode->child[1] != NULL && x1_pre_suc < x2 && x1_pre_suc > x1)
        {
            currentMax = _max(LcaNode->y, LcaNode->child[1]->max_y); /* find max between two y and update current max */
        }
        else if ((LcaNode->child[1] != NULL && x1_pre_suc == x2) || (LcaNode->child[1] == NULL))
        {
            currentMax = LcaNode->y;
        }
        else
        {
            currentMax = LcaNode->max_y; /* update current max */
        }
    }

    else if (LcaNode->key < x1_pre_suc)
    {
        currentMax = LCA_to_x1(LcaNode->child[1], x1_pre_suc, x1, x2); /* recursive call */
        if (LcaNode->key > x1_pre_suc)                                 /* current node's key is less than x1 pre or suc  */
        {
            currentMax = _max(currentMax, LcaNode->y); /* find max between two y and updste current max */
        }
    }

    else if (LcaNode->key > x1_pre_suc)
    {
        currentMax = LCA_to_x1(LcaNode->child[0], x1_pre_suc, x1, x2); /* recursive call */
        if (LcaNode->key > x1_pre_suc)                                 /* current node's key is larger than x1 pre or suc  */
        {
            currentMax = _max(currentMax, LcaNode->y); /* find max between two y and update current max */
        }
    }

    return currentMax;
}

int LCA_to_x2(AVLNodePtr LcaNode, int x2_pre_suc, int x2, int x1) /* find max y of all relevant nodes from LCA to x2 */
{
    int currentMax = 0;
    if (LcaNode == NULL)
    {
        return 0;
    }
    else if (LcaNode->key == x2_pre_suc) /* current node's key is equal to x2 pre or suc  */
    {
        if (x2_pre_suc == x2)
        {
            currentMax = LcaNode->y;
        }
        else if (LcaNode->child[0] != NULL && x2_pre_suc > x1 && x2_pre_suc < x2)
        {
            currentMax = _max(LcaNode->y, LcaNode->child[0]->max_y); /* find max between two y and update current max */
        }
        else if ((LcaNode->child[0] != NULL && x2_pre_suc == x1) || LcaNode->child[0] == NULL)
        {
            currentMax = LcaNode->y;
        }
        else
        {
            currentMax = LcaNode->max_y; /* update current max */
        }
    }

    else if (LcaNode->key < x2_pre_suc) /* current node's key is less than x2 pre or suc  */
    {
        currentMax = LCA_to_x2(LcaNode->child[1], x2_pre_suc, x2, x1); /* recursive call */
        if (LcaNode->key < x2_pre_suc)
        {
            currentMax = _max(currentMax, LcaNode->y); /* find max between two y and update current max */
        }
    }

    else if (LcaNode->key > x2_pre_suc) /* current node's key is larger than x2 pre or suc  */
    {
        currentMax = LCA_to_x2(LcaNode->child[0], x2_pre_suc, x2, x1); /* recursive call */
        if (LcaNode->key < x2_pre_suc)
        {
            currentMax = _max(currentMax, LcaNode->y); /* find max between two y and update current max */
        }
    }

    return currentMax;
}

int return_max_y(AVLNodePtr root) /* update root's max_y */
{
    int max;
    if (root->child[1] != NULL && root->child[0] != NULL)
    {
        max = _max(_max(root->child[1]->max_y, root->child[0]->max_y), root->y);
    }
    else if (root->child[1] != NULL && root->child[0] == NULL)
    {
        max = _max(root->y, root->child[1]->max_y);
    }
    else if (root->child[1] == NULL && root->child[0] != NULL)
    {
        max = _max(root->y, root->child[0]->max_y);
    }
    else
    {
        max = root->y;
    }
    return max;
}
