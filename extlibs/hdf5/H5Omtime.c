/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by the Board of Trustees of the University of Illinois.         *
 * All rights reserved.                                                      *
 *                                                                           *
 * This file is part of HDF5.  The full HDF5 copyright notice, including     *
 * terms governing use, modification, and redistribution, is contained in    *
 * the files COPYING and Copyright.html.  COPYING can be found at the root   *
 * of the source code distribution tree; Copyright.html can be found at the  *
 * root level of an installed copy of the electronic HDF5 document set and   *
 * is linked from the top-level documents page.  It can also be found at     *
 * http://hdf.ncsa.uiuc.edu/HDF5/doc/Copyright.html.  If you do not have     *
 * access to either file, you may request a copy from hdfhelp@ncsa.uiuc.edu. *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* Programmer:	Robb Matzke <matzke@llnl.gov>
 *		Friday, July 24, 1998
 *
 * Purpose:	The object modification time message.
 */

#define H5O_PACKAGE	/*suppress error about including H5Opkg	  */

#include "H5private.h"
#include "H5Eprivate.h"
#include "H5FLprivate.h"	/*Free Lists	  */
#include "H5MMprivate.h"
#include "H5Opkg.h"             /* Object header functions                  */

#if defined (WIN32) && !defined (__MWERKS__) 
#include <sys/types.h>
#include <sys/timeb.h>
#endif

#define PABLO_MASK	H5O_mtime_mask

static void *H5O_mtime_new_decode(H5F_t *f, hid_t dxpl_id, const uint8_t *p, H5O_shared_t *sh);
static herr_t H5O_mtime_new_encode(H5F_t *f, uint8_t *p, const void *_mesg);
static size_t H5O_mtime_new_size(H5F_t *f, const void *_mesg);

static void *H5O_mtime_decode(H5F_t *f, hid_t dxpl_id, const uint8_t *p, H5O_shared_t *sh);
static herr_t H5O_mtime_encode(H5F_t *f, uint8_t *p, const void *_mesg);
static void *H5O_mtime_copy(const void *_mesg, void *_dest);
static size_t H5O_mtime_size(H5F_t *f, const void *_mesg);
static herr_t H5O_mtime_reset(void *_mesg);
static herr_t H5O_mtime_free(void *_mesg);
static herr_t H5O_mtime_debug(H5F_t *f, hid_t dxpl_id, const void *_mesg, FILE *stream,
			     int indent, int fwidth);

/* This message derives from H5O */
const H5O_class_t H5O_MTIME[1] = {{
    H5O_MTIME_ID,		/*message id number		*/
    "mtime",			/*message name for debugging	*/
    sizeof(time_t),		/*native message size		*/
    H5O_mtime_decode,		/*decode message		*/
    H5O_mtime_encode,		/*encode message		*/
    H5O_mtime_copy,		/*copy the native value		*/
    H5O_mtime_size,		/*raw message size		*/
    H5O_mtime_reset,		/* reset method			*/
    H5O_mtime_free,		/* free method			*/
    NULL,		        /* file delete method		*/
    NULL,			/* link method			*/
    NULL,			/*get share method		*/
    NULL,			/*set share method		*/
    H5O_mtime_debug,		/*debug the message		*/
}};

/* This message derives from H5O */
/* (Only encode, decode & size routines are different from old mtime routines) */
const H5O_class_t H5O_MTIME_NEW[1] = {{
    H5O_MTIME_NEW_ID,		/*message id number		*/
    "mtime_new",		/*message name for debugging	*/
    sizeof(time_t),		/*native message size		*/
    H5O_mtime_new_decode,	/*decode message		*/
    H5O_mtime_new_encode,	/*encode message		*/
    H5O_mtime_copy,		/*copy the native value		*/
    H5O_mtime_new_size,		/*raw message size		*/
    H5O_mtime_reset,		/* reset method			*/
    H5O_mtime_free,		/* free method			*/
    NULL,		        /* file delete method		*/
    NULL,			/* link method			*/
    NULL,			/*get share method		*/
    NULL,			/*set share method		*/
    H5O_mtime_debug,		/*debug the message		*/
}};

