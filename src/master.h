#ifndef master_H
#define master_H

#include <pebble.h>

extern char *exercise_name_strings[6];
extern char *exercise_set_strings[6][5];
extern double exercise_multipliers[6][5];
int exercise_int;
double m_weight_d;

// Metric vs Imperial
int unit_system;
char *unit_type;
extern double step_size[2];
extern double plate_weights[6][2];
int bar_type;
extern int barbell_weights[3][2];

#endif
