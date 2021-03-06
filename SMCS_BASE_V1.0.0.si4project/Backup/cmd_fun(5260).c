/*
 * FreeRTOS Kernel V10.2.1
 * Copyright (C) 2017 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */


 /******************************************************************************
 *
 * http://www.FreeRTOS.org/cli
 *
 ******************************************************************************/
#include "cmd_fun.h"
#include "easyflash.h"


/* CLI Command Structure define */
static const CLI_Command_Definition_t CLI_Definition_All[] = {
  {
      "tasklist", /* The command string to type. */
      "\r\ntasklist:\r\n Displays a table showing the state of each FreeRTOS task\r\n",
      prvTaskStatsCommand, /* The function to run. */
      0 /* No parameters are expected. */

  },
  {
      "echo-3-parameters",
      "\r\necho <param1> <param2> <param3>:\r\n Expects three parameters, echos each in turn\r\n",
      prvThreeParameterEchoCommand, /* The function to run. */
      3 /* Three parameters are expected, which can take any value. */

  },
  {
      "echo-parameters",
      "\r\necho-parameters <...>:\r\n Take variable number of parameters, echos each in turn\r\n",
      prvParameterEchoCommand, /* The function to run. */
      -1 /* The user can enter any number of commands. */

  },
#if( configGENERATE_RUN_TIME_STATS == 1 )
      {
          "run-time-stats", /* The command string to type. */
          "\r\nrun-time-stats:\r\n Displays a table showing how much processing time each FreeRTOS task has used\r\n",
          prvRunTimeStatsCommand, /* The function to run. */
          0 /* No parameters are expected. */
      },
#endif /* configGENERATE_RUN_TIME_STATS */

#if( configINCLUDE_QUERY_HEAP_COMMAND == 1 )
	{
		"query-heap",
		"\r\nquery-heap:\r\n Displays the free heap space, and minimum ever free heap space.\r\n",
		prvQueryHeapCommand, /* The function to run. */
		0 /* The user can enter any number of commands. */
	},
#endif /* configQUERY_HEAP_COMMAND */

#if configINCLUDE_TRACE_RELATED_CLI_COMMANDS == 1
	{
		"trace",
		"\r\ntrace [start | stop]:\r\n Starts or stops a trace recording for viewing in FreeRTOS+Trace\r\n",
		prvStartStopTraceCommand, /* The function to run. */
		1 /* One parameter is expected.  Valid values are "start" and "stop". */
	},
#endif /* configINCLUDE_TRACE_RELATED_CLI_COMMANDS */

    {
        "printenv",
        "\r\nprintenv:\r\n Display all parameters\r\n\r\n",
        prvPrintEnv,
        0
    },
    
    {
        "setenv",
        "\r\nsetenv:\r\n set the value of key \r\n\r\n",
        prvSetEnv,
        2
    },

    {
        "getenv",
        "\r\ngetenv:\r\n set the value of key \r\n\r\n",
        prvGetEnv,
        1
    },
        

};



/* Structure that defines the "task-stats" command line command.  This generates
//a table that gives information on each task in the system. */
//static const CLI_Command_Definition_t xTaskStats =
//{
//	"tasklist", /* The command string to type. */
//	"\r\ntasklist:\r\n Displays a table showing the state of each FreeRTOS task\r\n",
//	prvTaskStatsCommand, /* The function to run. */
//	0 /* No parameters are expected. */
//};

/* Structure that defines the "echo_3_parameters" command line command.  This
//takes exactly three parameters that the command simply echos back one at a
//time. */
//static const CLI_Command_Definition_t xThreeParameterEcho =
//{
//	"echo-3-parameters",
//	"\r\necho <param1> <param2> <param3>:\r\n Expects three parameters, echos each in turn\r\n",
//	prvThreeParameterEchoCommand, /* The function to run. */
//	3 /* Three parameters are expected, which can take any value. */
//};

