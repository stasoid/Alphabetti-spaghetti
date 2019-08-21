// IMPORTANT:
//     all local array variables must be initalized to zero before calling any functions on them
//     example:
//     void func()
//     {
//         IntArray arr = {0};  // correct
//         IntArray arr;        // WRONG
//     }
//
//-------------------------------------------------------------------------------------------------------------------

/*struct Array
{
	int count; // count,N,count,size,len,length
	E*  data;  // items,i,data,d,elems,e,_,$,ptr,p,array,ary,arr,a
};*/


// Idea is from lacc (github.com/larmel/lacc/blob/master/include/lacc/array.h)
// Usage: array_of(int) ints = {0};
//  -or-: typedef array_of(int) IntArray;
#define array_of(type)  \
	struct {            \
		int count;      \
		type* data;     \
	}

// inserts elem at index i
// works as expected for empty array (i must be 0) and for inserting at arr->count (add to the end of array)
#define insert_elem(arr, i, elem) \
	do{  \
		int ii = i; /* precalc i to get rid of side effects (i may depend on count), example: insert_elem(arr, arr.count, elem) */  \
		assert(ii >= 0 && ii <= arr.count), /*note: i may be equal to count*/  \
		arr.data = realloc( arr.data, sizeof(arr.data[0]) * (arr.count + 1) ),  \
		ii < arr.count ? memmove( &arr.data[ii+1], &arr.data[ii], sizeof(arr.data[0]) * (arr.count - ii) ) : __noop,  \
		arr.count++,  \
		arr.data[ii] = elem,  \
		&arr.data[ii];  \
	} while(0)

// adds new element to the end of array
// returns pointer to the new element
// implemented independently from insert_elem to be able to return pointer to the new element
//       (insert_elem can't do this because it has to precalc i)
#define add_elem(arr, ...)  \
	(  \
		arr.data = realloc( arr.data, sizeof(arr.data[0]) * ++arr.count ),  \
		arr.data[arr.count - 1] = __VA_ARGS__,  \
		&arr.data[arr.count - 1]  \
	)

// parens needed around arr to be able to be invoked like this: remove_elem(*toks, i);
#define remove_elem(arr, i)  \
	do{  \
		int ii = i; /* precalc i to get rid of side effects (i may depend on count), example: remove_elem(arr, arr.count - 1) */  \
		assert(ii >= 0 && ii < (arr).count),  \
		memmove( &(arr).data[ii], &(arr).data[ii+1], sizeof((arr).data[0]) * (--(arr).count - ii) );  \
	} while(0)

// get pointer to the last element or 0 if array is empty
#define last_elem(arr)     (arr.count ? &arr.data[arr.count-1] : 0)

#define insert_item insert_elem
#define remove_item remove_elem

#define set_size(arr, n)  \
	do{  \
		arr.data = realloc( arr.data, sizeof(arr.data[0]) * (n) );  \
		arr.count = n;  \
	} while(0)

// adds n new empty(zeroed) elements to array
// returns pointer to the first new element
// n must be > 0
// IMPORTANT: n must not depend on arr->count (because of side effects, see insert_item/remove_item)
#define grow_array(arr, n)  \
	(  \
		assert((n) > 0),  \
		arr.data = realloc( arr.data, sizeof(arr.data[0]) * (arr.count + (n)) ),  \
		memset( &arr.data[arr.count], 0, sizeof(arr.data[0]) * (n) ),  \
		arr.count += (n),  \
		&arr.data[arr.count - (n)]  \
	)

// insert arr2 at index i in array arr
// arr: abcdefg
// arr2: xy
// i: 3
// result: abcxydefg
#define insert_array(arr, i, arr2)  \
	(  \
		assert(sizeof(arr.data[0]) == sizeof(arr2.data[0])),  \
		arr.count += arr2.count,  \
		arr.data  =  realloc(arr.data, sizeof(arr.data[0]) * arr.count),  \
		/*copy arr tail*/ \
		memmove( arr.data + i + arr2.count,  \
			arr.data + i,                                    \
			sizeof(arr.data[0]) * (arr.count - arr2.count - i) )  /*initial arr size minus i*/       \
		/*copy arr2*/ \
		memmove( arr.data + i,  \
			arr2.data,                                    \
			sizeof(arr.data[0]) * arr2.count )         \
	)

// arr += arr2
#define append_array(arr, arr2)  \
	(  \
		assert(sizeof(arr.data[0]) == sizeof(arr2.data[0])),  \
		arr.count += arr2.count,  \
		arr.data  =  realloc(arr.data, sizeof(arr.data[0]) * arr.count),  \
		memmove( arr.data + arr.count - arr2.count,  \
			arr2.data,                                    \
			sizeof(arr.data[0]) * arr2.count )         \
	)

#define clone_array(src, dst)  \
	(  \
		assert(sizeof(src.data[0]) == sizeof(dst.data[0])), \
		assert(!dst.count && !dst.data), \
		dst.count = src.count, \
		dst.data = memdup( src.data, src.count * sizeof(src.data[0]) )  \
	) 

#define delete_array(arr) (free(arr.data), arr.data=0, arr.count=0)

// delete array of pointers
#define delete_ptr_array(arr)  \
	do{  \
		for(int i = 0; i < arr.count; i++) free(arr.data[i]);  \
		delete_array(arr);  \
	} while(0)

// helper for reverse_array
#define swap(pelem1, pelem2, elem_size)       \
	{                                         \
		byte tmp[elem_size];                  \
		memmove( tmp,    pelem1, elem_size );  \
		memmove( pelem1, pelem2, elem_size );  \
		memmove( pelem2, tmp,    elem_size );  \
	}

#define reverse_array(arr)  \
	do{  \
		for(int i = 0; i < arr.count/2; i++)  \
		{  \
		   swap( &arr.data[i], &arr.data[arr.count-1-i], sizeof(arr.data[0]) );  \
	    }  \
	} while(0)

typedef array_of(int) int_array;
typedef array_of(void*) ptr_array;
typedef array_of(char*) str_array;
