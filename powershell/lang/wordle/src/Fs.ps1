function Write-File {
    param (
        [Parameter(Mandatory=$true)]
        [string] $path,

        [AllowEmptyString()]
        [AllowNull()]
        [string[]] $content,

        [switch] $overwrite = $false,
        [switch] $clear = $false
    )

    if ((-not $clear) -and (-not $content)) {
        Write-Host "You must provide -Content if you are not using -Clear."
        exit 1
    }

    if ($clear) {
        Clear-Content -Path $path
        return
    }

    $func = if ($overwrite) { 'Set-Content' } else { 'Add-Content' }
    & $func -Path $path -Value ($content -join "`n")
}

function Read-File {
    [OutputType([string[]])]
    param (
        [Parameter(Mandatory=$true)]
        [string] $path
    )

    ((Test-Path -Path $path) -and (Get-Content -Path $path -Raw).Length) `
        ? (Get-Content -Path $path)
        : @()
}
