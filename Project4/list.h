#ifndef __LIST__
#define __LIST__


typedef struct node *NodePtr;

typedef struct node
{
  char* name;
  off_t offset;
  NodePtr prev;
  NodePtr next;
} node;


typedef struct Stack_List
{
  NodePtr head;
  NodePtr tail;
  uint size;
} Stack_List;



NodePtr create_node(char* name, off_t offset, NodePtr prev, NodePtr next);
void node_destroy(NodePtr* node);

Stack_List* create_List(void);
Stack_List* copy_List(Stack_List* list);
int is_Empty(Stack_List* list);
uint get_Stack_List_Size(Stack_List* list);
int is_in_Root(Stack_List* list);
int Stack_List_Push(Stack_List* list, char* name, off_t offset);
int Stack_List_Pop(Stack_List* list);
int Stack_List_Peek(Stack_List* list, char** name, off_t* offset);
off_t Stack_List_Peek_offset(Stack_List* list);
int Stack_List_Print(Stack_List* list);
int Stack_List_Print_Path(Stack_List* list);
int Stack_List_Print_Directories(Stack_List* list, int n);
int Stack_List_Empty(Stack_List* list);
int Stack_List_Destroy(Stack_List** list);

#endif