/* Structure that defines the "echo_parameters" command line command.  This
//takes a variable number of parameters that the command simply echos back one at
//a time. */
//static const CLI_Command_Definition_t xParameterEcho =
//{
//	"echo-parameters",
//	"\r\necho-parameters <...>:\r\n Take variable number of parameters, echos each in turn\r\n",
//	prvParameterEchoCommand, /* The function to run. */
//	-1 /* The user can enter any number of commands. */
//};

//#if( configGENERATE_RUN_TIME_STATS == 1 )
//	/* Structure that defines the "run-time-stats" command line command.   This
//	generates a table that shows how much run time each task has */
//	static const CLI_Command_Definition_t xRunTimeStats =
//	{
//		"run-time-stats", /* The command string to type. */
//		"\r\nrun-time-stats:\r\n Displays a table showing how much processing time each FreeRTOS task has used\r\n",
//		prvRunTimeStatsCommand, /* The function to run. */
//		0 /* No parameters are expected. */
//	};
//#endif /* configGENERATE_RUN_TIME_STATS */

//#if( configINCLUDE_QUERY_HEAP_COMMAND == 1 )
//	/* Structure that defines the "query_heap" command line command. */
//	static const CLI_Command_Definition_t xQueryHeap =
//	{
//		"query-heap",
//		"\r\nquery-heap:\r\n Displays the free heap space, and minimum ever free heap space.\r\n",
//		prvQueryHeapCommand, /* The function to run. */
//		0 /* The user can enter any number of commands. */
//	};
//#endif /* configQUERY_HEAP_COMMAND */

//#if configINCLUDE_TRACE_RELATED_CLI_COMMANDS == 1
//	/* Structure that defines the "trace" command line command.  This takes a single
//	parameter, which can be either "start" or "stop". */
//	static const CLI_Command_Definition_t xStartStopTrace =
//	{
//		"trace",
//		"\r\ntrace [start | stop]:\r\n Starts or stops a trace recording for viewing in FreeRTOS+Trace\r\n",
//		prvStartStopTraceCommand, /* The function to run. */
//		1 /* One parameter is expected.  Valid values are "start" and "stop". */
//	};
//#endif /* configINCLUDE_TRACE_RELATED_CLI_COMMANDS */

/*-----------------------------------------------------------*/

void CMD_Init(void) {
  size_t cnt = sizeof(CLI_Definition_All) / sizeof(CLI_Command_Definition_t);
  size_t i;
  
  for (i = 0; i < cnt; i++) 
  {
    FreeRTOS_CLIRegisterCommand(&CLI_Definition_All[i]);
  }
}



//void vRegisterSampleCLICommands( void )
//{
//	/* Register all the command line commands defined immediately above. */
//	FreeRTOS_CLIRegisterCommand( &xTaskStats );	
//	FreeRTOS_CLIRegisterCommand( &xThreeParameterEcho );
//	FreeRTOS_CLIRegisterCommand( &xParameterEcho );

//	#if( configGENERATE_RUN_TIME_STATS == 1 )
//	{
//		FreeRTOS_CLIRegisterCommand( &xRunTimeStats );
//	}
//	#endif
//	
//	#if( configINCLUDE_QUERY_HEAP_COMMAND == 1 )
//	{
//		FreeRTOS_CLIRegisterCommand( &xQueryHeap );
//	}
//	#endif

//	#if( configINCLUDE_TRACE_RELATED_CLI_COMMANDS == 1 )
//	{
//		FreeRTOS_CLIRegisterCommand( &xStartStopTrace );
//	}
//	#endif
//}
/*-----------------------------------------------------------*/
static BaseType_t prvTaskStatsCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
const char *const pcHeader = "     State   Priority  Stack    #\r\n************************************************\r\n";
BaseType_t xSpacePadding;

	/* Remove compile time warnings about unused parameters, and check the
	write buffer is not NULL.  NOTE - for simplicity, this example assumes the
	write buffer length is adequate, so does not check for buffer overflows. */
	( void ) pcCommandString;
	( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );

	/* Generate a table of task stats. */
	strcpy( pcWriteBuffer, "Task" );
	pcWriteBuffer += strlen( pcWriteBuffer );

	/* Minus three for the null terminator and half the number of characters in
	"Task" so the column lines up with the centre of the heading. */
	configASSERT( configMAX_TASK_NAME_LEN > 3 );
	for( xSpacePadding = strlen( "Task" ); xSpacePadding < ( configMAX_TASK_NAME_LEN - 3 ); xSpacePadding++ )
	{
		/* Add a space to align columns after the task's name. */
		*pcWriteBuffer = ' ';
		pcWriteBuffer++;

		/* Ensure always terminated. */
		*pcWriteBuffer = 0x00;
	}
	strcpy( pcWriteBuffer, pcHeader );
	vTaskList( pcWriteBuffer + strlen( pcHeader ) );

	/* There is no more data to return after this single string, so return
	pdFALSE. */
	return pdFALSE;
}
/*-----------------------------------------------------------*/

