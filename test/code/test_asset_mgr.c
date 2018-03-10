#include <rq.h>
#include <stdlib.h>

const char *ccys[] = {
    "AUD",
    "GBP",
    "USD",
    "JPY"
};

const char *equities[] = {
    "ANZ",
    "BHP",
    "WBC",
    "ITE"
};

int
main()
{
    rq_asset_mgr_t asset_mgr = rq_asset_mgr_alloc();
    int i;
    int failed = 0;

    for (i = 0; i < sizeof(ccys)/sizeof(const char *); i++)
    {
        rq_asset_t asset = rq_asset_ccy_build(ccys[i], ccys[i], 365);
        rq_asset_mgr_add(asset_mgr, asset);
    }

    for (i = 0; i < sizeof(equities)/sizeof(const char *); i++)
    {
        rq_asset_t asset = rq_asset_equity_build(equities[i], equities[i], equities[i], "AUD");
        rq_asset_mgr_add(asset_mgr, asset);
    }

    for (i = 0; i < sizeof(ccys)/sizeof(const char *); i++)
    {
        rq_asset_t asset = rq_asset_ccy_find(
            asset_mgr,
            ccys[i]
            );

        printf("Searching for %s... ", ccys[i]);
        if (!RQ_IS_NULL(asset))
            printf("Found\n");
        else
        {
            printf("Not found!\n");
            failed = 1;
        }
    }

    for (i = 0; i < sizeof(equities)/sizeof(const char *); i++)
    {
        rq_asset_t asset = rq_asset_equity_find(
            asset_mgr,
            equities[i]
            );

        printf("Searching for %s... ", equities[i]);
        if (!RQ_IS_NULL(asset))
            printf("Found\n");
        else
        {
            printf("Not found!\n");
            failed = 1;
        }
    }

    return failed;
}
