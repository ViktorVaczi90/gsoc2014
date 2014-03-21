#ifndef PLANET
#define PLANET
#include "datastructures.h"

/*This structure defines a planet with its position, velocity and acceleration.
I've added the acceleration to the properties for easier numerical integration*/
typedef struct planet{
char* name;
struct dvec3 pos;
struct dvec3 vel;
struct dvec3 acc;
double mass;
}planet;

/*This struct defines an element in the linked list of planets. I added the pointer of the previous element */
typedef struct planet_list_item{
    struct planet_list_item* next_planet;
    planet* planet;
}list_planet_item;

/*This struct defines the linked list of planets*/
typedef struct planet_list{
struct planet_list_item* first_planet;
int number_of_planets;
}planet_list;

int add_planet(dvec3 pos_in,dvec3 vel_in, double mass_in, char* name_in, struct planet_list * list_of_planets);
void print_planet_data(planet* planet_input);
#endif