/* Interface initialization */
static int interface_initialize_g = 0;
#define INTERFACE_INIT	NULL

/* Current version of new mtime information */
#define H5O_MTIME_VERSION 	1	
 
/* Track whether tzset routine was called */
static int	ntzset=0;

/* Declare a free list to manage the time_t struct */
H5FL_DEFINE(time_t);


/*-------------------------------------------------------------------------
 * Function:	H5O_mtime_new_decode
 *
 * Purpose:	Decode a new modification time message and return a pointer to a
 *		new time_t value.
 *
 * Return:	Success:	Ptr to new message in native struct.
 *
 *		Failure:	NULL
 *
 * Programmer:	Quincey Koziol
 *		koziol@ncsa.uiuc.edu
 *		Jan  3 2002
 *
 * Modifications:
 *
 *-------------------------------------------------------------------------
 */
static void *
H5O_mtime_new_decode(H5F_t UNUSED *f, hid_t UNUSED dxpl_id, const uint8_t *p,
		 H5O_shared_t UNUSED *sh)
{
    time_t	*mesg, the_time;
    uint32_t    tmp_time;       /* Temporary copy of the time */
    int		version;        /* Version of mtime information */
    void        *ret_value;     /* Return value */

    FUNC_ENTER_NOAPI(H5O_mtime_new_decode, NULL);

    /* check args */
    assert(f);
    assert(p);
    assert (!sh);

    /* decode */
    version = *p++;
    if(version!=H5O_MTIME_VERSION)
        HGOTO_ERROR(H5E_OHDR, H5E_CANTLOAD, NULL, "bad version number for mtime message");

    /* Skip reserved bytes */
    p+=3;

    /* Get the time_t from the file */
    UINT32DECODE(p, tmp_time);
    the_time=(time_t)tmp_time;

    /* The return value */
    if (NULL==(mesg = H5FL_MALLOC(time_t)))
	HGOTO_ERROR(H5E_RESOURCE, H5E_NOSPACE, NULL, "memory allocation failed");
    *mesg = the_time;

    /* Set return value */
    ret_value=mesg;

done:
    FUNC_LEAVE_NOAPI(ret_value);
} /* end H5O_mtime_new_decode() */


/*-------------------------------------------------------------------------
 * Function:	H5O_mtime_decode
 *
 * Purpose:	Decode a modification time message and return a pointer to a
 *		new time_t value.
 *
 * Return:	Success:	Ptr to new message in native struct.
 *
 *		Failure:	NULL
 *
 * Programmer:	Robb Matzke
 *		matzke@llnl.gov
 *		Jul 24 1998
 *
 *-------------------------------------------------------------------------
 */
