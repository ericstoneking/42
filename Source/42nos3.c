/*    This file is distributed with 42,                               */
/*    the (mostly harmless) spacecraft dynamics simulation            */
/*    created by Eric Stoneking of NASA Goddard Space Flight Center   */

/*    Copyright 2018 United States Government                         */
/*    as represented by the Administrator                             */
/*    of the National Aeronautics and Space Administration.           */

/*    No copyright is claimed in the United States                    */
/*    under Title 17, U.S. Code.                                      */

/*    All Other Rights Reserved.                                      */

#define _POSIX_C_SOURCE 1 /* For signal handling */
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

#include "42.h"

#if defined(__linux__)
#include <semaphore.h>
static char BusName[120] = "command";
static char ConnectionString[120] = "tcp://127.0.0.1:12001";
static sem_t sem;

typedef struct NE_Bus NE_Bus;
typedef int64_t NE_SimTime;
typedef size_t NE_TimeTickCallbackId;
typedef void (*NE_TimeTickCallbackFunction)(NE_SimTime time);
static void *NOSHandle = NULL; /* static so the handle remains at file scope */
static NE_Bus *Bus = NULL;     /* ditto */
static NE_SimTime (*NE_bus_get_time)(NE_Bus *) = NULL; /* Pointer to get time function from the library */
static NE_TimeTickCallbackId (*NE_bus_add_time_tick_callback)(NE_Bus *, NE_TimeTickCallbackFunction) = NULL; /* Pointer to add time tick callback function from the library */

static void ReleaseHandle(void) {
    if (NOSHandle != NULL) dlclose(NOSHandle);
}

/* Forward declarations: */
static void ReadNos3InpFile(void);
static void InitializeTimeNode(void);
static void TimeTickCallback(NE_SimTime time);
#endif

void NOS3Time(long *year, long *day_of_year, long *month, long *day, long *hour, long *minute, double *second)
{

#if defined(_WIN32)
    printf("NOS3Time:  NOS3 time mode is not supported on _WIN32\n");
    /* Well, maybe it could be supported... but we would need to get all the NOS Engine stuff hooked up there... and we do not need to at this time. */
    exit(1);
#elif defined(__APPLE__)
    printf("NOS3Time:  NOS3 time mode is not supported on __APPLE__\n");
    /* Well, maybe it could be supported... but we would need to get all the NOS Engine stuff hooked up there... and we do not need to at this time. */
    exit(1);
#elif defined(__linux__)
    static long First = 1;
    int64_t ticks;
    double abs_time;
    double jd;
    if (First) {
        First = 0;
        ReadNos3InpFile();
        InitializeTimeNode();
    }
    if (sem_wait(&sem) == -1) {
        perror("NOS3Time error on sem_wait");
        exit(3);
    }
    ticks = NE_bus_get_time(Bus);
    abs_time = DynTime0 + (ticks * DTSIM) - LeapSec - 32.184;
    jd = TimeToJD(abs_time);
    JDToDate(jd, year, month, day, hour, minute, second);
    *day_of_year = MD2DOY(*year, *month, *day);
#else
#error "Unknown operating system in NOS3Time.  Fix that!"
    printf("Unknown operating system in NOS3Time.  Bailing out.\n");
    exit(1);
#endif
}

#if defined(__linux__)
static void ReadNos3InpFile(void)
{
    static long First = 1;
    char junk[120], newline, response[120];
    char *File = "Inp_NOS3.txt";
    char *FileName;
    FILE *infile;
    unsigned int tempu;
    if (First) {
        First = 0;
        FileName = (char *) calloc(strlen(InOutPath) + strlen(File) + 1, sizeof(char));
        strcpy(FileName, InOutPath);
        strcat(FileName, File);
        infile = fopen(FileName, "r");
        if(infile != NULL) {
            fscanf(infile, "%[^\n] %[\n]", junk, &newline);
            fscanf(infile, "%s %[^\n] %[\n]", BusName, junk, &newline);
            fscanf(infile, "%s %[^\n] %[\n]", ConnectionString, junk, &newline);
            fclose(infile);
        }
        free(FileName);
    }
}

static void InitializeTimeNode(void)
{
    char *error;
    NE_Bus *(*NE_create_bus2)(const char *, const char *);

    NOSHandle = dlopen("libnos_engine_client_cxx11.so", RTLD_LAZY | RTLD_LOCAL);
    if (NOSHandle == NULL) {
        printf("NOS3Time:  NOS3 shared object libraries required for NOS3 time mode were not loaded.  Bailing out.\n");
        exit(1);
    }
    atexit(ReleaseHandle); /* Release the handle to the shared object library at exit */

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
    NE_create_bus2 = (NE_Bus *(*)(const char *, const char *))dlsym(NOSHandle, "NE_create_bus2");
    if ((error = dlerror()) != NULL)  {
        printf("NOS3Time error finding symbol NE_create_bus2:  %s\n", error);
        exit(1);
    }

    NE_bus_get_time = (NE_SimTime (*)(NE_Bus *))dlsym(NOSHandle, "NE_bus_get_time");
    if ((error = dlerror()) != NULL)  {
        printf("NOS3Time error finding symbol NE_bus_get_time:  %s\n", error);
        exit(1);
    }

    NE_bus_add_time_tick_callback = 
        (NE_TimeTickCallbackId (*)(NE_Bus *, NE_TimeTickCallbackFunction))dlsym(NOSHandle, "NE_bus_add_time_tick_callback");
    if ((error = dlerror()) != NULL)  {
        printf("NOS3Time error finding symbol NE_bus_add_time_tick_callback:  %s\n", error);
        exit(1);
    }
#pragma GCC diagnostic pop

    Bus = NE_create_bus2(BusName, ConnectionString);
    NE_bus_add_time_tick_callback(Bus, TimeTickCallback);
    return;
}

static void TimeTickCallback(NE_SimTime time)
{
    if (sem_post(&sem) == -1) {
        perror("NOS3Time error on sem_post");
	exit(2);
    }
}

#endif
