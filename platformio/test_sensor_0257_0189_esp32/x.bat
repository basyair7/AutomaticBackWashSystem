@echo off

IF "%1" == "" (
    echo ShortCut Command PlatformIO
    echo running example : "exc <command> <port>"
    echo exc monitor or exc monitor com0 
    echo.
    echo list commands :
    for %%f in (bat\*.bat) do (
        ::set "filelist=!filelist! %%~nxf"
        echo - %%~nxf
    )

) ELSE (
    CALL bat\\%1 %2
)