#if( configINCLUDE_QUERY_HEAP_COMMAND == 1 )

	static BaseType_t prvQueryHeapCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
	{
		/* Remove compile time warnings about unused parameters, and check the
		write buffer is not NULL.  NOTE - for simplicity, this example assumes the
		write buffer length is adequate, so does not check for buffer overflows. */
		( void ) pcCommandString;
		( void ) xWriteBufferLen;
		configASSERT( pcWriteBuffer );

		sprintf( pcWriteBuffer, "Current free heap %d bytes, minimum ever free heap %d bytes\r\n", ( int ) xPortGetFreeHeapSize(), ( int ) xPortGetMinimumEverFreeHeapSize() );

		/* There is no more data to return after this single string, so return
		pdFALSE. */
		return pdFALSE;
	}

#endif /* configINCLUDE_QUERY_HEAP */
/*-----------------------------------------------------------*/

#if( configGENERATE_RUN_TIME_STATS == 1 )
	
	static BaseType_t prvRunTimeStatsCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
	{
	const char * const pcHeader = "  Abs Time      % Time\r\n****************************************\r\n";
	BaseType_t xSpacePadding;

		/* Remove compile time warnings about unused parameters, and check the
		write buffer is not NULL.  NOTE - for simplicity, this example assumes the
		write buffer length is adequate, so does not check for buffer overflows. */
		( void ) pcCommandString;
		( void ) xWriteBufferLen;
		configASSERT( pcWriteBuffer );

		/* Generate a table of task stats. */
		strcpy( pcWriteBuffer, "Task" );
		pcWriteBuffer += strlen( pcWriteBuffer );

		/* Pad the string "task" with however many bytes necessary to make it the
		length of a task name.  Minus three for the null terminator and half the
		number of characters in	"Task" so the column lines up with the centre of
		the heading. */
		for( xSpacePadding = strlen( "Task" ); xSpacePadding < ( configMAX_TASK_NAME_LEN - 3 ); xSpacePadding++ )
		{
			/* Add a space to align columns after the task's name. */
			*pcWriteBuffer = ' ';
			pcWriteBuffer++;

			/* Ensure always terminated. */
			*pcWriteBuffer = 0x00;
		}

		strcpy( pcWriteBuffer, pcHeader );
		vTaskGetRunTimeStats( pcWriteBuffer + strlen( pcHeader ) );

		/* There is no more data to return after this single string, so return
		pdFALSE. */
		return pdFALSE;
	}
	
#endif /* configGENERATE_RUN_TIME_STATS */
/*-----------------------------------------------------------*/

