
#include "codepages.h"

#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include <fcntl.h>
#include <io.h>
#include <locale.h>

#include <windows.h>
#pragma lib "user32.lib"

int main(int argc, char * argv[])
{
	char * source;
	size_t converted, invalid, length;
	UTF16Char utf16target[32];
	UTF8Char utf8target[32];
	wchar_t wmessage[128];
	char message[128];
	source  = "\x81\x40Hello";
	length = strlen(source);
	wprintf(L"Length %d [%s]\n", length + 1, source);
	converted = codepage_to_utf16(CODEPAGE_932, (ANSIPtr)source, strlen(source) + 1, utf16target, 32, &invalid);
	if(converted > 0 && converted < 16)
	{
		swprintf(wmessage, 128, L"Converted %d invalid %d [%ls]\n", converted, invalid, (wchar_t*)utf16target);
		wprintf(wmessage);
		MessageBoxW(NULL, wmessage, L"ok", MB_TOPMOST);
		// With BOM
		strcpy((char*)utf8target, "\xEF\xBB\xBF");
		converted = codepage_to_utf8(CODEPAGE_932, (ANSIPtr)source, strlen(source) + 1, &utf8target[3], 29, &invalid);
		//converted = codepage_to_utf8(CODEPAGE_932, (ANSIPtr)source, strlen(source) + 1, &utf8target[0], 29, &invalid);
		if(converted > 0 && converted < 16)
		{
			// UTF8 doesn't display correctly...
			setlocale(LC_ALL, "");
  			swprintf(wmessage, 128, L"Converted %d invalid %d [%s]\n", converted, invalid, (char*)utf8target);
			wprintf(wmessage);
			MessageBoxW(NULL, wmessage, L"ok", MB_TOPMOST);
		}		
	}
	return 0;
}
