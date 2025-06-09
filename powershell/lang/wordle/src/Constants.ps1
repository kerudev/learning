New-Variable apiUrl -Value "https://random-word-api.vercel.app/api"

New-Variable filesPath -Value "$Global:ProjectRoot/files" -Option ReadOnly
New-Variable knownWordsPath -Value "$filesPath/known.txt" -Option ReadOnly
New-Variable availableWordsPath -Value "$filesPath/available.txt" -Option ReadOnly
New-Variable usedWordsPath -Value "$filesPath/used.txt" -Option ReadOnly

New-Variable green -Value "`e[42;1;30m" -Option ReadOnly
New-Variable yellow -Value "`e[43;1;30m" -Option ReadOnly
New-Variable gray -Value "`e[47;1;30m" -Option ReadOnly
New-Variable ansiReset -Value "`e[0m" -Option ReadOnly