static void *
H5O_mtime_decode(H5F_t UNUSED *f, hid_t UNUSED dxpl_id, const uint8_t *p,
		 H5O_shared_t UNUSED *sh)
{
    time_t	*mesg, the_time;
    int	i;
    struct tm	tm;
    void        *ret_value;     /* Return value */

    FUNC_ENTER_NOAPI(H5O_mtime_decode, NULL);

    /* check args */
    assert(f);
    assert(p);
    assert (!sh);

    /* Initialize time zone information */
    if (!ntzset) {
        HDtzset();
        ntzset=1;
    } /* end if */

    /* decode */
    for (i=0; i<14; i++) {
	if (!HDisdigit(p[i]))
	    HGOTO_ERROR(H5E_OHDR, H5E_CANTINIT, NULL, "badly formatted modification time message");
    }

    /*
     * Convert YYYYMMDDhhmmss UTC to a time_t.  This is a little problematic
     * because mktime() operates on local times.  We convert to local time
     * and then figure out the adjustment based on the local time zone and
     * daylight savings setting.
     */
    HDmemset(&tm, 0, sizeof tm);
    tm.tm_year = (p[0]-'0')*1000 + (p[1]-'0')*100 +
		 (p[2]-'0')*10 + (p[3]-'0') - 1900;
    tm.tm_mon = (p[4]-'0')*10 + (p[5]-'0') - 1;
    tm.tm_mday = (p[6]-'0')*10 + (p[7]-'0');
    tm.tm_hour = (p[8]-'0')*10 + (p[9]-'0');
    tm.tm_min = (p[10]-'0')*10 + (p[11]-'0');
    tm.tm_sec = (p[12]-'0')*10 + (p[13]-'0');
    tm.tm_isdst = -1; /*figure it out*/
    if ((time_t)-1==(the_time=HDmktime(&tm)))
	HGOTO_ERROR(H5E_OHDR, H5E_CANTINIT, NULL, "badly formatted modification time message");

#if defined(H5_HAVE_TM_GMTOFF)
    /* FreeBSD, OSF 4.0 */
    the_time += tm.tm_gmtoff;
#elif defined(H5_HAVE___TM_GMTOFF)
    /* Linux libc-4 */
    the_time += tm.__tm_gmtoff;
#elif defined(H5_HAVE_TIMEZONE)
    /* Linux libc-5 */
    the_time -= timezone - (tm.tm_isdst?3600:0);
#elif defined(H5_HAVE_BSDGETTIMEOFDAY) && defined(H5_HAVE_STRUCT_TIMEZONE)
    /* Irix5.3 */
    {
        struct timezone tz;

        if (HDBSDgettimeofday(NULL, &tz)<0)
            HGOTO_ERROR(H5E_OHDR, H5E_CANTINIT, NULL, "unable to obtain local timezone information");
        the_time -= tz.tz_minuteswest * 60 - (tm.tm_isdst ? 3600 : 0);
    }
#elif defined(H5_HAVE_GETTIMEOFDAY) && defined(H5_HAVE_STRUCT_TIMEZONE) && defined(H5_GETTIMEOFDAY_GIVES_TZ)
    {
	struct timezone tz;
        struct timeval tv;  /* Used as a placebo; some systems don't like NULL */

	if (HDgettimeofday(&tv, &tz) < 0)
	    HGOTO_ERROR(H5E_OHDR, H5E_CANTINIT, NULL, "unable to obtain local timezone information");

	the_time -= tz.tz_minuteswest * 60 - (tm.tm_isdst ? 3600 : 0);
    }
#elif defined (WIN32) 
  #if !defined (__MWERKS__) /* MSVC */
    {
     struct timeb timebuffer;
     long  tz;
     
     ftime(&timebuffer);
     tz = timebuffer.timezone;
     /* daylight is not handled properly. Currently we just hard-code
     the problem. */
     the_time -= tz * 60 - 3600;
    }
  #else  /*__MWERKS__*/

    ;

  #endif /*__MWERKS__*/
#else /* WIN32 */
    /*
     * The catch-all.  If we can't convert a character string universal
     * coordinated time to a time_t value reliably then we can't decode the
     * modification time message. This really isn't as bad as it sounds -- the
     * only way a user can get the modification time is from H5Gget_objinfo()
     * and H5G_get_objinfo() can gracefully recover.
     */

    /* Irix64 */
    HGOTO_ERROR(H5E_OHDR, H5E_CANTINIT, NULL, "unable to obtain local timezone information");
#endif
    
    /* The return value */
    if (NULL==(mesg = H5FL_MALLOC(time_t)))
	HGOTO_ERROR (H5E_RESOURCE, H5E_NOSPACE, NULL, "memory allocation failed");
    *mesg = the_time;

    /* Set return value */
    ret_value=mesg;

done:
    FUNC_LEAVE_NOAPI(ret_value);
}


/*-------------------------------------------------------------------------
 * Function:	H5O_mtime_new_encode
 *
 * Purpose:	Encodes a new modification time message.
 *
 * Return:	Non-negative on success/Negative on failure
 *
 * Programmer:	Quincey Koziol
 *		koziol@ncsa.uiuc.edu
 *		Jan  3 2002
 *
 * Modifications:
 *
 *-------------------------------------------------------------------------
 */
