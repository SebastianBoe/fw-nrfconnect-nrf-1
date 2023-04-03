#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <stdio.h>
#include <secure_services.h>

#include <debug/ppi_trace.h>

bool rcrOtpRead_nse(uint8_t xField, uint8_t *xpDest, size_t *xpNumBytes);

bool rcrOtpRead(uint8_t xField, uint8_t *xpDest, size_t *xpNumBytes)
{
  bool retVal = false;
  //  unsigned int irqKey = 0u;

  before_nse();
 // irqKey = irq_lock();
  retVal = rcrOtpRead_nse(xField, xpDest, xpNumBytes);
  after_nse();
 // irq_unlock(irqKey);
  return retVal;
}
static void my_timer_handler(struct k_timer *dummy)
{
  printk("=");
}

static K_TIMER_DEFINE(my_timer, my_timer_handler, NULL);

void config(uint32_t pin, uint32_t event) {
	void * handle_trace = ppi_trace_config(pin, event);

	if(handle_trace == NULL) {
		printk("Failed");
	a: goto a;
	}

	ppi_trace_enable(handle_trace);
}

#define TIMER_PIN 0
#define EGU_PIN 1

void trace_enable(void) {
	/* config(TIMER_PIN, (uint32_t)(&NRF_RTC1_NS->EVENTS_COMPARE[0])); */
	/* config(EGU_PIN, (uint32_t)(&NRF_EGU0_NS->EVENTS_TRIGGERED[0])); */
}

void main(void){
    printk("Hello World! %s\n", CONFIG_BOARD);

	trace_enable();

    k_timer_start(&my_timer, K_USEC(100), K_USEC(300));
    for (uint32_t idx = 0u; idx < 0xFFFFFF; idx++)
    {
      uint8_t store[8];
      size_t size;

      size = 4u;
      printk("+");
      if (!rcrOtpRead(1, store, &size))
      {
        break;
      }
      printk("-");
    }
}

int spm_request_random_number_nse(uint8_t *output, size_t len, size_t *olen)
{
	return 0;
}
