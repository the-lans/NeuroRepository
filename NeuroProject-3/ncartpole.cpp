#include "ncartpole.h"

NCartPole::NCartPole()
{

}

NType NCartPole::getAngle() const
{
    return angle;
}

NType NCartPole::getX_dot() const
{
    return x_dot;
}

NType NCartPole::getAngle_dot() const
{
    return angle_dot;
}

NType NCartPole::getX() const
{
    return x;
}