static herr_t
H5O_mtime_new_encode(H5F_t UNUSED *f, uint8_t *p, const void *_mesg)
{
    const time_t	*mesg = (const time_t *) _mesg;
    herr_t ret_value=SUCCEED;   /* Return value */
    
    FUNC_ENTER_NOAPI(H5O_mtime_new_encode, FAIL);

    /* check args */
    assert(f);
    assert(p);
    assert(mesg);

    /* Version */
    *p++ = H5O_MTIME_VERSION;

    /* Reserved bytes */
    *p++ = 0;
    *p++ = 0;
    *p++ = 0;

    /* Encode time */
    UINT32ENCODE(p, *mesg);

done:
    FUNC_LEAVE_NOAPI(ret_value);
} /* end H5O_mtime_new_encode() */


/*-------------------------------------------------------------------------
 * Function:	H5O_mtime_encode
 *
 * Purpose:	Encodes a modification time message.
 *
 * Return:	Non-negative on success/Negative on failure
 *
 * Programmer:	Robb Matzke
 *		matzke@llnl.gov
 *		Jul 24 1998
 *
 * Modifications:
 *
 *-------------------------------------------------------------------------
 */
static herr_t
H5O_mtime_encode(H5F_t UNUSED *f, uint8_t *p, const void *_mesg)
{
    const time_t	*mesg = (const time_t *) _mesg;
    struct tm		*tm;
    herr_t ret_value=SUCCEED;   /* Return value */
    
    FUNC_ENTER_NOAPI(H5O_mtime_encode, FAIL);

    /* check args */
    assert(f);
    assert(p);
    assert(mesg);

    /* encode */
    tm = HDgmtime(mesg);
    sprintf((char*)p, "%04d%02d%02d%02d%02d%02d",
	    1900+tm->tm_year, 1+tm->tm_mon, tm->tm_mday,
	    tm->tm_hour, tm->tm_min, tm->tm_sec);

done:
    FUNC_LEAVE_NOAPI(ret_value);
}


/*-------------------------------------------------------------------------
 * Function:	H5O_mtime_copy
 *
 * Purpose:	Copies a message from _MESG to _DEST, allocating _DEST if
 *		necessary.
 *
 * Return:	Success:	Ptr to _DEST
 *
 *		Failure:	NULL
 *
 * Programmer:	Robb Matzke
 *		matzke@llnl.gov
 *		Jul 24 1998
 *
 * Modifications:
 *
 *-------------------------------------------------------------------------
 */
static void *
H5O_mtime_copy(const void *_mesg, void *_dest)
{
    const time_t	*mesg = (const time_t *) _mesg;
    time_t		*dest = (time_t *) _dest;
    void        *ret_value;     /* Return value */

    FUNC_ENTER_NOAPI(H5O_mtime_copy, NULL);

    /* check args */
    assert(mesg);
    if (!dest && NULL==(dest = H5FL_MALLOC(time_t)))
        HGOTO_ERROR (H5E_RESOURCE, H5E_NOSPACE, NULL, "memory allocation failed");
    
    /* copy */
    *dest = *mesg;

    /* Set return value */
    ret_value=dest;

done:
    FUNC_LEAVE_NOAPI(ret_value);
}


/*-------------------------------------------------------------------------
 * Function:	H5O_mtime_new_size
 *
 * Purpose:	Returns the size of the raw message in bytes not
 *		counting the message type or size fields, but only the data
 *		fields.	 This function doesn't take into account
 *		alignment.
 *
 * Return:	Success:	Message data size in bytes w/o alignment.
 *
 *		Failure:	0
 *
 * Programmer:	Quincey Koziol
 *		koziol@ncsa.uiuc.edu
 *		Jan  3 2002
 *
 * Modifications:
 *
 *-------------------------------------------------------------------------
 */
