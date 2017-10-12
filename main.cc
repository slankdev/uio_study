

#include <stdio.h>
#include <slankdev/intfd.h>
constexpr uint16_t INTEL = 0x8086;
constexpr uint16_t IXGBE_X540 = 0x1528;

int main()
{
  slankdev::intfd uio_fd("/dev/uio0", O_RDONLY);
  slankdev::intfd configfd("/sys/class/uio/uio0/device/config", O_RDWR);
  printf("configfd: %d \n", configfd.get_fd());

  uint16_t vendor_id;
  uint16_t device_id;
  configfd.pread(&vendor_id, 2, 0);
  configfd.pread(&device_id, 2, 2);
  if (!(vendor_id==INTEL && device_id==IXGBE_X540))
    throw slankdev::exception("device is not X540!!!");

  uint32_t bar;
  configfd.pread(&bar, 4, 0x10);
  printf("BAR: %x \n", bar);

  uint32_t size;
  uint32_t tmp = 0xffffffff;
  configfd.pwrite(&tmp, 4, 0x10);
  configfd.pread(&size, 4, 0x10);
  size = ~size + 1;
  configfd.pwrite(&bar, 4, 0x10);

  slankdev::intfd fd("/sys/class/uio/uio0/device/resource0", O_RDWR);
  void* addr = fd.mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_SHARED, 0);
  printf("addr: %p \n", addr);
}


