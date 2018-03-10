#include <rq.h>
#include <stdlib.h>

struct a {
    const char *asset_type_id;
    const char *asset_id;
    double price;
    double expected_price;
} alist[] = {
    { "Equity", "ANZ", 30.0, 40.0 },
    { "Equity", "BHP", 20.0, 20.0 },
    { "Equity", "ITE", 50.0, 50.0 },
    { "Equity", "IBM", 35.0, 35.0 },
    { "Equity", "AMC", 10.0, 10.0 },
    { "Equity", "ANZ", 40.0, 40.0 },
    { "Equity", "ANZ", 40.0, 40.0 },
    { "PreciousMetal", "XPT", 500.0, 500.0 },
    { "PreciousMetal", "XAU", 400.0, 400.0 },
    { "PreciousMetal", "XAG", 90.0, 90.0 }
};

int
test_spot_price_mgr(rq_spot_price_mgr_t spot_price_mgr)
{
    int i;
    int failed = 0;

    for (i = 0; i < sizeof(alist) / sizeof(struct a); i++)
    {
        printf("inserting %s %s %f\n", alist[i].asset_type_id, alist[i].asset_id, alist[i].price);
        rq_spot_price_mgr_add(spot_price_mgr, alist[i].asset_type_id, alist[i].asset_id, alist[i].price);
    }

    for (i = 0; i < sizeof(alist) / sizeof(struct a); i++)
    {
        double price = rq_spot_price_mgr_get_price(
            spot_price_mgr, 
            alist[i].asset_type_id, 
            alist[i].asset_id
            );

        printf("checking %s %s %f == %f... ", alist[i].asset_type_id, alist[i].asset_id, price, alist[i].expected_price);

        if (price != alist[i].expected_price)
        {
            failed = 1;
            printf("FAILED!\n");
        }
        else
            printf("OK\n");
    }

    return failed;
}

int
main(int argc, char **argv)
{
    rq_spot_price_mgr_t spot_price_mgr = rq_spot_price_mgr_alloc();
    int failed = test_spot_price_mgr(spot_price_mgr);

    if (!failed)
    {
        rq_spot_price_mgr_t spot_price_mgr_clone;
        printf("\nCloning spot price manager... ");
        spot_price_mgr_clone = rq_spot_price_mgr_clone(spot_price_mgr);
        printf("OK\n");
        printf("Repeating tests for spot price manager clone...\n");
        failed = test_spot_price_mgr(spot_price_mgr_clone);
        rq_spot_price_mgr_free(spot_price_mgr_clone);
    }

    rq_spot_price_mgr_free(spot_price_mgr);

    return failed;
}

