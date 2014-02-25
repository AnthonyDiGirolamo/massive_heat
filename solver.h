#ifndef SOLVER
#define SOLVER 1
void set_initial_conditions(Grid grid);
void set_x_boundary_conditions(Grid grid, int x_index, double fixed_value);
void solve_heat_equation(Grid grid);
#endif