static size_t
H5O_mtime_new_size(H5F_t UNUSED * f, const void UNUSED * mesg)
{
    size_t ret_value=8;        /* Return value */

    FUNC_ENTER_NOAPI(H5O_mtime_new_size, 0);

    /* check args */
    assert(f);
    assert(mesg);

done:
    FUNC_LEAVE_NOAPI(ret_value);
} /* end H5O_mtime_new_size() */


/*-------------------------------------------------------------------------
 * Function:	H5O_mtime_size
 *
 * Purpose:	Returns the size of the raw message in bytes not
 *		counting the message type or size fields, but only the data
 *		fields.	 This function doesn't take into account
 *		alignment.
 *
 * Return:	Success:	Message data size in bytes w/o alignment.
 *
 *		Failure:	0
 *
 * Programmer:	Robb Matzke
 *		matzke@llnl.gov
 *		Jul 14 1998
 *
 * Modifications:
 *
 *-------------------------------------------------------------------------
 */
static size_t
H5O_mtime_size(H5F_t UNUSED * f, const void UNUSED * mesg)
{
    size_t ret_value=16;        /* Return value */

    FUNC_ENTER_NOAPI(H5O_mtime_size, 0);

    /* check args */
    assert(f);
    assert(mesg);

done:
    FUNC_LEAVE_NOAPI(ret_value);
}


/*-------------------------------------------------------------------------
 * Function:	H5O_mtime_reset
 *
 * Purpose:	Frees resources within a modification time message, but doesn't free
 *		the message itself.
 *
 * Return:	Non-negative on success/Negative on failure
 *
 * Programmer:	Quincey Koziol
 *		Mondey, December 23, 2002
 *
 * Modifications:
 *
 *-------------------------------------------------------------------------
 */
static herr_t
H5O_mtime_reset(void UNUSED *_mesg)
{
    herr_t      ret_value=SUCCEED;       /* Return value */

    FUNC_ENTER_NOAPI(H5O_mtime_reset, FAIL);

done:
    FUNC_LEAVE_NOAPI(ret_value);
}


/*-------------------------------------------------------------------------
 * Function:	H5O_mtime_free
 *
 * Purpose:	Free's the message
 *
 * Return:	Non-negative on success/Negative on failure
 *
 * Programmer:	Quincey Koziol
 *              Thursday, March 30, 2000
 *
 * Modifications:
 *
 *-------------------------------------------------------------------------
 */
static herr_t
H5O_mtime_free (void *mesg)
{
    herr_t ret_value=SUCCEED;   /* Return value */

    FUNC_ENTER_NOAPI(H5O_mtime_free, FAIL);

    assert (mesg);

    H5FL_FREE(time_t,mesg);

done:
    FUNC_LEAVE_NOAPI(ret_value);
}


/*-------------------------------------------------------------------------
 * Function:	H5O_mtime_debug
 *
 * Purpose:	Prints debugging info for the message.
 *
 * Return:	Non-negative on success/Negative on failure
 *
 * Programmer:	Robb Matzke
 *		matzke@llnl.gov
 *		Jul 24 1998
 *
 * Modifications:
 *
 *-------------------------------------------------------------------------
 */
static herr_t
H5O_mtime_debug(H5F_t UNUSED *f, hid_t UNUSED dxpl_id, const void *_mesg, FILE *stream,
		int indent, int fwidth)
{
    const time_t	*mesg = (const time_t *)_mesg;
    struct tm		*tm;
    char		buf[128];
    herr_t ret_value=SUCCEED;   /* Return value */
    
    FUNC_ENTER_NOAPI(H5O_mtime_debug, FAIL);

    /* check args */
    assert(f);
    assert(mesg);
    assert(stream);
    assert(indent >= 0);
    assert(fwidth >= 0);

    /* debug */
    tm = HDlocaltime(mesg);
    
    HDstrftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S %Z", tm);
    fprintf(stream, "%*s%-*s %s\n", indent, "", fwidth,
	    "Time:", buf);

done:
    FUNC_LEAVE_NOAPI(ret_value);
}
