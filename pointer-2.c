// DO NOT INCLUDE ANY OTHER LIBRARIES/FILES
#include "pointer.h"

// In this assignment, you can assume that function parameters are valid and the memory is managed by the caller

// Compares the price of obj1 with obj2
// Returns a negative number if the price of obj1 is less than the price of obj2
// Returns a positive number if the price of obj1 is greater than the price of obj2
// Returns 0 if the price of obj1 is equal to the price of obj2
// I got the price for each object by using the function provided in the header file and made the comparisons.
int compare_by_price(Object* obj1, Object* obj2)
{
    double price1 = object_price(obj1);
    double price2 = object_price(obj2);
    if (price1 != price2) {
	if (price1 < price2) {
	    return -1;
	}
	else if (price1 > price2) {
	    return 1;
	}
    }
    else {
	return 0;
    }
    return 0;
}

// Compares the quantity of obj1 with obj2
// Returns a negative number if the quantity of obj1 is less than the quantity of obj2
// Returns a positive number if the quantity of obj1 is greater than the quantity of obj2
// Returns 0 if the quantity of obj1 is equal to the quantity of obj2
// I got the quantity for each object by using the function provided in the header file and made the comparisons.
int compare_by_quantity(Object* obj1, Object* obj2)
{
    double quan1 = object_quantity(obj1); 
    double quan2 = object_quantity(obj2);
    if (quan1 != quan2) {
        if (quan1 < quan2) {
            return -1;
        }
        else if (quan1 > quan2) {
            return 1;
        }
    }
    else {
        return 0;
    }
    return 0;
}
// Initializes a StaticPriceObject with the given quantity, name, and price
// Memory for the name string is managed by the caller and assumed to be valid for the duration of the object's lifespan
// I checked the structure provided in the header file and assigned each necessary input such as quantity, name, and price accoringly.
// I also had to cast the variables for the static and static bulk price and utilize the function provided in the object structure to assign the variables.
void static_price_object_construct(StaticPriceObject* obj, unsigned int quantity, const char* name, double price)
{
    obj->obj.name = name;
    obj->price = price;
    obj->obj.quantity = quantity;
    obj->obj.virtual_func_table.price = (price_fn)static_price;
    obj->obj.virtual_func_table.bulk_price = (bulk_price_fn)static_bulk_price;    
}
// Initializes a DynamicPriceObject with the given quantity, name, base price, and price scaling factor
// Memory for the name string is managed by the caller and assumed to be valid for the duration of the object's lifespan
// I followed a very similar approach as the static_price_object function except I had to iclude the factor this time.
void dynamic_price_object_construct(DynamicPriceObject* obj, unsigned int quantity, const char* name, double base, double factor)
{
    obj->obj.name = name;
    obj->base = base;
    obj->factor = factor;
    obj->obj.quantity = quantity;
    obj->obj.virtual_func_table.price = (price_fn)dynamic_price;
    obj->obj.virtual_func_table.bulk_price = (bulk_price_fn)dynamic_bulk_price;
}
// Returns the price of a StaticPriceObject or ERR_OUT_OF_STOCK if it is out of stock
// I assigned a variable to hold the quantity and price and simply check if the quantity was zero which returns the error or else it returns the stored price.
double static_price(StaticPriceObject* obj)
{
    unsigned int quantity = obj->obj.quantity;
    double price = obj->price;
    if (quantity == 0) {
	return ERR_OUT_OF_STOCK;
    }
    else {
        return price;
    }
    return 0;
}
// Returns the price of a DynamicPriceObject or ERR_OUT_OF_STOCK if it is out of stock
// The dynamic price is calculated as the base price multiplied by (the quantity raised to the power of the scaling factor)
// I followed a similar approach to the static_price except I had to use the equation provided above to calculate the price by using the power function.
double dynamic_price(DynamicPriceObject* obj)
{
    unsigned int quantity = obj->obj.quantity;
    double base = obj->base;
    double factor = obj->factor;
    double dynamic_price = base*pow(quantity,factor);
    if (quantity == 0) {
	return ERR_OUT_OF_STOCK;
    }
    else {
	return dynamic_price;
    }
}
// Returns the bulk price of purchasing multiple (indicated by quantity parameter) StaticPriceObject at a discount where the first item is regular price and the additional items are scaled by the BULK_DISCOUNT factor
// Return ERR_OUT_OF_STOCK of there is insufficient quantity available
// I began by setting a variable to hold the bulk price by adding the first price to the quantity decremented by one and the discount and the starting price to match the equation above.
// I then check if the quantity requested is larger than what is in stock and return an error if this case is encountered.
// I also had to include if no quantity is requested in which I just return 0 or if the quantity requested is 1 so I just send back the price using a function from the header file.
double static_bulk_price(StaticPriceObject* obj, unsigned int quantity)
{
    double bulk_price = object_price(&obj->obj) + object_price(&obj->obj)*BULK_DISCOUNT*(quantity - 1);
    if (quantity > object_quantity(&obj->obj)) {
        return ERR_OUT_OF_STOCK;
    }
    else if (quantity == 1) {
	return object_price(&obj->obj);
    }
    else {
	if (quantity == 0) {
	    return 0;
	}
	else {
    	    return bulk_price;
	}
    }
}
// Returns the bulk price of purchasing multiple (indicated by quantity parameter) DynamicPriceObject at a discount where the first item is regular price and the additional items are scaled by the BULK_DISCOUNT factor
// This uses the same dynamic price equation from the dynamic_price function, and note that the price changes for each item that is bought
// For example, if 3 items are requested, each of them will have a different price, and this function calculates the total price of all 3 items
// Return ERR_OUT_OF_STOCK of there is insufficient quantity available
// I followed a very similar approach to the static_bulk_price except I had to create a loop for counting subsequent quantity and prices because they would change as quantity decreased.
double dynamic_bulk_price(DynamicPriceObject* obj, unsigned int quantity)
{
    double base = obj->base;
    double factor = obj->factor;
    double dynamic_bulk_price = base*pow(object_quantity(&obj->obj), factor);
    unsigned int count = object_quantity(&obj->obj) - 1;
    if (quantity == 1) {
        return dynamic_bulk_price;
    }
    if (quantity == 0) {
        return 0;
    }
    if (quantity > object_quantity(&obj->obj)) {
	return ERR_OUT_OF_STOCK;
    }
    else{
	while (quantity > 1) {
	    dynamic_bulk_price = dynamic_bulk_price + (obj->base * BULK_DISCOUNT) * pow(count, obj->factor);
	    quantity = quantity - 1;
	    count = count - 1;
	}
    }
    return dynamic_bulk_price;
}
//
// Iterator functions
//

