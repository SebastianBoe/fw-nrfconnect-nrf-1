#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <stdio.h>
#include <secure_services.h>

void rcrOtpRead_nse(size_t num);

void rcrOtpRead(size_t num)
{
  // unsigned int irqKey = 0u;

  before_nse();
 // irqKey = irq_lock();
  rcrOtpRead_nse(num);
  after_nse();
 // irq_unlock(irqKey);
}
static void my_timer_handler(struct k_timer *dummy)
{
}

static K_TIMER_DEFINE(my_timer, my_timer_handler, NULL);

void main(void){
    printk("Hello World! %s\n", CONFIG_BOARD);

    k_timer_start(&my_timer, K_USEC(100), K_USEC(300));
    rcrOtpRead(0xFFFFFFFF);
}

int spm_request_random_number_nse(uint8_t *output, size_t len, size_t *olen)
{
	return 0;
}
