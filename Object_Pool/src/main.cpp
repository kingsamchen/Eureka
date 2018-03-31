/*
 @ 0xCCCCCCCC
*/

#include <iostream>

#include "stock_pool.h"

int main()
{
    std::shared_ptr<StockFactory> sf(new StockFactory);

    {
        auto ms_stock = sf->GetStock("MS");
        auto gg_stock = sf->GetStock("Google");
        auto ms2_sotkc = sf->GetStock("Amazon");
    }

    auto ms_stock = sf->GetStock("MS");

    sf->Prune();

    return 0;
}
