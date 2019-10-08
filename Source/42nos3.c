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
static char BusName[120] = "command";
static char ConnectionString[120] = "tcp://127.0.0.1:12001";
static unsigned short int TelemetryPortToNOS3 = 4242;
#define AbsoluteMaxWriteSockets 10
static int MaxTelemetryConnections = 1;
static unsigned short int CommandPortFromNOS3 = 4243;
#define AbsoluteMaxReadSockets 5
static int MaxCommandConnections = 1;
static int EchoCommands = 0;

typedef struct NE_Bus NE_Bus;
typedef int64_t NE_SimTime;
static void *NOSHandle = NULL; /* static so the handle remains at file scope */
static NE_Bus *Bus = NULL;     /* ditto */
static NE_SimTime (*NE_bus_get_time)(NE_Bus *) = NULL; /* Pointer to get time function from the library */

static void ReleaseHandle(void) {
    if (NOSHandle != NULL) dlclose(NOSHandle);
}

/* Forward declarations: */
static void ReadNos3InpFile(void);
static void InitializeTimeNode(void);
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
    ticks = NE_bus_get_time(Bus);
    abs_time = AbsTime0 + (ticks * DTSIM);
    jd = AbsTimeToJD(abs_time);
    JD2YMDHMS(jd, year, month, day, hour, minute, second);
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
            fscanf(infile, "%u %[^\n] %[\n]", &tempu, junk, &newline);
            if (tempu < 65536) TelemetryPortToNOS3 = tempu;
            fscanf(infile, "%u %[^\n] %[\n]", &tempu, junk, &newline);
            if (tempu < AbsoluteMaxWriteSockets) MaxTelemetryConnections = tempu;
            fscanf(infile, "%s %[^\n] %[\n]", response, junk, &newline);
            if (DecodeString(response)) EchoEnabled = 1;
            fscanf(infile, "%u %[^\n] %[\n]", &tempu, junk, &newline);
            if (tempu < 65536) CommandPortFromNOS3 = tempu;
            fscanf(infile, "%u %[^\n] %[\n]", &tempu, junk, &newline);
            if (tempu < AbsoluteMaxReadSockets) MaxCommandConnections = tempu;
            fscanf(infile, "%s %[^\n] %[\n]", response, junk, &newline);
            EchoCommands = DecodeString(response);
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
        printf("NOS3Time error:  %s\n", error);
        exit(1);
    }

    NE_bus_get_time = (NE_SimTime (*)(NE_Bus *))dlsym(NOSHandle, "NE_bus_get_time");
#pragma GCC diagnostic pop
    if ((error = dlerror()) != NULL)  {
        printf("NOS3Time error:  %s\n", error);
        exit(1);
    }

    Bus = NE_create_bus2(BusName, ConnectionString);
    return;
}
#endif

#if defined(_ENABLE_NOS3_FSW_) && defined(_ENABLE_SOCKETS_) && defined(__linux__)
#include <pthread.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>

/* State */
static pthread_mutex_t WriteStateMutex = PTHREAD_MUTEX_INITIALIZER;
static int NumWriteSockets = 0;
static int WriteSockets[AbsoluteMaxWriteSockets];
static pthread_mutex_t ReadStateMutex = PTHREAD_MUTEX_INITIALIZER;
static int NumReadSockets = 0;
static int ReadSockets[AbsoluteMaxReadSockets];

/* Forward declarations */
static void InitializeSocketServer(void *(*manager_routine) (void *), unsigned short int *port);
static void *TelemetrySocketServerManager(void *arg);
static void *CommandSocketServerManager(void *arg);
static void WriteBufferToSockets(void);
static void ReceiveBufferFromSockets(void);

/* Public interface */
void NOS3SendMessageToFSW(void)
{
    int i;
    static long First = 1;
    if (First) {
        First = 0;
        ReadNos3InpFile();
        for (i = 0; i < MaxTelemetryConnections; i++) WriteSockets[i] = 0;
        InitializeSocketServer(TelemetrySocketServerManager, &TelemetryPortToNOS3);
    }
    pthread_mutex_lock(&WriteStateMutex);
    WriteBufferToSockets();
    pthread_mutex_unlock(&WriteStateMutex);
}

