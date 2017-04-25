#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "customer_manager.h"

/*----------------------------------------------------------------------
user data structure
----------------------------------------------*/

#define UNIT_ARRAY_SIZE 1024
#define MAX_NUMBER_OF_BUCKETS 1048576
#define GROWTH_FACTOR 2

enum {HASH_MULTIPLIER = 65599};

typedef struct HashTable* HashTable_T;

struct UserInfo {
  //user item data structure that you defined in task 1,

  char *name;                // customer name
  char *id;                  // customer id
  int purchase;              // purchase amount (> 0)

//but add proper fields to keep track of the next node in a hashtable
  struct UserInfo *next_id;
  struct UserInfo *next_name;
  unsigned int h_id;
  unsigned int h_name;
};


struct DB {
    HashTable_T ht_id;  // pointer to the id table
    HashTable_T ht_name; //// pointer to the name table
    int numItems;  // # of stored items, needed to determine
                // # whether the array should be expanded or not
    int curArrSize;
};


struct HashTable {
   struct UserInfo **array;
   int curArrSize;
};


/* Return a hash code for pcKey that is between 0 and iBucketCount-1,
   inclusive. Adapted from the EE209 lecture notes. */
//static int hash_function(const char *pcKey, int iBucketCount)
//{
//   int i;
//   unsigned int uiHash = 0U;
//   for (i = 0; pcKey[i] != '\0'; i++)
//      uiHash = uiHash * (unsigned int)HASH_MULTIPLIER + (unsigned int)pcKey[i];
//   return (int)(uiHash & (unsigned int)(iBucketCount-1) );
//}


static unsigned int uihash(const char *pcKey)
{
   int i;
   unsigned int uiHash = 0U;
   for (i = 0; pcKey[i] != '\0'; i++)
      uiHash = uiHash * (unsigned int)HASH_MULTIPLIER + (unsigned int)pcKey[i];
   return uiHash;
}



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


static int name_search(HashTable_T t, const char *key, int iBuketCount, unsigned int* h)
{
   struct UserInfo* p;
   *h = uihash(key);
   for (p = t->array[(*h)&(iBuketCount-1)]; p != NULL; p = p->next_name)

      if ((p->h_name == *h) && strcmp(p->name, key) == 0) {
         //*value = p->value;
         return 1;
      }
   return 0;
}

static int id_search(HashTable_T t, const char *key, int iBuketCount, unsigned int* h)
{
   struct UserInfo* p;
   *h = uihash(key);
   for (p = t->array[(*h)&(iBuketCount-1)]; p != NULL; p = p->next_id)

      if ((p->h_id == *h) && strcmp(p->id, key) == 0) {
         //*value = p->value;
         return 1;
      }

   return 0;
}


static inline HashTable_T Table_create(int size) {
   HashTable_T t;
   t = (HashTable_T)calloc(1, sizeof(struct HashTable));
   t-> curArrSize = size;
   t->array = (struct UserInfo**)calloc(size,sizeof(struct UserInfo*)); //don't forget to free!!!
   return t;
}


/*--------------------------------------------------------------------*/
DB_T CreateCustomerDB(void)
{
//Allocate DB Initialize ID/NAME hash tables
//(contain array of UserInfo pointers)
  DB_T d;

  d = (DB_T) calloc(1, sizeof(struct DB));

  d->curArrSize = UNIT_ARRAY_SIZE; // start with 1024 elements
  d -> numItems = 0;
  d->ht_id = Table_create(d->curArrSize);
  d->ht_name = Table_create(d->curArrSize);

  if (!(d->ht_id) || !(d->ht_name)) {
    fprintf(stderr, "Can't create a hash table for DB_T\n");
    free(d->ht_id);
    free(d->ht_name);
    free(d);
    return NULL;
  }

//  printf("successfully Create CustomerDB \n");
  return d;
}
/*--------------------------------------------------------------------*/

/* Free all UserInfo members, free UserInfos , Free DB*/
void DestroyCustomerDB(DB_T d){
    if(d==NULL){return;}

    struct UserInfo *p;
    for (int b = 0; b < d->curArrSize; b++){
         for (p = d-> ht_id->array[b]; p != NULL; p = p->next_id) {
            free(p->name);
            free(p->id);
            free(p);
        }
    }
    free(d-> ht_id->array);
    free(d-> ht_name->array);
    free(d-> ht_id);
    free(d-> ht_name);
    free(d);

    return;
}

