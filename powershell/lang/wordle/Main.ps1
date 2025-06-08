#!/usr/bin/env pwsh

. "$PSScriptRoot/src/Api.ps1"
. "$PSScriptRoot/src/Fs.ps1"
. "$PSScriptRoot/src/Game.ps1"

$words = Read-File
# $word = Get-Word -exclude $words
# Write-File -word $word

Start-Game -word $words[0] -tries 6
