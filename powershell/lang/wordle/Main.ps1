#!/usr/bin/env pwsh

<#
    .SYNOPSIS
    Wordle game in PowerShell.

    .DESCRIPTION
    Play Wordle from your terminal with this simple script.
    Three files will be generated:
    - known.txt:     all words valid for input.
    - available.txt: all words that can be used to play.
    - used.txt:      all words that have been used to play.

    .PARAMETER play
    Start playing the game.

    .EXAMPLE
    ./Main.ps1 -Play

    .PARAMETER Add
    You can add a word with this parameter.

    .EXAMPLE
    ./Main.ps1 -Add "queue"

    .PARAMETER Reset
    Dumps known.txt into available.txt and empties used.txt.

    .EXAMPLE
    ./Main.ps1 -Reset

    .NOTES
    Author: Cristian Marcos Martín
#>

param (
    [switch] $Play,
    [string] $Add,
    [switch] $Reset
)

if (-not $PSBoundParameters.Keys.Count) {
    Write-Host "You must provide at least one argument."
    Write-Host "Use 'Get-Help ./Main.ps1' if you are stuck."
    Write-Host
    exit 0
}

$Global:ProjectRoot = $PSScriptRoot

. "$PSScriptRoot/src/Api.ps1"
. "$PSScriptRoot/src/Fs.ps1"
. "$PSScriptRoot/src/Game.ps1"
. "$PSScriptRoot/src/Constants.ps1"

if (-not (Test-Path $filesPath)) {
    New-Item -ItemType Directory -Path $filesPath | Out-Null
}

if (-not (Test-Path -Path $knownWordsPath)) { Get-AllWords }

$words = Read-File -Path $availableWordsPath

if ($Reset) {
    Write-File -Path $availableWordsPath -Content (Read-File -Path $knownWordsPath) -Overwrite
    Write-File -Path $usedWordsPath -Clear

    Write-Host "Available and used words resetted correctly."

    exit 0
}

if ($Add) {
    if ($Add -match '[^a-z]') {
        Write-Host "Only lowercase letters a-z are allowed."
        exit 1
    }

    if ($Add.Length -ne 5) {
        Write-Host "Please write exactly 5 letters."
        exit 1
    }

    if ($Add -in $words) {
        Write-Host "Word is already known."
        exit 1
    }

    Write-File -Path $availableWordsPath -Content $Add
    Write-File -Path $knownWordsPath -Content $Add

    Write-Host "Word added successfully."
}

if ($Play) {
    $rnd = Get-Random -Minimum 0 -Maximum $words.Length
    $word = $words[$rnd]

    $exit_code = Start-Game -Word $word -Tries 6

    $words = $words | Where-Object { $_ -ne $word }

    if ($exit_code -ne -1) {
        Write-File -Path $availableWordsPath -Content $words -Overwrite
        Write-File -Path $usedWordsPath -Content $word
    }

    exit $exit_code
}