static BaseType_t prvThreeParameterEchoCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
const char *pcParameter;
BaseType_t xParameterStringLength, xReturn;
static UBaseType_t uxParameterNumber = 0;

	/* Remove compile time warnings about unused parameters, and check the
	write buffer is not NULL.  NOTE - for simplicity, this example assumes the
	write buffer length is adequate, so does not check for buffer overflows. */
	( void ) pcCommandString;
	( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );

	if( uxParameterNumber == 0 )
	{
		/* The first time the function is called after the command has been
		entered just a header string is returned. */
		sprintf( pcWriteBuffer, "The three parameters were:\r\n" );

		/* Next time the function is called the first parameter will be echoed
		back. */
		uxParameterNumber = 1U;

		/* There is more data to be returned as no parameters have been echoed
		back yet. */
		xReturn = pdPASS;
	}
	else
	{
		/* Obtain the parameter string. */
		pcParameter = FreeRTOS_CLIGetParameter
						(
							pcCommandString,		/* The command string itself. */
							uxParameterNumber,		/* Return the next parameter. */
							&xParameterStringLength	/* Store the parameter string length. */
						);

		/* Sanity check something was returned. */
		configASSERT( pcParameter );

		/* Return the parameter string. */
		memset( pcWriteBuffer, 0x00, xWriteBufferLen );
		sprintf( pcWriteBuffer, "%d: ", ( int ) uxParameterNumber );
		strncat( pcWriteBuffer, pcParameter, ( size_t ) xParameterStringLength );
		strncat( pcWriteBuffer, "\r\n", strlen( "\r\n" ) );

		/* If this is the last of the three parameters then there are no more
		strings to return after this one. */
		if( uxParameterNumber == 3U )
		{
			/* If this is the last of the three parameters then there are no more
			strings to return after this one. */
			xReturn = pdFALSE;
			uxParameterNumber = 0;
		}
		else
		{
			/* There are more parameters to return after this one. */
			xReturn = pdTRUE;
			uxParameterNumber++;
		}
	}

	return xReturn;
}
/*-----------------------------------------------------------*/

static BaseType_t prvParameterEchoCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
const char *pcParameter;
BaseType_t xParameterStringLength, xReturn;
static UBaseType_t uxParameterNumber = 0;

	/* Remove compile time warnings about unused parameters, and check the
	write buffer is not NULL.  NOTE - for simplicity, this example assumes the
	write buffer length is adequate, so does not check for buffer overflows. */
	( void ) pcCommandString;
	( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );

	if( uxParameterNumber == 0 )
	{
		/* The first time the function is called after the command has been
		entered just a header string is returned. */
		sprintf( pcWriteBuffer, "The parameters were:\r\n" );

		/* Next time the function is called the first parameter will be echoed
		back. */
		uxParameterNumber = 1U;

		/* There is more data to be returned as no parameters have been echoed
		back yet. */
		xReturn = pdPASS;
	}
	else
	{
		/* Obtain the parameter string. */
		pcParameter = FreeRTOS_CLIGetParameter
						(
							pcCommandString,		/* The command string itself. */
							uxParameterNumber,		/* Return the next parameter. */
							&xParameterStringLength	/* Store the parameter string length. */
						);

		if( pcParameter != NULL )
		{
			/* Return the parameter string. */
			memset( pcWriteBuffer, 0x00, xWriteBufferLen );
			sprintf( pcWriteBuffer, "%d: ", ( int ) uxParameterNumber );
			strncat( pcWriteBuffer, ( char * ) pcParameter, ( size_t ) xParameterStringLength );
			strncat( pcWriteBuffer, "\r\n", strlen( "\r\n" ) );

			/* There might be more parameters to return after this one. */
			xReturn = pdTRUE;
			uxParameterNumber++;
		}
		else
		{
			/* No more parameters were found.  Make sure the write buffer does
			not contain a valid string. */
			pcWriteBuffer[ 0 ] = 0x00;

			/* No more data to return. */
			xReturn = pdFALSE;

			/* Start over the next time this command is executed. */
			uxParameterNumber = 0;
		}
	}

	return xReturn;
}
/*-----------------------------------------------------------*/

