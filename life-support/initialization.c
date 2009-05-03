/* -*- Mode: C; Tab-Width: 4 -*- */

/* VLM Life Support initialization */

#include "std.h"
#include <sys/utsname.h>
#include <ctype.h>

#include "life_types.h"
#include "embed.h"
#include "BootComm.h"
#include "FEPComm.h"
#include "SystemComm.h"
#include "VLM_configuration.h"
#include "life_prototypes.h"
#include "utilities.h"

#include "aihead.h"
#include "aistat.h"
#include "ivoryrep.h"
#include "memory.h"


/* Version of the VLM (nee, genera program):  Eventually, this information will be in
   a header file that's automatically generated by :Assemble Emulator */

/* 7.7 fixes to array reference trap code, array register recomputation */
/* 7.8 fixes EGC bug where emulator bit wasn't on for copyspace sometimes */
/* 7.9 fixes false-oldspace looping bug */
/* 8.0 fixes a hang because of disagreement between two stop-request flags */
/* 8.1 fixes xmodmap problems */
/* 8.2 fixes defaulting to Genera-8-4.vlod as default world */
/* 8.3 another try at RGETF hang fix */
/* 8.4 make cold load characters match overlay */
/* 8.5 make window background white by default */
/* 8.6 fixes deep window playback of X initialization */
/* 8.7 incorporates other changes */
/* 8.8 another try at RGET hang */
/* 8.9 floating point bug (macro only change) */
/* 8.10 (rescinded) only do setup_modifier_mapping once per display */
/* 8.11 fix bug with long host names */
/* 8.12 kludge fix for thread hang (wait with timeout) */
/* 8.13 floor/round/truncate/ceiling double-float cons trap bug */
/* 8.14 keep mprotect error code around */
/* 8.15 put in a couple of pthread_joins after pthread_cancels */
/* 8.16 initialize the tracep flag to 0 if TRACING not defined */
/* 8.17 fix LSH of > 32 bits, add tag checking for %unwind-to-frame-and-restart-or-apply */
/* 8.18 fix bugs in 8.17 */
/* 9.0 MacIvory G5 */

#define GeneraMajorVersion 9
#define GeneraMinorVersion 0


/*** Global Data ***/

BootCommArea *BootCommAreaPtr = NULL;
BootDataArea *BootDataAreaPtr = NULL;
FEPCommArea *FEPCommAreaPtr = NULL;
SystemCommArea *SystemCommAreaPtr = NULL;
EmbCommArea *EmbCommAreaPtr = NULL;

EmbPtr EmbCommAreaAllocPtr = NullEmbPtr;
size_t EmbCommAreaAllocSize = 0;

caddr_t	dataAddress = NULL;
caddr_t tagsAddress = NULL;
size_t areasSize = 0;

static pthread_key_t mainThread;


/* Allocate a piece of the embedded communications area */

EmbPtr EmbCommAreaAlloc (size_t nBytes)
{
  size_t nWords = (nBytes + sizeof (EmbWord) - 1) / sizeof (EmbWord);
  EmbPtr thePtr = EmbCommAreaAllocPtr;

#if LONG_BIT == 64
	if (nWords & 1) nWords++;					/* Must ensure quadword alignment */
#endif

	if ((nWords > EmbCommAreaAllocSize) || (nBytes <= 0))
		vpunt (NULL, "Couldn't allocate %d words in the embedded communications area", nWords);

	EmbCommAreaAllocSize -= nWords;
	EmbCommAreaAllocPtr += nWords;

	return (thePtr);
}


/* Store a string in the embedded communications area */

EmbPtr MakeEmbString (char* aString)
{
  EmbPtr theStringPtr;
  register EmbString* theString;
  register size_t nBytes = (NULL == aString) ? 0 : strlen (aString);
  uint32_t datum;

	if (0 == nBytes) return (NullEmbPtr);

	theStringPtr = EmbCommAreaAlloc (sizeof (EmbString) + nBytes);
	theString = (EmbString*) HostPointer (theStringPtr);
	theString->length = nBytes;
	memcpy ((char*)&theString->string, aString, nBytes);

#if BYTE_ORDER == BIG_ENDIAN
	bswap32_block (&theString->string, nBytes);
#endif

	return (theStringPtr);
}


/* Parses a version number into major and minor version numbers */

