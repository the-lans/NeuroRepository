#ifndef NTRADE_H
#define NTRADE_H

#include <string>
#include <vector>
#include "funcecsv.h"

using namespace std;

enum class NPriceType {PriceHigh, PriceLow, PriceClose};
enum class NPriceDelta {PricePrev, PriceClose};

void to_value(NPriceDelta& value, string& str_val);
string to_string(NPriceDelta& value);

#endif // NTRADE_H