//d->numItems > 0.75*d->curArrSize
static int DB_grow(DB_T d)  /* Expand HashTable  if full*/
{
    assert(d != NULL); /* Check d points to a valid memory location */

    struct UserInfo *p,*next_id;
    struct UserInfo **pNameArray,**pIdArray;
    int iNewLength;
    iNewLength = d->curArrSize * GROWTH_FACTOR;

    pIdArray = (struct UserInfo **)calloc(iNewLength,sizeof(struct UserInfo*));
    if (pIdArray == NULL)
        return (-1); /* reallocating memory for dynamic array failed */
    pNameArray = (struct UserInfo **)calloc(iNewLength,sizeof(struct UserInfo*));
    if (pNameArray == NULL)
        return (-1); /* reallocating memory for dynamic array failed */

    unsigned int id_index, name_index;
    for (int b = 0; b < d->curArrSize; b++){
         for (p = d-> ht_id->array[b]; p != NULL; p = next_id) {
            next_id = p->next_id;

            id_index = p->h_id&(iNewLength-1);
            p->next_id = pIdArray[id_index];
            pIdArray[id_index] = p;

            name_index = p->h_name&(iNewLength-1);
            p->next_name = pNameArray[name_index];
            pNameArray[name_index] = p;
         }
    }

    d->ht_id->array = pIdArray;
    d->ht_id->curArrSize = iNewLength;
    d->ht_name->array = pNameArray;
    d->ht_name->curArrSize = iNewLength;

    d->curArrSize = iNewLength;

    //printf("grow succeed\n");
    return 1; /* grow SUCCESS */
}


static int DB_add(DB_T d,const char *id, const char *name, int purchase,
                  unsigned int h_id,unsigned int h_name){




   struct UserInfo* p = (struct UserInfo*)malloc(sizeof(struct UserInfo));

   if(!p){return 0;}

   p->id = strdup(id); //remember to free!!
   p->name = strdup(name); //remember to free!!

   if(!(p->id) || !(p-> name)){
        return 0;
   }

   p->purchase = purchase;

   p->h_id = h_id;
   p->h_name = h_name;

   p->next_id = d->ht_id->array[p->h_id&(d->curArrSize-1)];
   d->ht_id->array[p->h_id&(d->curArrSize-1)] = p;

   p->next_name = d->ht_name->array[p->h_name&(d->curArrSize-1)];
   d->ht_name->array[p->h_name&(d->curArrSize-1)] = p;

   return 1;
}


