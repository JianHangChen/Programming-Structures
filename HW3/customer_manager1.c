#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "customer_manager.h"

/*----------------------------------------------------------------------
user datastructure
----------------------------------------------*/

#define UNIT_ARRAY_SIZE 1024

struct UserInfo {
  char *name;                // customer name
  char *id;                  // customer id
  int purchase;              // purchase amount (> 0)
};

struct DB {
  struct UserInfo *pArray;   // pointer to the array
  int curArrSize;            // current array size (max # of elements)
  int numItems;              // # of stored items, needed to determine
                            // # whether the array should be expanded or not

};

/*
static int DB_isValid(DB_T d)
{
    // Length cannot be negative
    if (d->curArrSize < 0)
        return 0;

    // items cannot be larger than Array length
    if (d->numItems > d->curArrSize)
        return 0;
    // pArray should point to valid memory location
    if (d->pArray == NULL)
        return 0;

    return 1; // Validation SUCCESS
}
*/


static int DynArray_grow(DB_T d)  /* Expand array  if full*/
{

    struct UserInfo *pNewArray;

    assert(d != NULL); /* Check d points to a valid memory location */


    pNewArray = (struct UserInfo *)realloc(d->pArray,
            (d-> curArrSize + UNIT_ARRAY_SIZE) * sizeof(struct UserInfo));



    if (pNewArray == NULL)
        return (-1); /* reallocating memory for dynamic array failed */

    for(int i = d->curArrSize; i < d-> curArrSize + UNIT_ARRAY_SIZE ;i++){
        pNewArray[i].name = NULL;
        pNewArray[i].id = NULL;
    }

    d->curArrSize = d-> curArrSize + UNIT_ARRAY_SIZE;
    d->pArray = pNewArray;
    //printf("grow succeed\n");
    return 1; /* grow SUCCESS */
}

/* return the name in the pArray*/
static int name_search(DB_T d, const char *name, int *position){

   for (int i = 0; i < d->curArrSize; i++){
      if((d->pArray[i]).name == NULL)
        ;
      else if (strcmp( (d->pArray[i]).name, name) == 0){
            *position = i;
            return 1;
      }
   }
   return 0;
}

/* return the id in the pArray*/
static int id_search(DB_T d, const char *id, int *position){
   for (int i = 0; i < d->curArrSize; i++){
        if((d->pArray[i]).name == NULL)
            ;
        else if (strcmp( (d->pArray[i]).id, id) == 0){
            *position = i;
            return 1;
        }
   }
   return 0;
}


/*--------------------------------------------------------------------*/
DB_T CreateCustomerDB(void)
{

  DB_T d;

  d = (DB_T) calloc(1, sizeof(struct DB));
  if (d == NULL) {
    fprintf(stderr, "Can't allocate a memory for DB_T\n");
    return NULL;
  }
  d->curArrSize = UNIT_ARRAY_SIZE; // start with 1024 elements
  d->pArray = (struct UserInfo *)calloc(d->curArrSize,
               sizeof(struct UserInfo));
  if (d->pArray == NULL) {
    fprintf(stderr, "Can't allocate a memory for array of size %d\n",
	    d->curArrSize);
    free(d);
    return NULL;
  }

  // printf("successfully Create CustomerDB \n");
  return d;

  return NULL;

}
/*--------------------------------------------------------------------*/
void
DestroyCustomerDB(DB_T d)
{
  if(d==NULL){return;}
    //assert(d != NULL);/* Check d points to a valid memory location */
  //assert(DB_isValid(d)); /* Check d is valid dynamic array */

  /* Free all UserInfo "id" and "name" , Free DB*/
   for(int i = 0; i < d->curArrSize;i++){
        if (d->pArray[i].name != NULL || d->pArray[i].id != NULL){

            // printf("Successfully free:\n");
            // printf("name: %s\n",d->pArray[i].name);
            // printf("id: %s\n",d->pArray[i].id);
            // printf("purchase: %d\n",d->pArray[i].purchase);

            free(d->pArray[i].name);
            free(d->pArray[i].id);

            (d->numItems)--;
        }
   }

  free(d->pArray);
  free(d);

  return;
}
/*--------------------------------------------------------------------*/
int
RegisterCustomer(DB_T d, const char *id,
		 const char *name, const int purchase)
{
    if(d == NULL || id == NULL ||  name == NULL || purchase <= 0){
        return (-1);
    }
    //assert(DB_isValid(d)); /* Check d is valid dynamic array */

    int pos;
    //check same id
    if(id_search(d,id,&pos))
        return (-1);
    //check same name
    if(name_search(d,name,&pos))
        return (-1);

    /* Increase d length.  Expand array  if full*/
    if (d->curArrSize == d->numItems)
        if (!DynArray_grow(d))
            return (-1);

    for(int i = 0; i < d->curArrSize;i++){
        if (d->pArray[i].name == NULL && d->pArray[i].id == NULL){
            d->pArray[i].name = strdup(name);
            d->pArray[i].id = strdup(id);
            d->pArray[i].purchase = purchase;
            (d->numItems)++;
            // printf("Successfully Register Customer:\n");
            // printf("name: %s\n",d->pArray[i].name);
            // printf("id: %s\n",d->pArray[i].id);
            // printf("purchase: %d\n",d->pArray[i].purchase);
            break;
        }

    }
    //assert(DB_isValid(d)); /* Check d is valid dynamic array */

    return 0; /* add SUCCESS */

}
/*--------------------------------------------------------------------*/
int
UnregisterCustomerByID(DB_T d, const char *id)
{
    if(d == NULL || id == NULL){
        return (-1);
    }
    //assert(d != NULL);/* Check d points to a valid memory location */
    //assert(DB_isValid(d)); /* Check d is valid dynamic array */

  /* Find UserInfo with id == ID  Free id and name  Point to NULL */
    int position;
    if(id_search(d, id, &position)){
        free(d->pArray[position].name);
        free(d->pArray[position].id);
        d->pArray[position].name = NULL;
        d->pArray[position].id = NULL;
        (d->numItems)--;
        return 0;
    }

    return (-1);
}

