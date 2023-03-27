#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <stdio.h>
#include <secure_services.h>

bool rcrOtpRead_nse
(
  uint8_t  xField,
  uint8_t*      xpDest,
  size_t*       xpNumBytes
);
bool rcrOtpRead
(
  uint8_t  xField,
  uint8_t*      xpDest,
  size_t*       xpNumBytes
)
{
  bool retVal = false;
  unsigned int irqKey = 0u;

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

void main(void){
    printk("Hello World! %s\n", CONFIG_BOARD);
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
