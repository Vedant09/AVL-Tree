/*
Vedant V Yelsangikar
G01379948
CS531
AVL Tree Implementation
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct address_t
{
	int octet[4];
    char alias[20];
    struct address_t *left, *right, *parent;
    int height, depth;
};
struct address_t *root = NULL; // root node
int n=1;


// function to get the height of the tree
int height(struct address_t *node)
{
	if (node == NULL)
		return 0;
	return node->height;
}

// Function to set the depth
int depth(struct address_t *node, int d,int max)
{
    struct address_t *pnode = node;
        pnode->depth = d;
    
    d=d+1;
    if (pnode->left != NULL)
        depth(pnode->left, d,max);
    if (pnode->right != NULL)
        depth(pnode->right, d,max);
    return 0;
}

// Function to set height and depth
void set(struct address_t *main)
{

    if (main != NULL)
    {
        set(main->left);
       // height(root);
        depth(root, 0,0);
        set(main->right);
    }
}

//Function to get maximum of two integers
int max(int a, int b)
{
	return (a > b)? a : b;
}

// Function to calculate the balance factors of the node
int rebalance(struct address_t *node,int d,int max) {
    struct address_t *pnode = node;
    int l,r=0;
    d++; 
    if (pnode!=NULL){
    if(d>max){
       max=d;
    }
    else{
    }
    if (pnode->left != NULL){
        int m=rebalance(pnode->left, d,max);
        if(m>max){
            max=m;
        }
    } 
    if (pnode->right != NULL){
        int m=rebalance(pnode->right, d, max);
        if(m>max){
            max=m;
        }
    }
   }
    return max;
}


//Funtion to add a New Node
struct address_t* newNode(struct address_t *parent,char *alias,int *octet)
{
	struct address_t* node = (struct address_t*)malloc(sizeof(struct address_t));
	strcpy(node->alias,alias);
    node->parent=parent;
    node->octet[0]=octet[0];
    node->octet[1]=octet[1];
    node->octet[2]=octet[2];
    node->octet[3]=octet[3];
	node->left = NULL;
	node->right = NULL;
	node->height = 1;
	return(node);
}

//Function to right rotate
struct address_t *rightRotate(struct address_t *y)
{
	struct address_t *x = y->left;
	struct address_t *T2 = x->right;

	//rotation
	x->right = y;
	y->left = T2;

	// Update heights
	y->height = max(height(y->left), height(y->right)) + 1;
	x->height = max(height(x->left), height(x->right)) + 1;
	return x;
}

//Function to left rotate 
struct address_t *leftRotate(struct address_t *x)
{
	struct address_t *y = x->right;
	struct address_t *T2 = y->left;

	//rotation
	y->left = x;
	x->right = T2;

	// Update heights
	x->height = max(height(x->left), height(x->right)) + 1;
	y->height = max(height(y->left), height(y->right)) + 1;
	return y;
}

//Function to rotate right then left
struct address_t* rotateRightThenLeft(struct address_t* node){
    node->right = rightRotate(node->right);
	return leftRotate(node);
}

//Function to rotate left then right
struct address_t* rotateLeftThenRight(struct address_t* node){
    node->left = leftRotate(node->left);
	return rightRotate(node);
}


// Get Balance factor of node N
int getBalance(struct address_t *pnode)
{
	if (pnode == NULL)
		return 0;
	return height(pnode->left) - height(pnode->right);
}

//Function to insert node recursively
struct address_t* insertnode(struct address_t* node, int *octet,char *alias,struct address_t *parent)
{

	if (node == NULL)
		return(newNode(parent,alias,octet));

	if (strcmp(alias,node->alias)<0)
		node->left = insertnode(node->left, octet,alias,node);
	else if (strcmp(alias,node->alias)>0)
		node->right = insertnode(node->right, octet,alias,node);
	else 
		return node;

	node->height = 1 + max(height(node->left), height(node->right));

	int balance = getBalance(node);

	// Left Left Rotate Call
	if (balance > 1 && strcmp(alias,node->left->alias)<0)
		return rightRotate(node);

	// Right Right Rotate Call
	if (balance < -1 && strcmp(alias,node->right->alias)>0)
		return leftRotate(node);

	// Left Right Rotate Call
	if (balance > 1 && strcmp(alias,node->left->alias)>0)
	{
		return rotateLeftThenRight(node);
	}

	// Right Left Rotate Call
	if (balance < -1 && strcmp(alias,node->right->alias)<0)
	{
		return rotateRightThenLeft(node);
	}
	return node;
}


// Function to read the content from the file
void readFile()
{
    FILE *fp;
    char k[200], alias[30];
    int i[4] = {0};
    struct address_t *node=root;
    fp = fopen("CS531_Inet.txt", "r");
    if (fp == NULL)
    {
        printf("File cannot be opened \n");
    }
    else
    {
        printf("content of this files are \n");
        while (fgets(k, 10000, fp) != NULL)
        {
            sscanf(k, "%d.%d.%d.%d %s", i, i + 1, i + 2, i + 3, alias);
            root = insertnode(root, i, alias,NULL);
        }
        fclose(fp);
    }
}

// Function to check for duplicate IP address
int valid_ip(struct address_t *main, int *t)
{

    if (main != NULL)
    {
        valid_ip(main->left, t);
        if ((main->octet[0] == t[0]) && (main->octet[1] == t[1]) && (main->octet[2] == t[2]) && (main->octet[3] == t[3]))
        {
            n = 0;
        }
        valid_ip(main->right, t);
    }
    return n;
}

// Function used for display alias
int lookUp(struct address_t *node, char *alias)
{
    if (node != NULL)
    {
        lookUp(node->left, alias);
        if (strcmp(node->alias, alias) == 0)
        {
            n = 0;
            printf("The address of the Alias: : %d.%d.%d.%d\n", node->octet[0], node->octet[1], node->octet[2], node->octet[3]);
        }
        lookUp(node->right, alias);
    }
    return n;
}

// Function to update the IP address
void change(struct address_t *node, char *alias, int *t)
{
    if (node != NULL)
    {
        change(node->left, alias, t);
        if (strcmp(node->alias, alias) == 0)
        {

            node->octet[0] = t[0];
            node->octet[1] = t[1];
            node->octet[2] = t[2];
            node->octet[3] = t[3];
        }
        change(node->right, alias, t);
    }
}

// Function to update the IP address for given alias
void updateAddress()
{

    char it[100], alias[100];
    int t[4], l, m;
    struct address_t *node = root;
    struct address_t *node1 = root;
    struct address_t *node2 = root;
    printf("Enter the name of alias whose address has to be updated : ");
    scanf("%s", alias);
    printf("Enter the new ip address to update : ");
    scanf("%s", it);
    sscanf(it, "%d.%d.%d.%d", t, t + 1, t + 2, t + 3);
    l = valid_ip(node1, t);
    n = 1;
    m = lookUp(node2, alias);
    n = 1;
    if (l == 1)
    {
        if (m == 0)
        {
            change(node, alias, t);
        }
        else
        {
            printf("The alias does not exsist");
        }
    }
    else
    {
        printf(" Address already present");
    }
}

// Function to Display
void displayList(struct address_t *main,struct address_t *parent)
{
    int l,r,max=0;
    if (main != NULL)
    {
        displayList(main->left,main);
        l = rebalance(main->left, 0, 0);    //calculate number of node in the left of tree
        r = rebalance(main->right, 0, 0);   //calculate number of node in the of right tree
        max = l-r;                          //calculate balance factor 
        if (parent == NULL)
        {
            printf("%d.%d.%d.%d\t%s\tHeight : %d\tDepth : %d\tParent : None\tBalance factor : %d\n", main->octet[0], main->octet[1], main->octet[2], main->octet[3], main->alias, main->height-1, main->depth, max);
        }
        else
        {
            printf("%d.%d.%d.%d\t%s\tHeight : %d\tDepth : %d\tParent : %s\tBalance factor : %d\n", main->octet[0], main->octet[1], main->octet[2], main->octet[3], main->alias, main->height-1, main->depth, parent->alias, max);
        }
        displayList(main->right,main);
    }
}

// Function to print the IP address for requested alias
void searchAddress()
{
    int h;
    char alias[100];
    struct address_t *node = root;
    printf("Enter the alias for which IP address has to be founded : ");
    scanf("%s", alias);
    h = lookUp(node, alias);
    if (h == 1)
    {
        printf("Alias name not found");
    }
}

// Function to add new IP address
void addAddress()
{
    int i, k;
    i = 1;
    k = 1;
    int t[4];
    char inputip[100], alias[30];
    struct address_t *node = root;
    struct address_t *node1 = root;
    printf("Enter IPV4 address to add : ");
    scanf("%s", inputip);
    sscanf(inputip, "%d.%d.%d.%d", t, t + 1, t + 2, t + 3);
    printf("Enter Alias : ");
    scanf("%s", alias);
    i = lookUp(node, alias);
    if (i == 1)
    {
        n = 1;
        k = valid_ip(node1, t);
        if (k == 1)
        {
            root=insertnode(root, t, alias,NULL);
        }
        else
        {
            printf(" Address is already present\n");
        }
    }
    else
    {
        printf(" Alias is already presents\n");
    }
}

// Function to save the file
void File(char *fp, struct address_t *node)
{

    FILE *test;
    test = fopen(fp, "a+");

    if (node != NULL)
    {
        File(fp, node->left);
        fprintf(test, "%d.%d.%d.%d %s\n", node->octet[0], node->octet[1], node->octet[2], node->octet[3], node->alias);
        File(fp, node->right);
    }
    printf("\nFile successfully saved to %s\n", fp);
    fclose(test);
}

// Function to save the file
void saveAddressToFile()
{
    struct address_t *node = root;
    char fp[200];
    printf("\nEnter the File Name : ");
    scanf("%s", fp);
    File(fp, node);
}

// Function to iterate in display option
void display(struct address_t *node, int *t)
{
    if (node != NULL)
    {
        display(node->left, t);
        if (node->octet[0] == t[0] && node->octet[1] == t[1])
        {
            printf("alias is %s\n", node->alias);
        }
        display(node->right, t);
    }
}

// Function to display the alias
void diplayAlias()
{
    struct address_t *node;
    node = root;
    int t[2];
    char it[100];
    printf("Enter the IP location : ");
    scanf("%s", it);
    sscanf(it, "%d.%d", t, t + 1);
    display(node, t);
}


// Function to find minimum among right sub tree
struct address_t * minValueNode(struct address_t* node)
{
	struct address_t* current = node;

	while (current->left != NULL)
		current = current->left;

	return current;
}

// Function to delete the node
struct address_t* deleteNode(struct address_t* node, char *alias)
{
	if (node == NULL)
		return node;

	if (strcmp(alias,node->alias)<0)
		node->left = deleteNode(node->left, alias);

	else if(strcmp(alias,node->alias)>0)
		node->right = deleteNode(node->right, alias);

	else
	{
		if( (node->left == NULL) || (node->right == NULL) )
		{
			struct address_t *temp = node->left ? node->left : node->right;

			if (temp == NULL)
			{
				temp = node;
				node = NULL;
			}
			else 
			*node = *temp;
			free(temp);
		}
		else
		{
			struct address_t* temp = minValueNode(node->right);
			strcpy(node->alias, temp->alias);
			node->right = deleteNode(node->right, temp->alias);
		}
	}
	if (node == NULL)
	return node;

	node->height = 1 + max(height(node->left), height(node->right));

	int balance = getBalance(node);

	// Left Left Case
	if (balance > 1 && getBalance(node->left) >= 0)
		return rightRotate(node);

	// Left Right Case
	if (balance > 1 && getBalance(node->left) < 0)
	{
		return rotateLeftThenRight(node);
	}

	// Right Right Case
	if (balance < -1 && getBalance(node->right) <= 0)
		return leftRotate(node);

	// Right Left Case
	if (balance < -1 && getBalance(node->right) > 0)
	{
		return rotateRightThenLeft(node);
	}

	return node;
}

void deleteAddress()
{
    char alias[100];
    struct address_t *newnode = root;
    struct address_t *newnode1;
    printf("Enter the alias name to delete : ");
    scanf("%s", alias);
    newnode1 = deleteNode(newnode, alias);
    root = newnode1;
}

//Function to Quit program
int quitProgram(){ 
   return 0;
}

//main funtion
int main()
{
    readFile();
    struct address_t *temp;
    temp = root;
    struct address_t *temp1;
    temp1 = root;
    int choice,r = 1;
    struct address_t *re=root;
    while(r){
        printf("\nSelect a number for the options below :\n\t1.Add address\n\t2.Look up address\n\t3.Update address\n\t4.Delete address\n\t5.Display list\n\t6.Display aliases for location\n\t7.Save to file\n\t8.Quit\nEnter your choice : ");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            printf("Add address choice\n");
            addAddress(); // Method call to add Address
            n = 1;
            break;
        case 2:
            printf("Look up address\n");
            searchAddress(); // Method call to search Address
            n = 1; 
            break;
        case 3:
            printf("Update address\n");
            updateAddress(); // Method call to update Address
            break;
        case 4:
            printf("Delete address\n");
            deleteAddress(); // Method call to delete Address
            break;
        case 5:
            printf("Display list\n");
            set(temp1);
            displayList(root,NULL); // Method call to display
            break;
        case 6:
            printf("Display aliases for location\n");
            diplayAlias();
            break;
        case 7:
            printf("Save to file\n");
            saveAddressToFile(); // Method call to save to a file
            break;
        case 8:
            printf("Quit\n");
            r = quitProgram(); // Method call to terminate
            break;
        default:
            main();
            break;
        }
    }
}