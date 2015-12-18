/*******************************
 * file name:   jvm.h
 * description: define jvm 
 * date:        2015-01-19
 * author:      kari.zhang
 *
 * modifications:
 *
 * ****************************/

#ifndef __JVM__H__
#define __JVM__H__

#include "class.h"
#include "comm.h"
#include "mem.h"

#define MIN_HEAP 4*KB
#define MIN_STACK 1*KB

#define DEFAULT_MIN_HEAP 16*MB
#define DEFAULT_MAX_HEAP 128*MB
#define DEFAULT_STACK 256*KB

/*
 * size of emergency area - big enough to create
 * a StackOverFlow exception 
 */
#define STACK_RED_ZONE_SIZE 1*KB

#define JAVA_VERSION "0.0.1" 

/*
 * Store system property
 */
typedef struct Property {
	char *key;
	char *value;
} Property;

/**
 * Initialization arguments
 */
typedef struct InitArgs {
	// ExecEnv not include me any more
	// GC mark me as the root node in the future
	Class *mainClass;           // class has static void main(String[])
	char *bootpath;             // java.lang.*
	U4 java_stack;              // java stack size that user specified
	U4 min_heap;                // min heap size, generation area size 
	U4 max_heap;                // max heap size, total starage size
	Property *cmdline_props;    // system properties
	int props_count;            // properties count

	// System.out
	int (*vfprintf)(FILE *stream, const char *fmt, va_list va);

	// System.in
	int (*vfscanf)(FILE *stream, const char *fmt, va_list va);

	void (*exit)(int status);   // System.exit()
	void (*abort)(void);        // System.abort()
} InitArgs;

/**
 * Java stack
 */
typedef struct JavaStack {
	int top;
	StackFrame **frames;
} JavaStack;

/*
 * Java virtual machine executing environment in runtime
 */
typedef struct ExecEnv {
	MemoryArea *heapArea;		// java heap memory area
	MemoryArea *stackArea;		// java stack memory area
    JavaStack  *javaStack;		// JavaStack to store stack frames
	U2 userClsCnt;				// user class count
	Class **userClsArea;	    // user class list exclude entry main()
	U2 rtClsCnt;				// runtime class count
	Class **rtClsArea;		    // runtime class address list
	MethodEntry *mainMethod;	// user class main()
} ExecEnv;

typedef struct VM {
   	InitArgs *initArgs;			// initialization arguments
    ExecEnv *execEnv;			// executing environment
} VM;

/**
 * Set the initial arguments to java virtual machine
 */
extern void setDefaultInitArgs(InitArgs *args);
extern int parseCmdLine(int argc, char **argv, Property **props);
extern int readSysConfig(char *path, Property *props);
extern int setInitArgs(Property *props, int nprop, InitArgs *args);

/**
 * Manage java virtual machine
 */
extern void initVM(InitArgs *args, VM *vm);
extern void startVM(VM *vm);
extern void destroyVM(VM *vm);

/*
 * Push stack frame into stack
 */
extern bool pushStack(JavaStack *stack, StackFrame *frame);
extern StackFrame* popStack(JavaStack *stack);
extern bool isStackEmpty(JavaStack *stack);

#endif