// Initializes an iterator to the beginning of a list
// I initialized both the iterator's current pointer and the prev_next to point to the head to start.
void iterator_begin(LinkedListIterator* iter, LinkedListNode** head)
{
    iter->curr = *head;
    iter->prev_next = head;
}

// Updates an iterator to move to the next element in the list if possible
// I first check if the iterator is on the last node (NULL) and return making it impossible to continue iterating, or I update the pointers for the current node and the pre_next pointers.
void iterator_next(LinkedListIterator* iter)
{
    if (iter->curr == NULL) {
        return;
    }
    iter->prev_next = &(iter->curr->next);
    iter->curr = iter->curr->next;
}

// Returns true if iterator is at the end of the list or false otherwise
// The end of the list is the position after the last node in the list
// I check the end of the list by seeing if the current pointer is at NULL.
bool iterator_at_end(LinkedListIterator* iter)
{
    if (iter->curr == NULL) {
	return true;
    }
    else {
	return false;
    }
}

// Returns the current object that the iterator references or NULL if the iterator is at the end of the list
// I first check if we are at the end of the list and return nothing or I can return the object by going through the iterator's current pointer.
Object* iterator_get_object(LinkedListIterator* iter)
{
    if (iter->curr == NULL) {
        return NULL;
    }
    else {
	return iter->curr->obj;
    }
}

// Removes the current node referenced by the iterator
// The iterator is valid after call and references the next object
// Returns removed node
// In order to remove a node, 3 different values have to be updated.
// I first point the node that is going to be removed to the current node so it can be returned later.
// Then I update the second prev_next pointer (not the head which is preserved) to the arrow pointing to the next node in the list.
// Finally, I can change which node I am on to the next one and simply return the removed node.
LinkedListNode* iterator_remove(LinkedListIterator* iter)
{
    LinkedListNode* removedNode = iter->curr;
    *iter->prev_next = iter->curr->next;
    iter->curr = iter->curr->next;  
    return removedNode;
}

// Inserts node after the current node referenced by the iterator
// The iterator is valid after call and references the same object as before
// Returns ERR_INSERT_AFTER_END error if iterator at the end of the list or 0 otherwise
// I can utilize by function created prior to check if the end of the list is reached and the return the error if it is the case.
// Or else I have to change the inserted node's next pointer to point where the current node's next pointer is and then change the previous node to point to the new one.
// I used a graph to visualize this insertion better.
int iterator_insert_after(LinkedListIterator* iter, LinkedListNode* node)
{
    if (iterator_at_end(iter) == true) {
	return ERR_INSERT_AFTER_END;
    }
    else {
	node->next = iter->curr->next;
        iter->curr->next = node;
        return 0;
    }
}

// Inserts node before the current node referenced by the iterator
// The iterator is valid after call and references the same object as before
// Similar to insert after except I did not have to check if it was the end of the list.
// I also realized I had to update 3 different things to make sure the node was fully integrated.
void iterator_insert_before(LinkedListIterator* iter, LinkedListNode* node)
{
    *iter->prev_next = node;
    node->next = iter->curr;
    iter->prev_next = &node->next;
}

//
// List functions
//

