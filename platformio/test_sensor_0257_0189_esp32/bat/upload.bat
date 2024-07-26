@echo off

if "%1"=="" (
	pio run -e esp32doit-devkit-v1 -t upload
) else (
	pio run -e esp32doit-devkit-v1 -t upload --upload-port %1
)