static void ParseVersionNumber (char* versionString, int* majorVersion, int* minorVersion)
{
  char *start, *end;
  int major, minor = -1;

	*majorVersion = *minorVersion = -1;

	start = versionString;
	major = strtoul (start, &end, 10);
	if (start == end)
		return;

	if (*end)
		if (*end == '.')
		  {
			start = end + 1;
			minor = strtoul (start, &end, 0);
			if ((start == end) || *end)
				return;
		  }
		else
			return;

	*majorVersion = major;
	*minorVersion = minor;
}


/* Guts of Life Support initialization */

void InitializeLifeSupport (VLMConfig* config)
{
  struct utsname osfName;
  char worldPathname[_POSIX_PATH_MAX+5+1], *loginName, *identifier;
  int major, minor;

	/* Ask the emulator to establish the BootComm/BootData/CommArea mapping */

	EnsureVirtualAddressRange (BootCommAreaAddress, 
							   (BootCommAreaSize + BootDataAreaSize + config->commAreaSize),
							   FALSE);
	BootCommAreaPtr = (BootCommArea*) MapVirtualAddressData (BootCommAreaAddress);
	BootDataAreaPtr = (BootDataArea*) MapVirtualAddressData (BootDataAreaAddress);
	EmbCommAreaPtr = (EmbCommArea*) MapVirtualAddressData (EmbCommAreaAddress);


	/* Initialize the BootComm and BootData */

	VirtualMemoryWriteBlockConstant (BootCommAreaAddress, 
									 MakeLispObj (Type_Null, BootCommAreaAddress),
									 (BootCommAreaSize + BootDataAreaSize), 1);

	WriteBootCommSlot (embCommArea, EmbCommAreaAddress, Type_Locative);
	WriteBootCommSlot (systemType, SystemTypeVLM, Type_Fixnum);
	WriteBootCommSlot (stackBase, BootStackBase, Type_Locative);
	WriteBootCommSlot (stackSize, BootStackSize, Type_Fixnum);
	WriteBootCommSlot (spyBlockAddress, BootDataAreaAddress, Type_Locative);
	WriteBootCommSlot (spyCommandAddress, BootDataSlotAddress (bootSpyCommand), Type_Locative);
	WriteBootCommSlot (spyStatusAddress, BootDataSlotAddress (bootSpyStatus), Type_Locative);


	/* Ask the emulator to establish the FEPComm area mapping and initialize the area */

	EnsureVirtualAddressRange (FEPCommAreaAddress, FEPCommAreaSize, FALSE);
	VirtualMemoryWriteBlockConstant (FEPCommAreaAddress, 
									 MakeLispObj (Type_Null, FEPCommAreaAddress),
									 FEPCommAreaSize, 1);
	FEPCommAreaPtr = (FEPCommArea*) MapVirtualAddressData (FEPCommAreaAddress);


	/* Ask the emulator to establish the SystemComm area mapping and initialize the area */

	EnsureVirtualAddressRange (SystemCommAreaAddress, SystemCommAreaSize, FALSE);
	VirtualMemoryWriteBlockConstant (SystemCommAreaAddress, 
									 MakeLispObj (Type_Null, SystemCommAreaAddress),
									 SystemCommAreaSize, 1);
	SystemCommAreaPtr = (SystemCommArea*) MapVirtualAddressData (SystemCommAreaAddress);


	/* Initialize the communications area */

	VirtualMemoryWriteBlockConstant (EmbCommAreaAddress, MakeLispObj (Type_Fixnum, 0),
									 config->commAreaSize, 0);
	
#if BYTE_ORDER == LITTLE_ENDIAN
	identifier = "EMBD";
#else
	identifier = "DBME";
#endif
	EmbCommAreaPtr->identifier = *(EmbWord*)identifier;

	EmbCommAreaPtr->version = 1;
	EmbCommAreaPtr->system_type = SystemTypeVLM;

	EmbCommAreaPtr->number_of_slots = ((ptrdiff_t)&EmbCommAreaPtr->pad0 - (ptrdiff_t)EmbCommAreaPtr) / sizeof (EmbWord);
	EmbCommAreaPtr->comm_memory_size = config->commAreaSize;

	EmbCommAreaPtr->generaVersion.major = GeneraMajorVersion;
	EmbCommAreaPtr->generaVersion.minor = GeneraMinorVersion;

	if (uname (&osfName) < 0)
		EmbCommAreaPtr->osfVersion.majorRelease = 0;	/* Couldn't determine the version */
	else
	  {
		EmbCommAreaPtr->osfVersion.testReleaseP = 0;
		if (isdigit (osfName.release[0]))
			ParseVersionNumber (osfName.release, &major, &minor);
		else
		  {
			EmbCommAreaPtr->osfVersion.testReleaseP = (osfName.release[0] != 'V');
			ParseVersionNumber (&osfName.release[1], &major, &minor);
		  }
		EmbCommAreaPtr->osfVersion.majorRelease = major;
		EmbCommAreaPtr->osfVersion.minorRelease = minor;
		ParseVersionNumber (osfName.version, &major, &minor);
		EmbCommAreaPtr->osfVersion.majorRevision = major;
		EmbCommAreaPtr->osfVersion.minorRevision = minor;
	  }

	EmbCommAreaPtr->channel_table = NullEmbPtr;
	EmbCommAreaPtr->consoleChannel = NullEmbPtr;
	EmbCommAreaPtr->cold_load_channel = NullEmbPtr;
	EmbCommAreaPtr->command_channel = NullEmbPtr;

	EmbCommAreaPtr->clock_signal = -1;			/* No signal allocated until guest needs it */

	EmbCommAreaPtr->slaveTrigger = NULL;		/* Will be the address of a global ... */

	InitializeSignalHandlers ();

	if (pthread_key_create (&mainThread, NULL))
		vpunt (NULL, "Unable to establish per-thread data.");

	pthread_setspecific (mainThread, (void*) TRUE);

	if (atexit (&TerminateLifeSupport))
		vpunt (NULL, "Unable to establish cleanup handler for Life Support");


	/* Life Support uses threads to implement handlers for signals from the VLM --
	   Each handler will run in its on thread and, effectively, is responsible for
	   a single unidirectional channel (e.g., a disk channel) or one half of a
	   bidirectional channel (e.g., network transmitter, network receiver).
	   Another thread is created to periodically invoke all signal handlers to avoid
	   lost "interrupts".  A single mutex (lock) and conditional variable (signal) is 
	   used to synchronize these threads.  (The mutex is locked until initialization
	   is completed to prevent the threads from running prematurely.) */

	SetupThreadAttrs ("polling", 0, &EmbCommAreaPtr->pollThreadAttrs, 
					  &EmbCommAreaPtr->pollThreadAttrsSetup);

	SetupThreadAttrs ("output", 2, &EmbCommAreaPtr->outputThreadAttrs,
					  &EmbCommAreaPtr->outputThreadAttrsSetup);

	SetupThreadAttrs ("input", 3, &EmbCommAreaPtr->inputThreadAttrs,
					  &EmbCommAreaPtr->inputThreadAttrsSetup);

	if (pthread_mutex_init (&EmbCommAreaPtr->signalLock, NULL))
		vpunt (NULL, "Unable to create the Life Support signal lock");
	EmbCommAreaPtr->signalLockSetup = TRUE;

	if (pthread_cond_init (&EmbCommAreaPtr->signalSignal, NULL))
		vpunt (NULL, "Unable to create the Life Support signal signal");
	EmbCommAreaPtr->signalSignalSetup = TRUE;

	if (pthread_mutex_lock (&EmbCommAreaPtr->signalLock))
		vpunt (NULL, "Unable to lock the Life Support signal lock in thread %lx",
			   pthread_self ());

	if (pthread_create (&EmbCommAreaPtr->pollingThread, &EmbCommAreaPtr->pollThreadAttrs,
						 (pthread_startroutine_t)&IvoryLifePolling, NULL))
		vpunt (NULL, "Unable to create the Life Support polling thread");
	EmbCommAreaPtr->pollingThreadSetup = TRUE;

	if (pthread_mutex_init (&EmbCommAreaPtr->clockLock, NULL))
		vpunt (NULL, "Unable to create the Life Support clock lock");
	EmbCommAreaPtr->clockLockSetup = TRUE;

	if (pthread_cond_init (&EmbCommAreaPtr->clockSignal, NULL))
		vpunt (NULL, "Unable to create the Life Support clock signal");
	EmbCommAreaPtr->clockSignalSetup = TRUE;

	if (pthread_create (&EmbCommAreaPtr->clockThread, &EmbCommAreaPtr->pollThreadAttrs,
						 (pthread_startroutine_t)&IntervalTimerDriver, NULL))
		vpunt (NULL, "Unable to create the Life Support interval timer thread");
	EmbCommAreaPtr->clockThreadSetup = TRUE;

	if (pthread_mutex_init (&EmbCommAreaPtr->XLock, NULL))
		vpunt (NULL, "Unable to create the Life Support X library lock");
	EmbCommAreaPtr->XLockSetup = TRUE;

	if (pthread_mutex_init (&EmbCommAreaPtr->wakeupLock, NULL))
		vpunt (NULL, "Unable to create the VLM wakeup lock");
	EmbCommAreaPtr->wakeupLockSetup = TRUE;

	if (pthread_cond_init (&EmbCommAreaPtr->wakeupSignal, NULL))
		vpunt (NULL, "Unable to create the VLM wakeup signal");
	EmbCommAreaPtr->wakeupSignalSetup = TRUE;


	/* Create the channels, their data structures, and threads */

	EmbCommAreaAllocPtr = sizeof (EmbCommArea) / sizeof (EmbWord);
	EmbCommAreaAllocSize = EmbCommAreaPtr->comm_memory_size - EmbCommAreaAllocPtr;

	if (config->worldPath[0])
		sprintf (worldPathname, "HOST:%s", config->worldPath);
	else
		worldPathname[0] = 0;
	EmbCommAreaPtr->worldPathname = MakeEmbString (worldPathname);

	loginName = getlogin ();
	if (loginName != NULL)
		EmbCommAreaPtr->unixLoginName = MakeEmbString (loginName);
	else
		EmbCommAreaPtr->unixLoginName = NullEmbPtr;
	EmbCommAreaPtr->unixUID = getuid ();
	EmbCommAreaPtr->unixGID = getgid ();

#ifndef MINIMA
#ifndef IVERIFY
	InitializeColdLoadChannel (config);
	InitializeConsoleChannel (config);
#endif
#endif
	InitializeMessageChannels (config);
	InitializeNetworkChannels (config);

	/* Host File channel */
	/* RPC channel? */


	/* Setup host, FEP, and guest buffers */

	EmbCommAreaPtr->host_buffer_start = EmbCommAreaAllocPtr;
	EmbCommAreaPtr->host_buffer_size = config->hostBufferSpace;
	/* Initialize the host buffers -- HOW? */

	EmbCommAreaPtr->fep_buffer_start = EmbCommAreaAllocPtr + EmbCommAreaPtr->host_buffer_size;
	EmbCommAreaPtr->fep_buffer_size = 512;		/* Enough for a single buffer plus overhead */

	EmbCommAreaPtr->guest_buffer_start = EmbCommAreaAllocPtr + EmbCommAreaPtr->host_buffer_size
															 + EmbCommAreaPtr->fep_buffer_size;
	EmbCommAreaPtr->guest_buffer_size = EmbCommAreaAllocSize - EmbCommAreaPtr->host_buffer_size
															 - EmbCommAreaPtr->fep_buffer_size;
	if (EmbCommAreaPtr->guest_buffer_size < config->guestBufferSpace)
		vpunt (NULL,
			   "Couldn't allocate %d words for guest buffers in the communcations area; only %d words are available.",
			   config->guestBufferSpace, EmbCommAreaPtr->guest_buffer_size);


	/* Release the signal lock to let Life Support threads run */

	EmbCommAreaPtr->useSignalLocks = TRUE;

	if (pthread_mutex_unlock (&EmbCommAreaPtr->signalLock))
		vpunt (NULL, "Unable to unlock the Life Support signal lock in thread %lx",
			   pthread_self ());
}


