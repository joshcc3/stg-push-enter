

void* new(int size)
{
    void *ptr = malloc(size);
    return ptr;
}

void delete(void **ptr)
{
    free(*ptr);
    *ptr = NULL;
}