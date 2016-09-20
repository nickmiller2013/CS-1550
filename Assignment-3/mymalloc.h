
struct node
{
    char key;		//Will be 0 if the node is free, the size (power of 2) if the node is being used
    struct node *prevNode;
    struct node *nextNode;
};

void *my_buddy_malloc(int size);
void my_free(void *ptr);