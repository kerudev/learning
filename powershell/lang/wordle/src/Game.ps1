function New-Game {
    param (
        [Parameter(Mandatory=$true)]
        [string] $word,

        [Parameter(Mandatory=$true)]
        [int] $tries
    )

    $default = "`e[47;1;30m`_"

    $row = 1..$word.Length | ForEach-Object { $default }
    $game = 1..$tries | ForEach-Object { ,$row.Clone() }

    $game

}

function Start-Game {
    param (
        [Parameter(Mandatory=$true)]
        [string] $word,

        [Parameter(Mandatory=$true)]
        [int] $tries
    )

    $game = New-Game -word $word -tries $tries

    $available = "a".."z"
    $used = @()

    for ($try = 0; $try -lt $tries; $try++) {
        do {
            # Clear-Host

            Write-Host "Available:", (($available | Sort-Object) -join "")
            Write-Host "     Used:", (($used | Sort-Object) -join "")

            foreach ($line in $game) {
                Write-Host $line, "`e[0m"
            }

            Write-Host "> " -NoNewline
            $read = Read-Host

            if ($null -eq $read) {
                Write-Host "Game finished"
                exit 0
            }
            
            $letters = $read.ToLower().ToCharArray()

            if ($letters.Count -ne $word.Length) {
                Write-Host "You have to write" $word.Length "letters"
            }
        } while ($letters.Count -ne $word.Length)

        $available = $available | Where-Object -FilterScript { $_ -notin $letters }

        for ($pos = 0; $pos -lt $word.Length; $pos++) {
            $wordLetter = $word[$pos]
            $letter = $letters[$pos]

            if ($letter -notin $used) {
                $used += $letter
            }

            $color = ($letter -eq $wordLetter) `
                ? "`e[42;1;30m"     # green
                : ($word.Contains($letter)) `
                    ? "`e[43;1;30m" # yellow
                    : "`e[47;1;30m" # gray
    
            $game[$try][$pos] = $color + $letter
        }

        if ($word -eq ($letters -join "")) {
            Write-Host "You win!"
            exit 0
        }

        if ($try + 1 -eq $tries) {
            Write-Host "You lose! The word was" $word
            exit 1
        }
    }
}
