#include"Drone_Epoll.h"

/*  UDP socket PORT number define */
#define __CONTROL_PORT__ 5556       //about controller
#define __STATUS_PORT__ 5554        //status data
#define __VIDEO_PORT__ 5555         //camara data not used

/*  TCP socket PORT number define */
#define __HANDLE_PORT__ 5559        //


typedef struct __CONTROL_DATA_FORMAT__ {
    uint8_t tag;
    uint8_t size;
} __attribute__ ((packed)) controll_Format;

typedef struct __STATUS_DATA_FORMAT__ {
    uint8_t tag;
    uint8_t size;
} __attribute__ ((packed)) status_Format;

typedef struct __HANDLE_DATA_FORMAT__ {
    uint8_t tag;
    uint8_t size;
} __attribute__ ((packed)) handle_Format;


int network_init();
int network_run();
int network_exit();
int network_error();