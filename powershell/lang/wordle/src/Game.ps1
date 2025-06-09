function New-Game {
    [OutputType([string[][]])]
    param (
        [Parameter(Mandatory=$true)]
        [string] $word,

        [Parameter(Mandatory=$true)]
        [int] $tries
    )

    $default = $gray + "_"

    $row = @($default) * $word.Length
    $game = 1..$tries | ForEach-Object { ,($row.Clone()) }

    $game
}

function Start-Game {
    [OutputType([int])]
    param (
        [Parameter(Mandatory=$true)]
        [string] $word,

        [Parameter(Mandatory=$true)]
        [int] $tries
    )

    $game = New-Game -word $word -tries $tries

    $message = ""
    $available = "a".."z"

    $valid = Read-File -Path $knownWordsPath

    $used = @()
    $used_fmt = @{}

    for ($try = 0; $try -lt $tries; $try++) {
        do {
            [Console]::Clear()

            # Render letter info
            Write-Host "Available:", (($available | Sort-Object) -join " ")
            Write-Host "     Used:" -NoNewline
            foreach ($key in ($used | Sort-Object)) { Write-Host $used_fmt[$key]$key -NoNewline }
            Write-Host

            # Render game
            Write-Host
            foreach ($line in $game) { Write-Host $line, $ansiReset }

            # Render input placeholder
            Write-Host $message
            Write-Host "> " -NoNewline
            $read = Read-Host

            if ($null -eq $read) {
                Write-Host
                Write-Host "Game finished"
                return -1
            }
            
            $letters = $read.ToLower().ToCharArray()

            if ($letters.Length -ne $word.Length) {
                $message = "Please write exactly $($word.Length) letters."
            }
            elseif ($letters -match '[^a-z]') {
                $message = "Only lowercase letters a-z are allowed."
            }
            elseif (-not ($valid -contains ($letters -join ''))) {
                $message = "Unknown word: '$read'"
            }
            else {
                $message = ""
            }

        } while ($message)

        $available = $available | Where-Object -FilterScript { $_ -notin $letters }

        for ($pos = 0; $pos -lt $word.Length; $pos++) {
            $wordLetter = $word[$pos]
            $letter = $letters[$pos]

            if ($letter -notin $used) { $used += $letter }

            $color = ($letter -eq $wordLetter) `
                ? $green
                : ($word.Contains($letter)) `
                    ? $yellow
                    : $gray

            if ($used_fmt.ContainsKey($letter)) {
                $used_color = $used_fmt[$letter]

                if (($used_color -eq $gray) -and ($color -ne $gray)) {
                    $used_fmt[$letter] = $color
                }

                if (($used_color -eq $yellow) -and ($color -eq $green)) {
                    $used_fmt[$letter] = $color
                }
            }

            $used_fmt[$letter] = $color

            $game[$try][$pos] = $color + $letter
        }

        if ($word -eq ($letters -join "")) {
            Write-Host "You win!"
            return 0
        }

        if ($try + 1 -eq $tries) {
            Write-Host "You lose! The word was" $word
            return 1
        }
    }
}
