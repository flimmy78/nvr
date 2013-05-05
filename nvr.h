/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _NVR_H_RPCGEN
#define _NVR_H_RPCGEN

#include <rpc/rpc.h>


#ifdef __cplusplus
extern "C" {
#endif

#define camerIDLEN 256
#define HEAD_LEN 10240
#define describeLEN 1024
#define CMN_LEN 20
#define IPLEN 64
#define userNameLEN 256
#define pwdLen 256 

typedef unsigned int u_int;
typedef unsigned long u_long;
typedef unsigned long long u_long_long;

struct LOGINargs {
    char *ip;
    int port;
    char *userName;
    char *pwd;
};
typedef struct LOGINargs LOGINargs;

struct CMMNargs {
    u_int loginID;
    char *camerID;
    int recordType;
    u_int beginTime;
    u_int endTime;
    u_int mode;
};
typedef struct CMMNargs CMMNargs;

struct READargs {
    u_int playHandle;
    u_int readSize;
    u_int mode;
};
typedef struct READargs READargs;

typedef CMMNargs SEARCHargs;

struct RECORDBYORDERargs {
    u_int num;
    char *camerID;
    u_int beginTime;
    u_int endTime;
};
typedef struct RECORDBYORDERargs RECORDBYORDERargs;

struct RECORDBYORDERres {
	u_int flag;
	u_int beginTime;
	u_int endTime;
	char *describe;
	int describeLen;
};
typedef struct RECORDBYORDERres RECORDBYORDERres;

struct READres {
u_int dataLen;
struct {
    u_int data_len;
    char *data_val;
} data;
};
typedef struct READres READres;

struct HEADERinfo {
struct {
    u_int data_len;
    char *data_val;
} data;
u_int beginTime;
u_int endTime;
u_int nextBeginTime;
u_int recordSize;
};
typedef struct HEADERinfo HEADERinfo;

struct SEARCHres {
struct CMMNargs cmmnArgs;
char *describe;
int describeLen;
u_int nextBeginTime;
};
typedef struct SEARCHres SEARCHres;

struct CREATEargs {
u_int loginID;
char *camerID;
char *camerAlias;
int recordType;
struct {
    u_int header_len;
    char *header_val;
} header;
char *describe;
int describeLen;
};
typedef struct CREATEargs CREATEargs;

struct WRITEargs {
    u_int recordHandle;
    u_int beginTime;
    struct {
        u_int data_len;
        char *data_val;
    } data;
};
typedef struct WRITEargs WRITEargs;

typedef CMMNargs DELargs;

struct VOLUMinfo {
    u_int volSizeHigh;
    u_int volSizeLow;
    u_int usedSizeHigh;
    u_int usedSizeLow;
    int savedDays;
    int delPolicy;
    int encoderType;
    int resolusion;
    int datarate;
};
typedef struct VOLUMinfo VOLUMinfo;

typedef SEARCHres SETRECINFOargs;

struct GETRECSIZEargs {
    char *camerID;
    u_int beginTime;
    u_int endTime;
};
typedef struct GETRECSIZEargs GETRECSIZEargs;

struct CREATRECVOLargs {
    char *volumeid;
    char *name;
    char *alias;
    short savedDays;
    char delPolicy;
    char encodeType;
    u_int blocks;
    u_int blockSize;
};
typedef struct CREATRECVOLargs CREATRECVOLargs;

struct DELRECVOLargs {
char *cameraID;
int mode;
};
typedef struct DELRECVOLargs DELRECVOLargs;

struct DELVEDVOLargs {
char *volname;
int mode;
};
typedef struct DELVEDVOLargs DELVEDVOLargs;

   
#define NVRPROG 0x30090949
#define NVRVERS 1

#if defined(__STDC__) || defined(__cplusplus)
#define NVRPROC_OPEN 1
    extern u_int *nvrproc_open_1(CMMNargs *, CLIENT *);
    extern u_int *nvrproc_open_1_svc(CMMNargs *, struct svc_req *);
#define NVRPROC_CLOSE 2
    extern u_int *nvrproc_close_1(u_int *, CLIENT *);
    extern u_int *nvrproc_close_1_svc(u_int *, struct svc_req *);
#define NVRPROC_READ 3
    extern READres *nvrproc_read_1(READargs *, CLIENT *);
    extern READres *nvrproc_read_1_svc(READargs *, struct svc_req *);
#define NVRPROC_WRITE 4
    extern u_int *nvrproc_write_1(WRITEargs *, CLIENT *);
    extern u_int *nvrproc_write_1_svc(WRITEargs *, struct svc_req *);
#define NVRPROC_GETHEADER 5
    extern HEADERinfo *nvrproc_getheader_1(CMMNargs *, CLIENT *);
    extern HEADERinfo *nvrproc_getheader_1_svc(CMMNargs *,
					       struct svc_req *);
#define NVRPROC_CREATE 6
    extern u_int *nvrproc_create_1(CREATEargs *, CLIENT *);
    extern u_int *nvrproc_create_1_svc(CREATEargs *, struct svc_req *);
#define NVRPROC_SEARCHRECORD 7
    extern SEARCHres *nvrproc_searchrecord_1(SEARCHargs *, CLIENT *);
    extern SEARCHres *nvrproc_searchrecord_1_svc(SEARCHargs *,
						 struct svc_req *);
#define NVRPROC_SETRECINFO 8
    extern u_int *nvrproc_setrecinfo_1(SETRECINFOargs *, CLIENT *);
    extern u_int *nvrproc_setrecinfo_1_svc(SETRECINFOargs *,
					   struct svc_req *);
#define NVRPROC_GETVOLUMEINFO 9
    extern VOLUMinfo *nvrproc_getvolumeinfo_1(u_int *, CLIENT *);
    extern VOLUMinfo *nvrproc_getvolumeinfo_1_svc(u_int *,
						  struct svc_req *);
#define NVRPROC_DELETE 10
    extern u_int *nvrproc_delete_1(DELargs *, CLIENT *);
    extern u_int *nvrproc_delete_1_svc(DELargs *, struct svc_req *);
#define NVRPROC_LOGIN 11
    extern u_int *nvrproc_login_1(LOGINargs *, CLIENT *);
    extern u_int *nvrproc_login_1_svc(LOGINargs *, struct svc_req *);
#define NVRPROC_LOGOUT 12
    extern int *nvrproc_logout_1(u_int *, CLIENT *);
    extern int *nvrproc_logout_1_svc(u_int *, struct svc_req *);
#define NVRPROC_GETLASTERROR 13
    extern u_long *nvrproc_getlasterror_1(void *, CLIENT *);
    extern u_long *nvrproc_getlasterror_1_svc(void *, struct svc_req *);
#define NVRPROC_GETRECSIZE 14
    extern u_int *nvrproc_getrecsize_1(GETRECSIZEargs *, CLIENT *);
    extern u_int *nvrproc_getrecsize_1_svc(GETRECSIZEargs *,
					   struct svc_req *);
#define NVRPROC_CREATRECVOL 15
    extern int *nvrproc_creatrecvol_1(CREATRECVOLargs *, CLIENT *);
    extern int *nvrproc_creatrecvol_1_svc(CREATRECVOLargs *,
					  struct svc_req *);
#define NVRPROC_DELRECVOL 16
    extern int *nvrproc_delrecvol_1(DELRECVOLargs *, CLIENT *);
    extern int *nvrproc_delrecvol_1_svc(DELRECVOLargs *, struct svc_req *);
	
#define NVRPROC_SEARCHRECORDBYORDER 17
	extern RECORDBYORDERres* nvrproc_searchrecordbyorder_1(RECORDBYORDERargs*,   CLIENT *);
	extern RECORDBYORDERres* nvrproc_searchrecordbyorder_1_svc(RECORDBYORDERargs*,   struct svc_req *);

#define NVRPROC_DELVEDVOL 18         
    extern int *nvrproc_delvedvol_1(DELVEDVOLargs *, CLIENT *);
    extern int *nvrproc_delvedvol_1_svc(DELVEDVOLargs *, struct svc_req *);    
	
    extern int nvrprog_1_freeresult(SVCXPRT *, xdrproc_t, caddr_t);

#else				/* K&R C */
#define NVRPROC_OPEN 1
    extern u_int *nvrproc_open_1();
    extern u_int *nvrproc_open_1_svc();
#define NVRPROC_CLOSE 2
    extern u_int *nvrproc_close_1();
    extern u_int *nvrproc_close_1_svc();
#define NVRPROC_READ 3
    extern READres *nvrproc_read_1();
    extern READres *nvrproc_read_1_svc();
#define NVRPROC_WRITE 4
    extern u_int *nvrproc_write_1();
    extern u_int *nvrproc_write_1_svc();
#define NVRPROC_GETHEADER 5
    extern HEADERinfo *nvrproc_getheader_1();
    extern HEADERinfo *nvrproc_getheader_1_svc();
#define NVRPROC_CREATE 6
    extern u_int *nvrproc_create_1();
    extern u_int *nvrproc_create_1_svc();
#define NVRPROC_SEARCHRECORD 7
    extern SEARCHres *nvrproc_searchrecord_1();
    extern SEARCHres *nvrproc_searchrecord_1_svc();
#define NVRPROC_SETRECINFO 8
    extern u_int *nvrproc_setrecinfo_1();
    extern u_int *nvrproc_setrecinfo_1_svc();
#define NVRPROC_GETVOLUMEINFO 9
    extern VOLUMinfo *nvrproc_getvolumeinfo_1();
    extern VOLUMinfo *nvrproc_getvolumeinfo_1_svc();
#define NVRPROC_DELETE 10
    extern u_int *nvrproc_delete_1();
    extern u_int *nvrproc_delete_1_svc();
#define NVRPROC_LOGIN 11
    extern u_int *nvrproc_login_1();
    extern u_int *nvrproc_login_1_svc();
#define NVRPROC_LOGOUT 12
    extern int *nvrproc_logout_1();
    extern int *nvrproc_logout_1_svc();
#define NVRPROC_GETLASTERROR 13
    extern u_long *nvrproc_getlasterror_1();
    extern u_long *nvrproc_getlasterror_1_svc();
#define NVRPROC_GETRECSIZE 14
    extern u_int *nvrproc_getrecsize_1();
    extern u_int *nvrproc_getrecsize_1_svc();
#define NVRPROC_CREATRECVOL 15
    extern int *nvrproc_creatrecvol_1();
    extern int *nvrproc_creatrecvol_1_svc();
#define NVRPROC_DELRECVOL 16
    extern int *nvrproc_delrecvol_1();
    extern int *nvrproc_delrecvol_1_svc();

		
#define NVRPROC_SEARCHRECORDBYORDER 17
	extern RECORDBYORDERres* nvrproc_searchrecordbyorder_1();
	extern RECORDBYORDERres* nvrproc_searchrecordbyorder_1_svc();

#define NVRPROC_DELVEDVOL 18         
    extern int *nvrproc_delvedvol_1();
    extern int *nvrproc_delvedvol_1_svc();

    extern int nvrprog_1_freeresult();
#endif				/* K&R C */

/* the xdr functions */

#if defined(__STDC__) || defined(__cplusplus)
    extern bool_t xdr_LOGINargs(XDR *, LOGINargs *);
    extern bool_t xdr_CMMNargs(XDR *, CMMNargs *);
    extern bool_t xdr_READargs(XDR *, READargs *);
    extern bool_t xdr_SEARCHargs(XDR *, SEARCHargs *);
    extern bool_t xdr_READres(XDR *, READres *);
    extern bool_t xdr_HEADERinfo(XDR *, HEADERinfo *);
    extern bool_t xdr_SEARCHres(XDR *, SEARCHres *);
    extern bool_t xdr_CREATEargs(XDR *, CREATEargs *);
    extern bool_t xdr_WRITEargs(XDR *, WRITEargs *);
    extern bool_t xdr_DELargs(XDR *, DELargs *);
    extern bool_t xdr_VOLUMinfo(XDR *, VOLUMinfo *);
    extern bool_t xdr_SETRECINFOargs(XDR *, SETRECINFOargs *);
    extern bool_t xdr_GETRECSIZEargs(XDR *, GETRECSIZEargs *);
    extern bool_t xdr_CREATRECVOLargs(XDR *, CREATRECVOLargs *);
    extern bool_t xdr_DELRECVOLargs(XDR *, DELRECVOLargs *);
    extern	bool_t xdr_RECORDBYORDERargs (XDR *, RECORDBYORDERargs*);
    extern  bool_t xdr_RECORDBYORDERres (XDR *, RECORDBYORDERres*);
    extern bool_t xdr_DELVEDVOLargs(XDR *, DELVEDVOLargs *);


#else				/* K&R C */
    extern bool_t xdr_LOGINargs();
    extern bool_t xdr_CMMNargs();
    extern bool_t xdr_READargs();
    extern bool_t xdr_SEARCHargs();
    extern bool_t xdr_READres();
    extern bool_t xdr_HEADERinfo();
    extern bool_t xdr_SEARCHres();
    extern bool_t xdr_CREATEargs();
    extern bool_t xdr_WRITEargs();
    extern bool_t xdr_DELargs();
    extern bool_t xdr_VOLUMinfo();
    extern bool_t xdr_SETRECINFOargs();
    extern bool_t xdr_GETRECSIZEargs();
    extern bool_t xdr_CREATRECVOLargs();
    extern bool_t xdr_DELRECVOLargs();
    extern bool_t xdr_RECORDBYORDERargs();
    extern boot_t xdr_RECORDBYORDERres();
    extern bool_t xdr_DELVEDVOLargs();

#endif				/* K&R C */

#ifdef __cplusplus
}
#endif
#endif				/* !_NVR_H_RPCGEN */
