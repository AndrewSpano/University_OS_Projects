#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"


NodePtr create_node(char* name, off_t offset, NodePtr prev, NodePtr next)
{
  NodePtr new_node = malloc(sizeof(node));
  if (new_node == NULL)
  {
    return NULL;
  }

  new_node->name = name;
  new_node->offset = offset;
  new_node->prev = prev;
  new_node->next = next;

  return new_node;
}


void node_destroy(NodePtr* node)
{
  free((*node)->name);
  free(*node);
  node = NULL;
}



Stack_List* create_List(void)
{
  Stack_List* new_List = malloc(sizeof(Stack_List));
  if (new_List == NULL)
  {
    perror("malloc");
    return NULL;
  }
  new_List->head = NULL;
  new_List->tail = NULL;
  new_List->size = 0;

  return new_List;
}



Stack_List* copy_List(Stack_List* list)
{
  if (is_Empty(list))
  {
    return NULL;
  }

  /* create the new list */
  Stack_List* copy_list = create_List();
  if (copy_list == NULL)
  {
    return NULL;
  }


  char* new_name = malloc((strlen(list->head->name) + 1) * sizeof(char));
  if (new_name == NULL)
  {
    perror("malloc() error");
    free(copy_list);
    return NULL;
  }
  strcpy(new_name, list->head->name);


  /* copy the first node */
  copy_list->head = create_node(new_name, list->head->offset, NULL, NULL);
  if (copy_list->head == NULL)
  {
    perror("malloc() error in new_node()");
    free(new_name);
    free(copy_list);
    return NULL;
  }
  copy_list->tail = copy_list->head;

  int len = list->size;
  /* if the list has more than 1 nodes, copy the rest of them */
  if (len > 1)
  {
    NodePtr temp = list->head->next;
    NodePtr temp_copy = copy_list->head;
    int i = 1;
    /* iterate to copy the nodes */
    for (; i < len; i++)
    {
      char* temp_name = malloc((strlen(temp->name) + 1) * sizeof(char));
      if (temp_name == NULL)
      {
        perror("malloc() error");
        free(copy_list);
        return NULL;
      }
      strcpy(temp_name, temp->name);

      temp_copy->next = create_node(temp_name, temp->offset, temp_copy, NULL);
      if (temp_copy->next == NULL)
      {
        perror("malloc() error in new_node()");
        free(temp_name);
        free(copy_list);
        return NULL;
      }

      temp = temp->next;
      temp_copy = temp_copy->next;
    }

    /* set the tail */
    copy_list->tail = temp_copy;
  }



  copy_list->size = len;
  return copy_list;
}



int is_Empty(Stack_List* list)
{
  return list->size == 0;
}



uint get_Stack_List_Size(Stack_List* list)
{
  return list->size;
}



int is_in_Root(Stack_List* list)
{
  return list->size == 1;
}



int Stack_List_Push(Stack_List* list, char* name, off_t offset)
{
  if (is_Empty(list))
  {
    NodePtr new_node = create_node(name, offset, NULL, NULL);
    if (new_node == NULL)
    {
      return 0;
    }
    list->head = new_node;
    list->tail = new_node;
  }
  else
  {
    NodePtr new_node = create_node(name, offset, list->tail, NULL);
    if (new_node == NULL)
    {
      return 0;
    }
    list->tail->next = new_node;
    list->tail = new_node;
  }

  list->size++;

  return 1;
}



int Stack_List_Pop(Stack_List* list)
{
  if (is_Empty(list))
  {
    return 0;
  }

  if (list->size == 1)
  {
    node_destroy(&(list->tail));
    list->head = NULL;
    list->tail = NULL;
  }
  else
  {
    NodePtr temp = list->tail->prev;
    node_destroy(&(list->tail));
    list->tail = temp;
    temp->next = NULL;
  }


  list->size--;

  return 1;
}



int Stack_List_Peek(Stack_List* list, char** name, off_t* offset)
{
  if (is_Empty(list))
  {
    return 0;
  }

  strcpy(*name, list->tail->name);
  *offset = list->tail->offset;

  return 1;
}



off_t Stack_List_Peek_offset(Stack_List* list)
{
  if (is_Empty(list))
  {
    return (off_t) 0;
  }

  return list->tail->offset;
}



int Stack_List_Print(Stack_List* list)
{
  if (is_Empty(list))
  {
    return 0;
  }

  NodePtr temp = list->head;
  while (temp != NULL)
  {
    printf("Directory:   %s   offset: %lu\n", temp->name, temp->offset);
    temp = temp->next;
  }

  return 1;
}



int Stack_List_Print_Path(Stack_List* list)
{
  if (is_Empty(list))
  {
    return 0;
  }

  NodePtr temp = list->head;
  while (temp != NULL)
  {
    printf("/%s", temp->name);
    temp = temp->next;
  }
  printf("\n");

  return 1;
}



int Stack_List_Print_Directories(Stack_List* list, int n)
{
  if (list == NULL || n <= 0)
  {
    return 0;
  }

  uint number_of_prints = n;
  if (list->size < number_of_prints)
  {
    number_of_prints = list->size;
  }

  NodePtr temp = list->tail;
  int i = 1;
  for (; i < number_of_prints; i++)
  {
    temp = temp->prev;
  }


  // [0;34m	Blue
  // [1;34m	Bold Blue

  /* set print colour to Bold Blue */
  printf("\033[1;34m");

  if (list->size > number_of_prints)
  {
    printf("~");
  }

  while (number_of_prints > 0)
  {
    printf("/%s", temp->name);
    temp = temp->next;
    number_of_prints--;
  }

  /* set print colour back to normal */
  printf("\033[0m");

  return 1;
}



int Stack_List_Empty(Stack_List* list)
{
  if (list == NULL || is_Empty(list))
  {
    return 0;
  }

  while (Stack_List_Pop(list));

  return 1;
}


int Stack_List_Destroy(Stack_List** list)
{
  if (*list == NULL)
  {
    return 0;
  }


  while (Stack_List_Pop(*list));



  (*list)->head = NULL;
  (*list)->tail = NULL;
  (*list)->size = 0;

  free(*list);
  *list = NULL;

  return 1;
}
