
#include "ptest.h"

void suite_core(void);
void suite_functional(void);
void suite_data(void);
void suite_exception(void);
void suite_memory(void);
void suite_threading(void);

int main(int argc, char **argv)
{

	pt_add_suite(suite_core);
	pt_add_suite(suite_functional);
	pt_add_suite(suite_data);
	pt_add_suite(suite_exception);
	pt_add_suite(suite_memory);
	pt_add_suite(suite_threading);

	pt_run();

}
