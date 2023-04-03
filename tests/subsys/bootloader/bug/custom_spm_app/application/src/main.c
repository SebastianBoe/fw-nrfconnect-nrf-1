#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <stdio.h>
#include <secure_services.h>

#include <hal/nrf_rtc.h>
#include <hal/nrf_egu.h>


#include <debug/ppi_trace.h>


#define TIMER_PIN 10
#define EGU0_PIN 11
#define EGU1_PIN 12


bool rcrOtpRead_nse(uint8_t xField, uint8_t *xpDest, size_t *xpNumBytes);

bool rcrOtpRead(uint8_t xField, uint8_t *xpDest, size_t *xpNumBytes)
{
  bool retVal = false;
  //  unsigned int irqKey = 0u;

  before_nse();
 // irqKey = irq_lock();
  //  NRF_P0->OUTSET |= 1 << EGU1_PIN;
  retVal = rcrOtpRead_nse(xField, xpDest, xpNumBytes);
  //  NRF_P0->OUTCLR |= 1 << EGU1_PIN;
  after_nse();
 // irq_unlock(irqKey);
  return retVal;
}
static void my_timer_handler(struct k_timer *dummy)
{
	//	NRF_P0->OUTSET |= 1 << EGU0_PIN;
	printk("=");
	//	NRF_P0->OUTCLR |= 1 << EGU0_PIN;
}

static K_TIMER_DEFINE(my_timer, my_timer_handler, NULL);

static void conf(uint32_t pin, uint32_t evt)
{
	void *handle;

	handle = ppi_trace_config(pin, evt);
	__ASSERT(handle != NULL,
		"Failed to initialize trace pin, no PPI or GPIOTE resources?");

	ppi_trace_enable(handle);
}

void trace_enable(void) {
	conf(TIMER_PIN,
		 nrf_rtc_event_address_get(NRF_RTC1_NS, NRF_RTC_EVENT_COMPARE_0));

	NRF_P0->DIRSET |= 1 << EGU0_PIN;
	NRF_P0->DIRSET |= 1 << EGU1_PIN;
	NRF_P0->DIRSET |= 1 << 13;

	NRF_P0->OUTSET |= 1 << EGU0_PIN;
	for(volatile int i = 0; i < 1000; i++);
	NRF_P0->OUTCLR |= 1 << EGU0_PIN;

	NRF_P0->OUTSET |= 1 << EGU1_PIN;
	for(volatile int i = 0; i < 1000; i++);
	NRF_P0->OUTCLR |= 1 << EGU1_PIN;

		/* __NOP(); __NOP(); __NOP(); __NOP(); */
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
