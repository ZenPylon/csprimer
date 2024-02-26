#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define STARTING_CAPACITY 8

typedef struct DA
{
  size_t max_items; // The max number of items we can store before we have to extend `offsets`
  size_t num_items;
  void **data;
} DA;

DA *DA_new(void)
{
  // To start, naively assume that all entries are of size_t
  DA *new_array = malloc(sizeof(DA));
  new_array->max_items = STARTING_CAPACITY;
  new_array->num_items = 0;
  new_array->data = calloc(STARTING_CAPACITY, sizeof(void *));

  return new_array;
}

int DA_size(DA *da)
{
  return da->num_items;
}

void DA_push(DA *da, void *x)
{
  // Expand the size of `data` if the input x would exceed the DA's current capacity
  if (da->num_items == da->max_items)
  {
    da->max_items *= 2;
    da->data = realloc(da->data, sizeof(void *) * da->max_items);
  }
  da->data[da->num_items] = x;
  da->num_items++;
}

void *DA_pop(DA *da)
{
  if (!da->num_items)
  {
    fprintf(stderr, "Warning: Attempting to pop from an array of size 0.\n");
    return NULL;
  }

  // Don't bother clearing memory in `data` or `offsets`, since the api shouldn't allow for grabbing data beyond num_items
  // (Is this a reasonable stance?)
  da->num_items--;
  return da->data[da->num_items];
}

void DA_set(DA *da, void *x, int i)
{
  if (i >= da->num_items || i < 0)
  {
    fprintf(stderr, "Error: Array index out %d of bounds (size is %zu).\n", i, da->num_items);
    exit(1);
  }

  da->data[i] = x;
}

void *DA_get(DA *da, int i)
{
  if (i >= da->num_items || i < 0)
  {
    fprintf(stderr, "Error: Array index out %d of bounds (size is %zu).\n", i, da->num_items);
    exit(1);
  }

  return da->data[i];
}

void DA_free(DA *da)
{
  for (int i = 0; i < da->num_items; i++)
  {
    free(da->data[i]);
  }
  free(da->data);
  da->data = NULL;
}

int main()
{

  DA *da = DA_new();

  assert(DA_size(da) == 0);

  // basic push and pop test
  int x = 5;
  float y = 12.4;
  DA_push(da, &x);
  DA_push(da, &y);
  assert(DA_size(da) == 2);

  assert(DA_pop(da) == &y);
  assert(DA_size(da) == 1);

  assert(DA_pop(da) == &x);
  assert(DA_size(da) == 0);
  assert(DA_pop(da) == NULL);

  // basic set/get test
  DA_push(da, &x);
  DA_set(da, &y, 0);
  assert(DA_get(da, 0) == &y);
  DA_pop(da);
  assert(DA_size(da) == 0);

  // expansion test
  DA *da2 = DA_new(); // use another DA to show it doesn't get overriden
  DA_push(da2, &x);
  int i, n = 100 * STARTING_CAPACITY, arr[n];
  for (i = 0; i < n; i++)
  {
    arr[i] = i;
    DA_push(da, &arr[i]);
  }
  assert(DA_size(da) == n);
  for (i = 0; i < n; i++)
  {
    assert(DA_get(da, i) == &arr[i]);
  }
  for (; n; n--)
    DA_pop(da);
  assert(DA_size(da) == 0);
  assert(DA_pop(da2) == &x); // this will fail if da doesn't expand

  DA_free(da);
  DA_free(da2);
  printf("OK\n");
}
