
/*
* DBGMACRO.C
* In this program, calls are made to the _ASSERT and _ASSERTE macros to test the condition 'string1 == string2'.  
* If the condition fails, these macros print a diagnostic message.
*
* The _RPTn and _RPTFn group of macros are also exercised in this program, as an alternative to the printf function.
*/
#define  _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <crtdbg.h>
#include <stdarg.h>
#include <ctype.h>

int main__rpt()
{
	char *p1, *p2;

	/*
	* The Reporting Mode and File must be specified
	* before generating a debug report via an assert
	* or report macro.
	* This program sends all report types to STDOUT
	*/
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDOUT);
	_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDOUT);

	/*
	* Allocate and assign the pointer variables
	*/
	p1 = (char*)malloc(10);
	strcpy(p1, "I am p1");
	p2 = (char*)malloc(10);
	strcpy(p2, "I am p2");

	/*
	* Use the report macros as a debugging warning mechanism, similar to printf.
	*
	* Use the assert macros to check if the p1 and p2 variables are equivalent.
	*
	* If the expression fails, _ASSERTE will include a string representation of the faild
	* expression in the report.  _ASSERT does not include the expression in the generated report.
	*/
	_RPT0(_CRT_WARN, "\n\n Use the assert macros to evaluate the expression p1 == p2.\n");
	_RPTF2(_CRT_WARN, "\n Will _ASSERT find '%s' == '%s' ?\n", p1, p2);
	_ASSERT(p1 == p2);

	_RPTF2(_CRT_WARN, "\n\n Will _ASSERTE find '%s' == '%s' ?\n", p1, p2);
	_ASSERTE(p1 == p2);

	_RPT2(_CRT_ERROR, "\n \n '%s' != '%s'\n", p1, p2);

	free(p2);
	free(p1);

	//sprintf(msgbuf, "My variable is %d\n", integerVariable);
	OutputDebugStringW(L"Hello!" /*msgbuf*/);

	return 0;
}