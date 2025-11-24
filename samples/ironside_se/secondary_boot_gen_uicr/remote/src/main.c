
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <nrf_ironside/bootmode.h>
#include <string.h>

int main(void)
{
	int err;

	printk("=== Hello World from Radiocore Image ===\n");

	return 0;
}
