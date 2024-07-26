@echo off

if "%1"=="" (
	pio run -e uno -t upload
) else (
	pio run -e uno -t upload --upload-port %1
)