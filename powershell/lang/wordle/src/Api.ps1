. "$PSScriptRoot/Constants.ps1"

function Get-Word {
    [OutputType([string])]
    param (
        [Parameter(Mandatory=$false, Position=0)]
        [System.Object[]] $exclude = @()
    )

    do {
        $word = (Invoke-RestMethod -Uri $apiUrl)[0]
    } while($word -in $exclude)

    $word
}