/*--------------------------------------------------------------------*/
int
UnregisterCustomerByName(DB_T d, const char *name)
{
    if(d == NULL || name == NULL){
        return (-1);
    }
    //assert(d != NULL);/* Check d points to a valid memory location */
    //assert(DB_isValid(d)); /* Check d is valid dynamic array */

    /* Find UserInfo with name == NAME  Free id and name Point to NULL  */

    int position;
    if(name_search(d, name, &position)){
        free(d->pArray[position].name);
        free(d->pArray[position].id);
        d->pArray[position].name = NULL;
        d->pArray[position].id = NULL;
        (d->numItems)--;
        return 0;
    }
    return (-1);
}
/*--------------------------------------------------------*/
int
GetPurchaseByID(DB_T d, const char* id)
{
    if(d == NULL || id == NULL){
        return (-1);
    }
  /*Find UserInfo with id == ID  Return purchase */
    int position;
    if(id_search(d, id, &position)){
        return d->pArray[position].purchase;
    }

    return (-1);
}
/*--------------------------------------------------------------------*/
int
GetPurchaseByName(DB_T d, const char* name)
{
    if(d == NULL || name == NULL){
        return (-1);
    }
  /* Find UserInfo with name == NAME  Return purchase */
    int position;
    if(name_search(d, name, &position)){
        return d->pArray[position].purchase;
    }

    return (-1);
}
/*--------------------------------------------------------------------*/
 /* Iterate through pArray  Sum all purchase values in D.
  scan array from index 0 till the max index,
  and call fp for each _valid_ element.
   */

int GetSumCustomerPurchase(DB_T d, FUNCPTR_T fp)
{
    if(d == NULL || fp == NULL){return -1;}

    int sum = 0;

    for(int i = 0; i < d->curArrSize;i++){
        if (d->pArray[i].name != NULL && d->pArray[i].id != NULL){

            sum += fp(d->pArray[i].id, d->pArray[i].name,d->pArray[i].purchase);
            //sum += d->pArray[i].purchase;
            // printf("Successfully add purchase:\n");
            // printf("name: %s\n",d->pArray[i].name);
            // printf("id: %s\n",d->pArray[i].id);
            // printf("purchase: %d\n",d->pArray[i].purchase);

        }
    }

    return sum;
}

/*
int main(){
    DB_T d;
    d = CreateCustomerDB();
//    for(int i=0;i<1025;i++){
//        RegisterCustomer(d, "DDR","XiaoJiejie", 13);
//    }

    RegisterCustomer(d, "id1","name1", 12);
    RegisterCustomer(d, "id2","name2", 19);

//test unregister
    //UnregisterCustomerByID(d, "id1");
    //UnregisterCustomerByName(d, "name1");


//test for name/id search
    int pos1;
    int pos2;
    int ifName;
    ifName = name_search(d,"name1",&pos1);
    printf("%d\n",ifName);

    ifName = id_search(d,"id1",&pos2);
    printf("%d\n",ifName);

    ifName = name_search(d,"name2",&pos1);
    printf("%d\n",ifName);

    ifName = id_search(d,"id2",&pos2);
    printf("%d\n",ifName);

    printf("purchase: %d\n",GetPurchaseByID(d,"id1"));
    printf("purchase: %d\n",GetPurchaseByName(d,"name1"));

    printf("purchase: %d\n",GetPurchaseByID(d,"id2"));
    printf("purchase: %d\n",GetPurchaseByName(d,"name2"));

    GetSumCustomerPurchase(DB_T d, FUNCPTR_T fp)

    //DestroyCustomerDB(d);
    return 0;
}*/