/* Cleanup Life Support on exit -- Kill existing threads, close disk channels, etc. */

void TerminateLifeSupport ()
{
  struct timespec killSleep;
  void* exit_code;

	if (NULL == pthread_getspecific (mainThread))
		return;

	TerminateSignalHandlers ();

#ifndef MINIMA
#ifndef IVERIFY
	TerminateColdLoadChannel ();
	TerminateConsoleChannel ();
#endif
#endif
	TerminateDiskChannels ();
	TerminateMessageChannels ();
	TerminateNetworkChannels ();

	killSleep.tv_sec = 1;
	killSleep.tv_nsec = 250000000;
	pthread_delay_np (&killSleep);

	if (EmbCommAreaPtr->wakeupSignalSetup)
	  {
		pthread_cond_destroy (&EmbCommAreaPtr->wakeupSignal);
		EmbCommAreaPtr->wakeupSignalSetup = FALSE;
	  }

	if (EmbCommAreaPtr->wakeupLockSetup)
	  {
		pthread_mutex_destroy (&EmbCommAreaPtr->wakeupLock);
		EmbCommAreaPtr->wakeupLockSetup = FALSE;
	  }

	if (EmbCommAreaPtr->XLockSetup)
	  {
		pthread_mutex_destroy (&EmbCommAreaPtr->XLock);
		EmbCommAreaPtr->XLockSetup = FALSE;
	  }

	if (EmbCommAreaPtr->clockThreadSetup)
	  {
		pthread_cancel (EmbCommAreaPtr->clockThread);
		pthread_join (EmbCommAreaPtr->clockThread, &exit_code);
		EmbCommAreaPtr->clockThreadSetup = FALSE;
	  }

	if (EmbCommAreaPtr->clockSignalSetup)
	  {
		pthread_cond_destroy (&EmbCommAreaPtr->clockSignal);
		EmbCommAreaPtr->clockSignalSetup = FALSE;
	  }

	if (EmbCommAreaPtr->clockLockSetup)
	  {
		pthread_mutex_destroy (&EmbCommAreaPtr->clockLock);
		EmbCommAreaPtr->clockLockSetup = FALSE;
	  }

	if (EmbCommAreaPtr->pollingThreadSetup)
	  {
		pthread_cancel (EmbCommAreaPtr->pollingThread);
		pthread_join (EmbCommAreaPtr->pollingThread, &exit_code);
		EmbCommAreaPtr->pollingThreadSetup = FALSE;
	  }

	if (EmbCommAreaPtr->signalSignalSetup)
	  {
		pthread_cond_destroy (&EmbCommAreaPtr->signalSignal);
		EmbCommAreaPtr->signalSignalSetup = FALSE;
	  }

	if (EmbCommAreaPtr->signalLockSetup)
	  {
		pthread_mutex_destroy (&EmbCommAreaPtr->signalLock);
		EmbCommAreaPtr->signalLockSetup = FALSE;
	  }

	if (EmbCommAreaPtr->inputThreadAttrsSetup)
	  {
		pthread_attr_destroy (&EmbCommAreaPtr->inputThreadAttrs);
		EmbCommAreaPtr->inputThreadAttrsSetup = FALSE;
	  }

	if (EmbCommAreaPtr->outputThreadAttrsSetup)
	  {
		pthread_attr_destroy (&EmbCommAreaPtr->outputThreadAttrs);
		EmbCommAreaPtr->outputThreadAttrsSetup = FALSE;
	  }

	if (EmbCommAreaPtr->pollThreadAttrsSetup)
	  {
		pthread_attr_destroy (&EmbCommAreaPtr->pollThreadAttrs);
		EmbCommAreaPtr->pollThreadAttrsSetup = FALSE;
	  }
}


/* Setup the attributes for a class of threads */

static void SetupThreadAttrs (char* class, int priorityBoost, pthread_attr_t* threadAttrs,
							  bool* threadAttrsSetup)
{
  size_t stackSize;
  int priority;

	if (pthread_attr_init (threadAttrs))
		vpunt (NULL, "Unable to create attributes for Life Support %s threads", class);
	*threadAttrsSetup = TRUE;

	pthread_attr_getstacksize (threadAttrs, &stackSize);
	if (pthread_attr_setstacksize (threadAttrs, (4 * stackSize)))
		vpunt (NULL, 
			   "Unable to set stack size attribute for Life Support %s threads to %d bytes",
			   class, (4 * stackSize));

	/* Can't change the priority of a regular thread in Linux and Mac OS X */

#if 0
#ifdef OS_OSF
	priority = pthread_attr_getprio (*threadAttrs);
	if (pthread_attr_setprio (threadAttrs, (priority + priorityBoost)))
		vpunt (NULL, "Unable to set priority attribute for Life Support %s threads to %d",
			   class, (priority + priorityBoost));
#endif
#endif
}