// Returns the maximum, minimum, and average price of the linked list
// I first began by making the values of the max and min to whatever was stored in the first head node.
// I also intialized a counter and total for the average.
// I then created a loop to run until the end of the list from the function I coded prior.
// I then simply update the max and min as needed and then add to the total and counter.
// I am able to move through the list with the last line that iterates through the nodes.
void max_min_avg_price(LinkedListNode** head, double* max, double* min, double* avg)
{
    double count = 0;
    double total = 0;
    LinkedListIterator iter;
    iterator_begin(&iter, head);
    *max = object_price(iterator_get_object(&iter));
    *min = object_price(iterator_get_object(&iter));
    double price = 0;
    while(iterator_at_end(&iter) != true) {
        price = object_price(iterator_get_object(&iter));
	if (price > *(max)) {
	    *max = price;
        }
        if (price < *(min)) {
            *min = price;
        }
        total = total + price;
        count += 1;
	iterator_next(&iter);
    }
    *avg = total/count;
}

// Executes the func function for each node in the list
// The function takes in an input data and returns an output data, which is used as input to the next call to the function
// The initial input data is provided as a parameter to foreach, and foreach returns the final output data
// For example, if there are three nodes, foreach should behave like: return func(node3, func(node2, func(node1, data)))
// I began this function by creating an iterator and checking if it is empty in which case I can just return data.
// Or else I have to recursively call the function on the next node and data which will go through each code and return the required data.
Data foreach(LinkedListNode** head, foreach_fn func, Data data)
{
    LinkedListIterator iter;
    iterator_begin(&iter, head);
    if (iterator_at_end(&iter) == true) {
	return data;
    }
    else {
	return foreach(&iter.curr->next, func, func(iterator_get_object(&iter), data));
}
}

// Returns the length of the list
// I made another iterator and simply put a counter in a loop to check if the end of the list is reached.
// I used the function coded prior to get to each next node in the loop and just return the count afterwards.
int length(LinkedListNode** head)
{
    int count = 0;
    LinkedListIterator iter;
    iterator_begin(&iter, head);
    while (iterator_at_end(&iter) != true) {
	count += 1;
	iterator_next(&iter);
    }
    return count;
}

//
// Mergesort
//

// Assuming list1 and list2 are sorted lists, merge list2 into list1 while keeping it sorted
// That is, when the function returns, list1 will have all the nodes in sorted order and list2 will be empty
// The sort order is determined by the compare function
// Default convention for compare functions on objects A and B:
//   Negative return values indicate A should be earlier than B in the list
//   Positive return values indicate A should be later than B in the list
//   Zero return values indicate A and B are equal
// A stable sort is not required for this implementation, so equal objects can be in either order
// I began by creating 2 different iterators for the different head nodes provided.
// I then stored the value of the compare function and set cases for the three options outlined above.
// A graph helped to represent exactly which list would have to iterate to the next node or have it removed.
// I also had to check the case where the first list reaches the end while the second still has values in which case I just insert everything before and iterate through.
void merge(LinkedListNode** list1_head, LinkedListNode** list2_head, compare_fn compare)
{
    LinkedListIterator iter1;
    LinkedListIterator iter2;
    iterator_begin(&iter1, list1_head);
    iterator_begin(&iter2, list2_head);
    while (iterator_at_end(&iter1) != true && iterator_at_end(&iter2) != true) {
	int value = compare(iterator_get_object(&iter1), iterator_get_object(&iter2));
	if (value < 0) {
	    iterator_next(&iter1);
	}
	if (value == 0) {
	    iterator_insert_after(&iter1, iterator_remove(&iter2));
	    iterator_next(&iter1);
	}
	if (value > 0) {
	    iterator_insert_before(&iter1, iterator_remove(&iter2));
	}
    }
    if (iterator_at_end(&iter1) == true) {
	while (iterator_at_end(&iter2) == false) {
	    iterator_insert_before(&iter1, iterator_remove(&iter2));
	    iterator_next(&iter1);
	}
    }
}

// Split the list head in half and place half in the split list
// For example, if head has 8 nodes, then split will move 4 of them to split_head
// In order to split the list in half, I began by using my length funtion from before and simply dividing it by 2.
// I then created a counter to only iterate through the list that many times and then set the split_head pointer and prev_next pointer accordingly.
void split(LinkedListNode** head, LinkedListNode** split_head)
{
   LinkedListIterator iter;
   iterator_begin(&iter, head);
   int len = length(head);
   int half = len/2;
   int i = 0;
   while(i < half) {
	iterator_next(&iter);
        i += 1;
   }
   *iter.prev_next = NULL;
   *split_head = iter.curr;
}

// Implement the mergesort algorithm to sort the list
// The sort order is determined by the compare function
// Default convention for compare functions on objects A and B:
//   Negative return values indicate A should be earlier than B in the list
//   Positive return values indicate A should be later than B in the list
//   Zero return values indicate A and B are equal
// A stable sort is not required for this implementation, so equal objects can be in either order
// I began by creating a node for the split list so I could utilize the functions I coded prior.
// I check if the list is atleast more than one element and then run my spit function from prior.
// I then recursively call mergesort on the 2 split lists created and finally merge them together afterwards.
void mergesort(LinkedListNode** head, compare_fn compare)
{
    LinkedListNode* split_list;
    if (length(head) > 1) {
	split(head, &split_list);
	mergesort(head, compare);
	mergesort(&split_list, compare);
	merge(head, &split_list, compare);
    }
}
