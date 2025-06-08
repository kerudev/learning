function Write-File {
    [Parameter(Mandatory=$true, Position=0)]
    [string] $word

    Add-Content -Path "words.txt" -Value $word
}

function Read-File {
    Get-Content -Path "words.txt"
}