void NOS3ReceiveMessageFromFSW(void)
{
    static long First = 1;
    if (First) {
        First = 0;
        ReadNos3InpFile();
        InitializeSocketServer(CommandSocketServerManager, &CommandPortFromNOS3);
    }
    pthread_mutex_lock(&ReadStateMutex);
    ReceiveBufferFromSockets();
    pthread_mutex_unlock(&ReadStateMutex);
}

/* Private helper functions */

/* Forward declarations */
static int InitServer(unsigned short int port);
static int ConnectClientToSocketServer(int init_sockfd);
static int ReceiveBufferFromSocket(int sockfd);
static void HandleCommand(char *line);

static void InitializeSocketServer(void *(*manager_routine) (void *), unsigned short int *port)
{
    pthread_t manager_thread;
    if (pthread_create(&manager_thread, NULL, manager_routine, port)) {
        printf("Could not create a thread for the server socket manager.");
        exit(1);
    }
}

static void *TelemetrySocketServerManager(void *arg)
{
    unsigned short int port;
    int init_sockfd;
    int socket;
    struct sigaction act;

    act.sa_handler = SIG_IGN;
    sigemptyset (&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGPIPE, &act, NULL); /* Handle the client closing the socket */

    port = *((unsigned short int *)arg);
    init_sockfd = InitServer(port);
    printf("Telemetry server listening.    Port=%u\n", port);

    while ((socket = ConnectClientToSocketServer(init_sockfd))) {
        if (NumWriteSockets < MaxTelemetryConnections) {
            pthread_mutex_lock(&WriteStateMutex);
            WriteSockets[NumWriteSockets] = socket;
            NumWriteSockets++;
            printf("Telemetry socket established.  Number of sockets=%d\n", NumWriteSockets);
            pthread_mutex_unlock(&WriteStateMutex);
        } else { 
            close(socket); /* Already at capacity */
        }
    }
    close(init_sockfd);
    return NULL;
}

static void *CommandSocketServerManager(void *arg)
{
    unsigned short int port;
    int init_sockfd;
    int socket;
    struct sigaction act;
    
    act.sa_handler = SIG_IGN;
    sigemptyset (&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGPIPE, &act, NULL); /* Handle the client closing the socket */

    port = *((unsigned short int *)arg);
    init_sockfd = InitServer(port);
    printf("Command   server listening.    Port=%u\n", port);

    while ((socket = ConnectClientToSocketServer(init_sockfd))) {
        if (NumReadSockets < MaxCommandConnections) {
            pthread_mutex_lock(&ReadStateMutex);
            ReadSockets[NumReadSockets] = socket;
            NumReadSockets++;
            printf("Command   socket established.  Number of sockets=%d\n", NumReadSockets);
            pthread_mutex_unlock(&ReadStateMutex);
        } else {
            close(socket); /* Already at capacity */
        }
    }
    close(init_sockfd);
    return NULL;
}

static int InitServer(unsigned short int port)
{
    int init_sockfd;
    struct sockaddr_in Server;

    init_sockfd = socket(AF_INET,SOCK_STREAM,0);
    if (init_sockfd < 0) {
        printf("Error opening server socket.\n");
        exit(1);
    }
    memset((char *) &Server,0,sizeof(Server));
    Server.sin_family = AF_INET;
    Server.sin_addr.s_addr = INADDR_ANY;
    Server.sin_port = htons(port);
    if (bind(init_sockfd,(struct sockaddr *) &Server,sizeof(Server)) < 0) {
        printf("Error on binding server socket to port %u.\n", port);
        exit(1);
    }
    listen(init_sockfd,5);
    return init_sockfd;
}

static int ConnectClientToSocketServer(int init_sockfd)
{
    int flags;
    struct sockaddr_in Client;
    socklen_t clilen = sizeof(Client);
    int sockfd = accept(init_sockfd,(struct sockaddr *) &Client,&clilen);
    if (sockfd < 0) {
        printf("Error on accepting client socket.\n");
        exit(1);
    }

    /* Keep read() from waiting for message to come */
    flags = fcntl(sockfd, F_GETFL, 0);
    fcntl(sockfd,F_SETFL, flags|O_NONBLOCK);

    return(sockfd);
}

void WriteToSocket(SOCKET Socket); /* Reuse from IPC so we get updates to that code for no work. */

