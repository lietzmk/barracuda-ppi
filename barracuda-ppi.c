#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdbool.h>
#include <sys/ioctl.h>
#include </usr/include/dev/ppbus/ppi.h>
#include </usr/include/dev/ppbus/ppbconf.h>

// Pin 4 Network ByPass
static uint8_t NETWORK_BYPASS = 8;
// Pin 6 Red LED
static uint8_t RED_LED = 32;
// Pin 7 Yellow LED
static uint8_t YELLOW_LED = 64;
// Pin 8 Green LED
static uint8_t GREEN_LED = 128;

static int ppi_fd;

static void do_init(void)
{
  char port[] = "/dev/ppi0";

  ppi_fd = open(port, O_RDWR);
  if( ppi_fd < 0 ) {
    perror(port);
    exit(1);
  }
}

static void do_out(unsigned long outval)
{
  uint8_t val = outval, n;

  n = ioctl(ppi_fd, PPISDATA, &val);
  ioctl(ppi_fd, PPIGCTRL, &val);
  ioctl(ppi_fd, PPISCTRL, &val);
  if( n < 0 ) {
    perror("ioctl PPISDATA");
    exit(1);
  }
}

static unsigned long do_in(void)
{
  uint8_t val, n;

  n = ioctl(ppi_fd, PPIGDATA, &val);
  if( n < 0 ) {
    perror("ioctl PPIGSTATUS");
    exit(1);
  }
  return val;
}

void set_pin( uint8_t *val, char pinValue, uint8_t *Pin ) {
  if( !(*val & *Pin) ) {
    if( pinValue == 48 ) {
      *val = *val + *Pin;
    }
  } else if( *val & *Pin ) {
    if( pinValue == 49 ) {
      *val = *val - *Pin;
    }
  }
}

int main( int argc, char *argv[] )
{
  bool printStatus = false;
  do_init();
  uint8_t val = do_in();
  uint8_t startval = val;

  for(int i = 1; i < argc; ++i) {
    if( strcmp(argv[i], "--green") == 0 ||
        strcmp(argv[i], "-g") == 0 ) {
      ++i;
      set_pin( &val, *argv[i], &GREEN_LED );
    } else if( strcmp(argv[i], "--yellow") == 0 ||
               strcmp(argv[i], "-y") == 0 ) {
      ++i;
      set_pin( &val, *argv[i], &YELLOW_LED );
    } else if( strcmp(argv[i], "--red") == 0 ||
               strcmp(argv[i], "-r" ) == 0 ) {
      ++i;
      set_pin( &val, *argv[i], &RED_LED );
    } else if( strcmp(argv[i], "--port") == 0 ||
               strcmp(argv[i], "-p")  == 0 ) {
      ++i;
      set_pin( &val, *argv[i], &NETWORK_BYPASS );
    } else if( strcmp(argv[i], "--all") == 0 ||
               strcmp(argv[i], "-a") == 0 ) {
      ++i;
      set_pin( &val, *argv[i], &GREEN_LED );
      set_pin( &val, *argv[i], &YELLOW_LED );
      set_pin( &val, *argv[i], &RED_LED );
    } else if( strcmp(argv[i], "--status") == 0 ||
               strcmp(argv[i], "-s") == 0 ) {
      printStatus = true;
    } else {
      printf("Barracuda Parallel Port Manager\n"
        "Usage: barracuda-ppi [OPTION] [1 turn on, 0 turn off]\n"
        "-g, --green   turn green LED on/off\n"
        "-y, --yellow  turn yellow LED on/off\n"
        "-r, --red     turn red LED on/off\n"
        "-a, --all     turn all LED's on/off\n"
        "-p, --port    turn port bypass on/off\n"
        "-s, --status  print LED/port status\n" );
    }
  }

  if( startval != val ) {
    do_out( val );
  }

  if(printStatus) {
    if(!(val & GREEN_LED))
      printf("Green LED is on.\n");
    if(!(val & YELLOW_LED))
      printf("Yellow LED is on.\n");
    if(!(val & RED_LED))
      printf("Red LED is on.\n");
    if(val & NETWORK_BYPASS)
      printf("Network Bypass is off.\n");
    else
      printf("Network Bypass is on.\n");
  }

  return 0;
}
