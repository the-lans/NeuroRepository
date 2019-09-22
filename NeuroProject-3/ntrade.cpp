
#include "ntrade.h"


void to_value(NPriceDelta& value, string& str_val)
{
    str_to_lower(str_val);
         if(str_val == "prev") {value = NPriceDelta::PricePrev;}
    else if(str_val == "close") {value = NPriceDelta::PriceClose;}
    else {value = NPriceDelta::PricePrev;}
}

string to_string(NPriceDelta& value)
{
         if(value == NPriceDelta::PricePrev) {return "Prev";}
    else if(value == NPriceDelta::PriceClose) {return "Close";}
    else {return "";}
}