/*--------------------------------------------------------------------*/
//Make use of strdup()  Insert new UserInfo entry to hash tables
int RegisterCustomer(DB_T d, const char *id, const char *name, const int purchase)
{
    if(d == NULL || id == NULL ||  name == NULL || purchase <= 0){
        return (-1);
    }
    //assert(DB_isValid(d)); /* Check d is valid dynamic array */

    unsigned int h_id,h_name;
    //check same id
    if(id_search(d->ht_id,id,d->curArrSize,&h_id))
        return (-1);
    //check same name
    if(name_search(d->ht_name,name,d->curArrSize,&h_name))
        return (-1);

    /* Increase d length.  Expand array  if full*/
    //d->numItems > 0.75*d->curArrSize
    if ( d->numItems >= 0.75*d->curArrSize && MAX_NUMBER_OF_BUCKETS > d->curArrSize*GROWTH_FACTOR)
        if (!DB_grow(d))
            return (-1);


    if(DB_add(d,id,name,purchase,h_id,h_name)==0){
        return (-1);
    }
    d->numItems++;

//
//     printf("Successfully Register Customer:\n");
//     printf("name: %s\n",d->ht_name->array[h_name&(d->curArrSize-1)]->name);
//     printf("id: %s\n",d->ht_name->array[h_name&(d->curArrSize-1)]->id);
//     printf("purchase: %d\n",d->ht_id->array[h_id&(d->curArrSize-1)]->purchase);

    //assert(DB_isValid(d)); /* Check d is valid dynamic array */

    return 0; /* add SUCCESS */

}
///*--------------------------------------------------------------------*/
////Find UserInfo with key == ID  Free id and name Remove from hash tables
int UnregisterCustomerByID(DB_T d, const char *id)
{
    if(d == NULL || id == NULL){
        return (-1);
    }
    //assert(d != NULL);/* Check d points to a valid memory location */
    //assert(DB_isValid(d)); /* Check d is valid dynamic array */


    struct UserInfo* p1,*p2;
    struct UserInfo* p_last_id,*p_last_name;
    unsigned int h_id = uihash(id);
    unsigned int hash_id = (h_id)&(d->curArrSize-1);


    int find_id=false;
    int find_name=false;

    //find the last id, point it to next id
    p1 = d->ht_id->array[hash_id];
    if(!p1){return (-1);}

    if((p1->h_id == h_id) && strcmp(p1->id, id) == 0){ // if the first node is p
            d->ht_id->array[hash_id] = p1->next_id;// change the bucket pointer to p->next
            find_id = true;
    } else{ // if the 1st node is not p,
         p_last_id = p1; // last id is a mark
         for (p1 = p1->next_id; p1 != NULL; p1 = p1->next_id){ //p move to next point
            if((p1->h_id == h_id) && strcmp(p1->id, id) == 0){// if the n-th point is p,
                p_last_id->next_id = p1->next_id; //change the last point's next to p->next
                find_id = true;
                break;
            }
            p_last_id = p1; // last id is a mark
         }
     }
     if(!find_id){
        return (-1);
     }

    //find the last name, point it to next name

    char* name = p1->name;
    unsigned int h_name = uihash(name);
    unsigned int hash_name = (h_name)&(d->curArrSize-1);
    p2 = d->ht_name->array[hash_name];
    if(!p2){return (-1);}

    if((p2->h_name == h_name) && strcmp(p2->name, name) == 0){ // if the first node is p
            d->ht_name->array[hash_name] = p2->next_name;// change the bucket pointer to p->next
            find_name = true;
    } else{ // if the 1st node is not p,
         p_last_name = p2; // last id is a mark
         for (p2 = p2->next_name; p2 != NULL; p2 = p2->next_name){ //p move to next point
            if((p2->h_name == h_name) && strcmp(p2->name, name) == 0){// if the n-th point is p,
                p_last_name->next_name = p2->next_name; //change the last point's next to p->next
                find_name = true;
                break;
            }
            p_last_name = p2; // last id is a mark
         }
     }
     if(!find_name){
        return (-1);
     }


    free(p1->id);
    free(p1->name);
    free(p1);
    d->numItems--;
    return 0;

}


///*--------------------------------------------------------------------*/
////Find UserInfo with key == NAME  Free id and name  Remove from hash tables
//
int
UnregisterCustomerByName(DB_T d, const char *name)
{
    if(d == NULL || name == NULL){
        return (-1);
    }
    //assert(d != NULL);/* Check d points to a valid memory location */
    //assert(DB_isValid(d)); /* Check d is valid dynamic array */


    struct UserInfo* p1,*p2;
    struct UserInfo* p_last_id,*p_last_name;

    int find_id=false;
    int find_name=false;

    //find the last name, point it to next name


    unsigned int h_name = uihash(name);
    unsigned int hash_name = (h_name)&(d->curArrSize-1);
    p2 = d->ht_name->array[hash_name];
    if(!p2){return (-1);}

    if((p2->h_name == h_name) && strcmp(p2->name, name) == 0){ // if the first node is p
            d->ht_name->array[hash_name] = p2->next_name;// change the bucket pointer to p->next
            find_name = true;
    } else{ // if the 1st node is not p,
         p_last_name = p2; // last id is a mark
         for (p2 = p2->next_name; p2 != NULL; p2 = p2->next_name){ //p move to next point
            if((p2->h_name == h_name) && strcmp(p2->name, name) == 0){// if the n-th point is p,
                p_last_name->next_name = p2->next_name; //change the last point's next to p->next
                find_name = true;
                break;
            }
            p_last_name = p2; // last id is a mark
         }
     }
     if(!find_name){
        return (-1);
     }


    char* id = p2->id;
    unsigned int h_id = uihash(id);
    unsigned int hash_id = (h_id)&(d->curArrSize-1);
    //find the last id, point it to next id
    p1 = d->ht_id->array[hash_id];
    if(!p1){return (-1);}

    if((p1->h_id == h_id) && strcmp(p1->id, id) == 0){ // if the first node is p
            d->ht_id->array[hash_id] = p1->next_id;// change the bucket pointer to p->next
            find_id = true;
    } else{ // if the 1st node is not p,
         p_last_id = p1; // last id is a mark
         for (p1 = p1->next_id; p1 != NULL; p1 = p1->next_id){ //p move to next point
            if((p1->h_id == h_id) && strcmp(p1->id, id) == 0){// if the n-th point is p,
                p_last_id->next_id = p1->next_id; //change the last point's next to p->next
                find_id = true;
                break;
            }
            p_last_id = p1; // last id is a mark
         }
     }
     if(!find_id){
        return (-1);
     }


    free(p1->id);
    free(p1->name);
    free(p1);
    d->numItems--;
    return 0;

}