#if configINCLUDE_TRACE_RELATED_CLI_COMMANDS == 1

	static BaseType_t prvStartStopTraceCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
	{
	const char *pcParameter;
	BaseType_t lParameterStringLength;

		/* Remove compile time warnings about unused parameters, and check the
		write buffer is not NULL.  NOTE - for simplicity, this example assumes the
		write buffer length is adequate, so does not check for buffer overflows. */
		( void ) pcCommandString;
		( void ) xWriteBufferLen;
		configASSERT( pcWriteBuffer );

		/* Obtain the parameter string. */
		pcParameter = FreeRTOS_CLIGetParameter
						(
							pcCommandString,		/* The command string itself. */
							1,						/* Return the first parameter. */
							&lParameterStringLength	/* Store the parameter string length. */
						);

		/* Sanity check something was returned. */
		configASSERT( pcParameter );

		/* There are only two valid parameter values. */
		if( strncmp( pcParameter, "start", strlen( "start" ) ) == 0 )
		{
			/* Start or restart the trace. */
			vTraceStop();
			vTraceClear();
			vTraceStart();

			sprintf( pcWriteBuffer, "Trace recording (re)started.\r\n" );
		}
		else if( strncmp( pcParameter, "stop", strlen( "stop" ) ) == 0 )
		{
			/* End the trace, if one is running. */
			vTraceStop();
			sprintf( pcWriteBuffer, "Stopping trace recording.\r\n" );
		}
		else
		{
			sprintf( pcWriteBuffer, "Valid parameters are 'start' and 'stop'.\r\n" );
		}

		/* There is no more data to return after this single string, so return
		pdFALSE. */
		return pdFALSE;
	}

#endif /* configINCLUDE_TRACE_RELATED_CLI_COMMANDS */


static BaseType_t prvPrintEnv(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString) 
{

    /* Remove compile time warnings about unused parameters, and check the
    write buffer is not NULL.  NOTE - for simplicity, this example assumes the
    write buffer length is adequate, so does not check for buffer overflows. */
    ( void ) pcCommandString;
    ( void ) xWriteBufferLen;
    configASSERT( pcWriteBuffer );
    
    ef_print_env();
    
    /* There is no more data to return after this single string, so return
    pdFALSE. */
    return pdFALSE;

}


static BaseType_t prvGetEnv(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString)
{
    char *pcParameter;
    BaseType_t xParameterStringLength;
    char * xReturn;

	/* Remove compile time warnings about unused parameters, and check the
	write buffer is not NULL.  NOTE - for simplicity, this example assumes the
	write buffer length is adequate, so does not check for buffer overflows. */
	( void ) pcCommandString;
	( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );    

    
    pcParameter = (char *)FreeRTOS_CLIGetParameter(pcCommandString, 1, &xParameterStringLength);    

	/* Sanity check something was returned. */
	configASSERT( pcParameter );

	/* Return the parameter string. */
	memset( pcWriteBuffer, 0x00, xWriteBufferLen );
	sprintf( pcWriteBuffer, "%d: ", ( int ) xParameterStringLength );
	strncat( pcWriteBuffer, pcParameter, ( size_t ) xParameterStringLength );
	strncat( pcWriteBuffer, "\r\n", strlen( "\r\n" ) );

    xReturn = ef_get_env(pcParameter);  

    strncat( pcWriteBuffer, xReturn, strlen( xReturn) );
    strncat( pcWriteBuffer, "\r\n", strlen( "\r\n" ) );
	
	if(xReturn != EF_NO_ERR)
	{
        sprintf( pcWriteBuffer, "setenv error \r\n" );
    }
	
    return pdFALSE;
}
static BaseType_t prvSetValue(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString)
{
    char *pcParameter1;
    char *pcParameter2;
    BaseType_t xParameter1StringLength;
    BaseType_t xParameter2StringLength;
    EfErrCode xReturn;

	/* Remove compile time warnings about unused parameters, and check the
	write buffer is not NULL.  NOTE - for simplicity, this example assumes the
	write buffer length is adequate, so does not check for buffer overflows. */
	( void ) pcCommandString;
	( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );    

    
    pcParameter1 = (char *)FreeRTOS_CLIGetParameter(pcCommandString, 1, &xParameter1StringLength);
    pcParameter2 = (char *)FreeRTOS_CLIGetParameter(pcCommandString, 2, &xParameter2StringLength);

    pcParameter1[xParameter1StringLength] = 0x00;
    pcParameter2[xParameter2StringLength] = 0x00;

    xReturn = ef_set_env(pcParameter1, pcParameter2);  
	
	if(xReturn != EF_NO_ERR)
	{
        sprintf( pcWriteBuffer, "setenv error \r\n" );
    }
	
    return pdFALSE;    
}

//static BaseType_t prvResetEnv(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString);














