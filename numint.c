//Viktor Váczi 2014.03.20 for GSOC 2014
//Update 2014.03.24
#include "planet.h"
#include "datastructures.h"
#include "algorithms.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define G 1
#define DEBUG
#define timestep 600//also defined in algorithms, need to update

int  main()
{
    output_data* out_data;
    dvec3 pos_earth={1.4960*pow(10,11),0,0},pos_sun={0,0,0},pos_moon={384400*1000+1.4960*pow(10,11),0,0},
    vel_earth={0,29.8*1000,0},vel_sun={0,0,0},vel_moon={0 ,1.023 *1000+29.8*1000,0};//just for debugging
    dvec3 pos_jupiter ={5.096210785220794*pow(10,8)*pow(10,3),-5.625673003639891*pow(10,8)*pow(10,3), -9.109215289602894*pow(10,6)*pow(10,3)}
    ,vel_jupiter={9.531092644846060*1000,9.401837023552590*1000,-2.522073348918472*100};
    dvec3 pos_CG={ -5.314297832314674*pow(10,8)*1000, -5.852838070505260*pow(10,8)*1000,2.828844868846621*pow(10,7)*1000};
    dvec3 vel_CG={5.073676334789207*1000,-9.361113278695045*1000,-4.153703329969050*1000};
    planet_list* list_of_planets = (planet_list*)calloc(1,sizeof(planet_list));
    add_planet(pos_sun,vel_sun , 1.9891*pow(10,30), "SUN", list_of_planets);
    add_planet(pos_CG,vel_CG,3.14*pow(10,12),"Churyumov-Gerasimenko",list_of_planets);
    add_planet(pos_jupiter,vel_jupiter,1.89813*pow(10,27),"Jupiter",list_of_planets);
    out_data = (output_data*)calloc(1,sizeof(output_data));
    iterate_all(7305*24*60*60,timestep,list_of_planets,out_data);//7305 = 20 years
    data_to_csv(out_data);
    free_output_data(out_data);
    free(out_data);
    free_planet_list(list_of_planets);
    free(list_of_planets);

    return 0;

}