/*--------------------------------------------------------*/
//Find UserInfo with key == ID  Return purchase
int GetPurchaseByID(DB_T d, const char* id)
{

    if(d == NULL || id == NULL){
        return (-1);
    }
  /*Find UserInfo with id == ID  Return purchase */
    struct UserInfo* p;
    unsigned int h = uihash(id);
    for (p = d->ht_id->array[(h)&(d->curArrSize-1)]; p != NULL; p = p->next_id)
        if ((p->h_id == h) && strcmp(p->id, id) == 0) {
            return p->purchase;
        }
    return (-1);
}
/*--------------------------------------------------------------------*/
//Find UserInfo with key == NAME  Return purchase
int GetPurchaseByName(DB_T d, const char* name)
{
    if(d == NULL || name == NULL){
        return (-1);
    }
  /* Find UserInfo with name == NAME  Return purchase */
    struct UserInfo* p;
    unsigned int h = uihash(name);
    for (p = d->ht_name->array[(h)&(d->curArrSize-1)]; p != NULL; p = p->next_name)
        if ((p->h_name == h) && strcmp(p->name, name) == 0) {
            return p->purchase;
        }
    return (-1);
}
/*--------------------------------------------------------------------*/
 /* Iterate through pArray  Sum all purchase values in D.
  scan array from index 0 till the max index,
  and call fp for each _valid_ element.
   */
//Find UserInfo with key == NAME Return purchase

int GetSumCustomerPurchase(DB_T d, FUNCPTR_T fp)
{
    if(d == NULL || fp == NULL){return -1;}

    struct UserInfo *p;
    int sum = 0;
    for (int b = 0; b < d->curArrSize; b++){
         for (p = d-> ht_id->array[b]; p != NULL; p = p->next_id) {
            sum += fp(p->id, p->name, p->purchase);
        }
    }

    return sum;
}

//int main(){
////    int n = 1024;
//    //printf(" and method: hash = %d\n",hash_function("key1",n) );
//    //printf(" mod method: hash = %d\n",hash_function2("key1",n) );
//    DB_T d;
//    d = CreateCustomerDB();
//    unsigned int h;
//
//    int result;
//
//    RegisterCustomer(d,"id1","name1",996);
//    RegisterCustomer(d,"id2","name2",336);
//    result = id_search(d->ht_id,"id1",d->curArrSize,&h);
//    printf("%d\n",result);
//    result = name_search(d->ht_name,"name2",d->curArrSize,&h);
//    printf("%d\n",result);
//
//    UnregisterCustomerByID(d,"id1");
//    result = id_search(d->ht_id,"id1",d->curArrSize,&h);
//    printf("%d\n",result);
//    result = name_search(d->ht_name,"name2",d->curArrSize,&h);
//    printf("%d\n",result);
//
////    UnregisterCustomerByID(d,"id2");
////    result = id_search(d->ht_id,"id1",d->curArrSize,&h);
////    printf("%d\n",result);
////    result = name_search(d->ht_name,"name2",d->curArrSize,&h);
////    printf("%d\n",result);
//
//    UnregisterCustomerByName(d,"name1");
//    result = id_search(d->ht_id,"id1",d->curArrSize,&h);
//    printf("%d\n",result);
//    result = name_search(d->ht_name,"name2",d->curArrSize,&h);
//    printf("%d\n",result);
//
//
//    printf("%d\n",GetPurchaseByID(d,"id1"));
//    printf("%d\n",GetPurchaseByID(d,"id2"));
//    printf("%d\n",GetPurchaseByName(d,"name1"));
//    printf("%d\n",GetPurchaseByName(d,"name2"));
//    DestroyCustomerDB(d);
//
//    return 0;
//}
