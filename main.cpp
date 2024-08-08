#include <boost/hana.hpp>
#include <boost/tti/has_member_function.hpp>
#include <iostream>
#include <type_traits>

class Pizza
{
public:
    virtual void showIngredients() = 0;
};

class Margherita : public Pizza
{
public:
    void showIngredients() override
    {
        std::cout << "Dough\nTomato sauce\nMozzarella\nBasil" << std::endl;
    }
};

// Replace with concepts when C++20 comes
//*****************************************************************************
// Generate the metafunction
BOOST_TTI_HAS_MEMBER_FUNCTION(showTopping)

// Check whether T has a member function showTopping
// which takes no parameter and void return
template<typename T>
constexpr bool has_showTopping =
    has_member_function_showTopping<T, void>::value;
//*****************************************************************************

struct BaconTopping
{
    void showTopping()
    {
        std::cout << "Bacon" << std::endl;
    }
};

struct OnionTopping
{
    void showTopping()
    {
        std::cout << "Onion" << std::endl;
    }
};

template<typename BazePizza, typename... Toppings>
class PizzaWithTopping : public BazePizza
{
    static_assert((... && has_showTopping<Toppings>),
                  "template arguments Pizza with toppings must have a "
                  "showTopping method");

public:
    void showIngredients() override
    {
        (Toppings{}.showTopping(), ...);
        BazePizza::showIngredients();
    }
};

int main()
{
    PizzaWithTopping<Margherita, BaconTopping, OnionTopping> disgusting_pizza;
    disgusting_pizza.showIngredients();
    std::cout << "=============" << std::endl;
    Margherita& pm = disgusting_pizza;
    pm.showIngredients();

    // static_assert error
    // PizzaWithTopping<Margherita, BaconTopping, double> e; // ERROR
}