static void WriteBufferToSockets()
{
    int status = 0;
    char *nos3eof = "";
    int i, j;
    if (EchoEnabled) WriteToSocket(1); /* FD 1 = stdout */
    for (i = 0; i < MaxTelemetryConnections; i++) {
        if (WriteSockets[i] != 0) {
            WriteToSocket(WriteSockets[i]);
            status = write(WriteSockets[i], nos3eof, strlen(nos3eof)); /* Determine if the socket was closed on the other end. */
            if (status == -1) {
                for (j = i; j < MaxTelemetryConnections-1; j++) {
                    WriteSockets[j] = WriteSockets[j+1];
                }
                WriteSockets[MaxTelemetryConnections-1] = 0;
                NumWriteSockets--;
                printf("Telemetry socket closed.       Number of sockets=%d\n", NumWriteSockets);
            }
        }
    }
}

static void ReceiveBufferFromSockets(void)
{
    int error;
    int i, j;
    for (i = 0; i < MaxCommandConnections; i++) {
        if (ReadSockets[i] != 0) {
            error = ReceiveBufferFromSocket(ReadSockets[i]);
            if (error) {
                for (j = i; j < MaxCommandConnections-1; j++) {
                    ReadSockets[j] = ReadSockets[j+1];
                }
                ReadSockets[MaxCommandConnections-1] = 0;
                NumReadSockets--;
                printf("Command   socket closed.       Number of sockets=%d\n", NumReadSockets);
            }
        }
    }
}

static int ReceiveBufferFromSocket(int sockfd)
{
    char line[512];
    int status;
    status = read(sockfd, line, sizeof(line));
    if (status > 0) {
        line[status-1] = '\0';
        if (EchoCommands) printf("%s\n", line);
        /* Now decide what commands to implement */
        HandleCommand(line);
        return 0; /* Successful read */
    } else if (status == 0) {
        return 1; /* FD was closed, return 1 to end */
    } else { /* status < 0 */
        if (errno == EAGAIN) {
            return 0; /* Nothing to read */
        } else {
            printf("ReceiveBufferFromSocket: Error reading data: %d\n", errno);
            return errno;
        }
    }
}

static void HandleCommand(char *line)
{
    long int sc, whl, thr, gimbal, mtb;
    double trq, pulse, ax, ay, az, arx, ary, arz, mcmd;
    if (sscanf(line, "SC[%ld] Whl[%ld] TorqueCommand = %lf", &sc, &whl, &trq) == 3) {
        if ((sc < Nsc) && ((&SC[sc])->Exists) && (whl < (&SC[sc])->Nw)) {
            (&SC[sc])->AC.Whl[whl].Tcmd = trq;
        }
    }
    if (sscanf(line, "SC[%ld] Thr[%ld] PulseWidthCommand = %lf", &sc, &thr, &pulse) == 3) {
        if ((sc < Nsc) && ((&SC[sc])->Exists) && (thr < (&SC[sc])->Nthr)) {
            (&SC[sc])->AC.Thr[thr].PulseWidthCmd = pulse;
        }
    }
    if (sscanf(line, "SC[%ld] Gimbal[%ld] AngCmd = [%lf %lf %lf] AngRateCmd = [%lf %lf %lf]", &sc, &gimbal, &ax, &ay, &az, &arx, &ary, &arz) == 8) {
        if ((sc < Nsc) && ((&SC[sc])->Exists) && (gimbal < (&SC[sc])->Ng)) {
            (&SC[sc])->AC.G[gimbal].Cmd.Ang[0] = ax;
            (&SC[sc])->AC.G[gimbal].Cmd.Ang[1] = ay;
            (&SC[sc])->AC.G[gimbal].Cmd.Ang[2] = az;
            (&SC[sc])->AC.G[gimbal].Cmd.AngRate[0] = arx;
            (&SC[sc])->AC.G[gimbal].Cmd.AngRate[1] = ary;
            (&SC[sc])->AC.G[gimbal].Cmd.AngRate[2] = arz;
        }
    }
    if (sscanf(line, "SC[%ld] MTB[%ld] Mcmd = %lf", &sc, &mtb, &mcmd) == 3) {
        if ((sc < Nsc) && ((&SC[sc])->Exists) && (mtb < (&SC[sc])->Nmtb)) {
            (&SC[sc])->AC.MTB[mtb].Mcmd = mcmd;
        }
    }
}
#endif
