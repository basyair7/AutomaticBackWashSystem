# monitor.ps1

param (
    [string]$port
)

# Use the provided port to run the pio command
if ($null -eq $port) {
    Invoke-Expression "pio device monitor"
} else {
    Invoke-Expression "pio device monitor --port $port"
}
