//Viktor Váczi 2014.03.20 for GSOC 2014
#include "planet.h"
#include "datastructures.h"
#include "algorithms.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define G 1
#define DEBUG
#define timestep 600//also defined in algorithms, need to update

int  main()
{
    output_data* out_data;
    dvec3 pos1={1.4960*pow(10,11),0,0},pos2={0,0,0},vel1={0,29.8*1000,0},vel2={0,0,0};//just for debugging
    planet_list* list_of_planets = (planet_list*)calloc(1,sizeof(planet_list));
    add_planet(pos1,vel1 , 5.97219*pow(10,24), "Earth", list_of_planets);
    add_planet(pos2,vel2 , 1.9891*pow(10,30), "SUN", list_of_planets);
    out_data = (output_data*)calloc(1,sizeof(output_data));
    iterate_all(356*24*60*60,timestep,list_of_planets,out_data);
    data_to_csv(out_data);
    return 0;

}
//(planet)(*list_of_planets->first_planet->next_planet->next_planet->planet) for debugging in code blocks.
