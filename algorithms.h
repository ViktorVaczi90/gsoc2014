#ifndef ALGOR
#define ALGOR
#include "datastructures.h"
#include "planet.h"

void calculate_gravity_acc(planet* planet_target ,planet* planet_acting);
void iterate_euler(planet* planet_target);
void iterate_second_order_RK(planet* planet_target);
void one_iteration(planet_list* list_of_planets);
void iterate_all(double time, double time_step,planet_list* list_of_planets,output_data* outdata);
#endif

