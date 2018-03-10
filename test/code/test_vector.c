#include <rq.h>

struct x {
    unsigned i;
};

static void 
free_func(void *p)
{
    free(p);
}

int
main(int argc, char **argv)
{
    unsigned count = (argc >= 2 ? atoi(argv[1]) : 1000);
    rq_vector_t vector = rq_vector_alloc(free_func);
    unsigned i;
    int ret = 0;

    if (count == 0)
        count = 1000;

    for (i = 0; i < count; i++)
    {
        struct x *p = (struct x *)malloc(sizeof(struct x));
        p->i = i;
        rq_vector_push_back(vector, p);
    }

    for (i = 0; i < rq_vector_size(vector); i++)
    {
        struct x *p = (struct x *)rq_vector_get_at(vector, i);
        if (i != p->i)
        {
            ret = -1;
            printf("ERROR: value at offset %d != %d\n", i, p->i);
        }
    }

    rq_vector_free(vector);

    return ret;
}
