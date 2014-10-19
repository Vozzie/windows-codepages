
REM node download.js

del ..\inc\wincp.h
echo #pragma once >> wincp.h
echo #ifndef __WINCP_HEADER__ >> wincp.h
echo #define __WINCP_HEADER__ >> wincp.h
echo. >> wincp.h
echo #include "codepages.h" >> wincp.h
echo. >> wincp.h
echo // Windows >> wincp.h
node make_utf16_table.js CP874.txt >> wincp.h
node make_utf16_table.js CP1250.txt >> wincp.h
node make_utf16_table.js CP1251.txt >> wincp.h
node make_utf16_table.js CP1252.txt >> wincp.h
node make_utf16_table.js CP1253.txt >> wincp.h
node make_utf16_table.js CP1254.txt >> wincp.h
node make_utf16_table.js CP1255.txt >> wincp.h
node make_utf16_table.js CP1256.txt >> wincp.h
node make_utf16_table.js CP1257.txt >> wincp.h
node make_utf16_table.js CP1258.txt >> wincp.h
echo. >> wincp.h
REM echo // EBCDIC >> wincp.h
REM node make_utf16_table.js CP037.txt >> wincp.h
REM node make_utf16_table.js CP500.txt >> wincp.h
REM node make_utf16_table.js CP875.txt >> wincp.h
REM node make_utf16_table.js CP1026.txt >> wincp.h

echo. >> wincp.h
echo //Windows >> wincp.h
echo. >> wincp.h
node make_utf16_table.js CP932.txt 1 >> wincp.h
echo. >> wincp.h
node make_utf16_table.js CP936.txt 1 >> wincp.h
echo. >> wincp.h
node make_utf16_table.js CP949.txt 1 >> wincp.h
echo. >> wincp.h
node make_utf16_table.js CP950.txt 1 >> wincp.h
echo. >> wincp.h
echo #endif//__WINCP_HEADER__ >> wincp.h

move wincp.h ..\inc\wincp.h