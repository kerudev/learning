function Get-AllWords {
    $set = [System.Collections.Generic.HashSet[string]]::new()

    for ($i = 0; $i -lt 8; $i++) {
        $uri = $apiUrl + "?length=5&words=500"
        $set.UnionWith([string[]](Invoke-RestMethod $uri))
    }

    Write-File -Path $knownWordsPath -Content $set
    Write-File -Path $availableWordsPath -Content $set
}
