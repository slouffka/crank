#include "DataTables.hpp"
#include "Ship.hpp"

#include <vector>

std::vector<ShipData> initializeShipData()
{
    std::vector<ShipData> data(Ship::TypeCount);

    data[Ship::Eagle].hitpoints = 100;
    data[Ship::Eagle].speed = 450.f;
    data[Ship::Eagle].texture = Textures::Eagle;

    data[Ship::Raptor].hitpoints = 150;
    data[Ship::Raptor].speed = 300.f;
    data[Ship::Raptor].texture = Textures::Raptor;

    return data;
}
